#include "main.h"

/******************************************************************************/
/*!
	Creates game object
*/
/******************************************************************************/
void  GameObjInst::gameObjInstCreate(unsigned int type, float scale1, AEVec2* pPos, AEVec2* pVel, float dir)
{
	AEVec2 zero;
	AEVec2Zero(&zero);

	AE_ASSERT_PARM(type < sGameObjNum);

	pObject = sGameObjList + type;
	flag = FLAG_ACTIVE | FLAG_VISIBLE;
	scale = scale1;
	posCurr = pPos ? *pPos : zero;
	velCurr = pVel ? *pVel : zero;
	dirCurr = dir;
	dirFaceR = true;
}

/******************************************************************************/
/*!
	Destroys game object
*/
/******************************************************************************/
void GameObjInst::gameObjInstDestroy()
{
	flag = 0;
}

/******************************************************************************/
/*!
	Sets game object bounding box
*/
/******************************************************************************/
void GameObjInst::gameObjInstBoundingBox()
{
	AEVec2Set(&boundingBox.min, scale * -0.5f, scale * -0.5f);
	AEVec2Add(&boundingBox.min, &boundingBox.min, &posCurr);
	AEVec2Set(&boundingBox.max, scale * 0.5f, scale * 0.5f);
	AEVec2Add(&boundingBox.max, &boundingBox.max, &posCurr);
}

/******************************************************************************/
/*!
	Updates game object position
*/
/******************************************************************************/
void GameObjInst::gameObjInstUpdatePos()
{
	posCurr.x = velCurr.x * g_dt + posCurr.x;
	posCurr.y = velCurr.y * g_dt + posCurr.y;
	if (velCurr.x < 0)
		dirFaceR = false;
	else if (velCurr.x > 0)
		dirFaceR = true;
}

/******************************************************************************/
/*!
	Sets game object transformation matrix
*/
/******************************************************************************/
void GameObjInst::gameObjInstTransformMatrix()
{
	AEMtx33 scale1, rot, trans, result;
	if(dirFaceR)
		AEMtx33Scale(&scale1, scale, scale);
	else
		AEMtx33Scale(&scale1, -scale, scale);
	// Compute the rotation matrix 
	AEMtx33Rot(&rot, dirCurr);
	// Compute the translation matrix
	AEMtx33Trans(&trans, posCurr.x, posCurr.y);
	// Concatenate the 3 matrix in the correct order in the object instance's "transform" matrix
	AEMtx33Concat(&result, &rot, &scale1);
	AEMtx33Concat(&transform, &trans, &result);
}

/******************************************************************************/
/*!
	Draws game object
*/
/******************************************************************************/
void GameObjInst::gameObjInstDrawObject(AEMtx33* map)
{
	AEMtx33 cellTranslation, cellFinalTransformation;
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTextureMode(AE_GFX_TM_PRECISE);
	AEGfxSetTransparency(1.0f);
	AEMtx33Concat(&cellFinalTransformation, map, &transform);
	AEGfxSetTransform(cellFinalTransformation.m);
	AEGfxTextureSet(pObject->pTex, 0, 0);
	AEGfxMeshDraw(pObject->pMesh, AE_GFX_MDM_TRIANGLES);
	//Don't forget to concatenate the MapTransform matrix with the transformation of each game object instance
}

/******************************************************************************/
/*!
	Creates characters
*/
/******************************************************************************/
void Character::characterCreate(unsigned int type, float scale1, AEVec2* pPos, AEVec2* pVel, float dir, enum STATE startState)
{
	gameObjInstCreate(type, scale1, pPos, pVel, dir);

	state = startState;
	innerState = INNER_STATE::INNER_STATE_ON_ENTER;
}

/******************************************************************************/
/*!
	Enemy state behaviour
*/
/******************************************************************************/
void Character::EnemyStateMachine()
{
	switch (state)
	{
		case STATE::STATE_GOING_LEFT:
		{
			switch (innerState)
			{
				case INNER_STATE::INNER_STATE_ON_ENTER:
				{
					velCurr.x = -MOVE_VELOCITY_ENEMY;
					innerState = INNER_STATE::INNER_STATE_ON_UPDATE;
				}
				break;

				case INNER_STATE::INNER_STATE_ON_UPDATE:
				{
					if (gridCollisionFlag & COLLISION_LEFT || !(GetCellValue((int)(posCurr.x - 0.5f), (int)(posCurr.y - 1.0f))))
					{
						counter = ENEMY_IDLE_TIME;
						innerState = INNER_STATE::INNER_STATE_ON_EXIT;
						velCurr.x = 0;
						SnapToCell(&posCurr.x);
					}
				}
				break;

				case INNER_STATE::INNER_STATE_ON_EXIT:
				{
					counter -= g_dt;
					if (counter <= 0)
					{
						state = STATE::STATE_GOING_RIGHT;
						innerState = INNER_STATE::INNER_STATE_ON_ENTER;
					}
				}
				break;
			}
		}
		break;

		case STATE::STATE_GOING_RIGHT:
		{
			switch (innerState)
			{
				case INNER_STATE::INNER_STATE_ON_ENTER:
				{
					velCurr.x = MOVE_VELOCITY_ENEMY;
					innerState = INNER_STATE::INNER_STATE_ON_UPDATE;
				}
				break;

				case INNER_STATE::INNER_STATE_ON_UPDATE:
				{
					if (gridCollisionFlag & COLLISION_RIGHT || !(GetCellValue((int)(posCurr.x + 0.5f), (int)(posCurr.y - 1.0f))))
					{
						counter = ENEMY_IDLE_TIME;
						innerState = INNER_STATE::INNER_STATE_ON_EXIT;
						velCurr.x = 0;
						SnapToCell(&posCurr.x);
					}
				}
				break;

				case INNER_STATE::INNER_STATE_ON_EXIT:
				{
					counter -= g_dt;
					if (counter <= 0)
					{
						state = STATE::STATE_GOING_LEFT;
						innerState = INNER_STATE::INNER_STATE_ON_ENTER;
					}

				}
				break;
			}
		}
		break;
	}
}

/******************************************************************************/
/*!
	Creates projectile baised on who shot it
*/
/******************************************************************************/
void Character::boomerangCreate(Character character)
{
	AEVec2 zero;
	AEVec2Zero(&zero);
	AEVec2 vel;
	AEVec2Zero(&vel);

	projectileRange = 10;
	projectileTime = zero;
	initialPos = character.posCurr;
	projectileReturning = false;
	if (character.dirFaceR)
	{
		vel.x = 10;
	}
	else
	{
		vel.x = -10;
	}
	// replaace type object with boomerang
	gameObjInstCreate(TYPE_OBJECT_ENEMY1, 1.0f, &character.posCurr, &vel, character.dirCurr);
}

/******************************************************************************/
/*!
	Boomerang returns to whoever threw it
*/
/******************************************************************************/
void Character::boomerangReturn(Character character)
{
	if (projectileReturning)
	{
		float x_distance = posCurr.x - character.posCurr.x;
		float y_distance = posCurr.y - character.posCurr.y;
		velCurr.x = -x_distance / (projectileTime.x -= g_dt);
		velCurr.y = -y_distance / (projectileTime.y -= g_dt);
	}
	else
	{
		projectileTime.x += g_dt;
		projectileTime.y += g_dt;
		if (posCurr.x >= initialPos.x + projectileRange || posCurr.x <= initialPos.x - projectileRange)
		{
			projectileReturning = true;
		}
	}
}