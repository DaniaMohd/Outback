#include "main.h"
//#include "GameObjects.h"

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
	if (pObject->type == TYPE_OBJECT_BULLET)
	{
		AEMtx33Scale(&scale1, scale, scale);
	}
	else
	{
		if (dirFaceR)
		{
			AEMtx33Scale(&scale1, scale, scale);
		}
		else
		{
			AEMtx33Scale(&scale1, -scale, scale);
		}
	}
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
	Creates Enemy
*/
/******************************************************************************/
void Enemy::enemyCreate(unsigned int enemyType, AEVec2* pPos)
{
	AEVec2 vel;
	AEVec2Zero(&vel);
	gameObjInstCreate(enemyType, 1.0f, pPos, &vel, 0);

	state = STATE::STATE_GOING_LEFT;
	innerState = INNER_STATE::INNER_STATE_ON_ENTER;
	healthPoints = 100;
}

/******************************************************************************/
/*!
	Enemy state behaviour
*/
/******************************************************************************/
void Enemy::EnemyStateMachine()
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
	Enemy creates bullet
*/
/******************************************************************************/
void Enemy::enemyFire(Player character, Projectile *bullet)
{
	// Weird warning if there are no (double) cast
	float x = (float)(((double)character.posCurr.x - posCurr.x) / sqrt(pow((double)character.posCurr.x - posCurr.x, 2.0f) + pow((double)character.posCurr.y - posCurr.y, 2.0f)));
	float y = (float)(((double)character.posCurr.y - posCurr.y) / sqrt(pow((double)character.posCurr.x - posCurr.x, 2.0f) + pow((double)character.posCurr.y - posCurr.y, 2.0f)));

	float angle = (float)acos(x);
	
	//bullet velocity
	//float x or y * 10(velocity)
	AEVec2 vel = { x * 10,y * 10 };
	//flip the angle if the bullet is suppose to go downwards
	if (posCurr.y > character.posCurr.y)
	{
		angle *= -1;
	}
	
	//find empty slot in the projectile array
	for (size_t i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		if (bullet[i].flag == 0)
		{
			bullet[i].ProjectileCreate(&posCurr, &vel, angle);
			break;
		}
	}
	//bullet.ProjectileCreate(&posCurr, &vel, angle);
}

/******************************************************************************/
/*!
	Creates player
*/
/******************************************************************************/
void Player::playerCreate(AEVec2* pPos)
{
	AEVec2 vel;
	AEVec2Zero(&vel);
	gameObjInstCreate(TYPE_OBJECT_HERO, 1.0f, pPos, &vel, 0);
	boomerangRange = 5.0f;
	projectileMax = 1;
	damage = 1;
}

/******************************************************************************/
/*!
	Player creates boomerang
*/
/******************************************************************************/
void Player::playerFire(Projectile *boomerang)
{
	AEVec2 vel;
	AEVec2Zero(&vel);
	boomerangRange = 5 + 1 * powerRange;
	//boomerang.projectileTime = zero;
	//boomerang.initialPos = posCurr;
	//boomerang.projectileReturning = false;
	float tmp = 0.5 * powerRange;
	vel.x = (dirFaceR) ? 10.0f + tmp : -10.0f - tmp;
	//if (dirFaceR)
	//{
	//	vel.x = 10;
	//}
	//else
	//{
	//	vel.x = -10;
	//}

	//find empty slot in the projectile array
	for (size_t i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		if (boomerang[i].flag == 0)
		{
			boomerang[i].boomerangCreate(&posCurr, &vel, boomerangRange);
			break;
		}
	}
	//boomerang.boomerangCreate(&posCurr, &vel, boomerangRange);
}

/******************************************************************************/
/*!
	Creates boomerang
*/
/******************************************************************************/
void Projectile::boomerangCreate(AEVec2* startPosition, AEVec2* vel, float range)
{
	AEVec2 zero;
	AEVec2Zero(&zero);

	gameObjInstCreate(TYPE_OBJECT_BOOMERANG, 1.0f, startPosition, vel, 0);
	boomerangTime = zero;
	boomerangReturning = false;
	projectileRange = range;
	initialPos.x = startPosition->x;
	initialPos.y = startPosition->y;
}

/******************************************************************************/
/*!
	Boomerang returns to whoever threw it
*/
/******************************************************************************/
void Projectile::boomerangUpdate(Player character)
{
	if (boomerangReturning)
	{
		float x_distance = posCurr.x - character.posCurr.x;
		float y_distance = posCurr.y - character.posCurr.y;
		velCurr.x = -x_distance / (boomerangTime.x -= g_dt);
		velCurr.y = -y_distance / (boomerangTime.y -= g_dt);
	}
	else
	{
		boomerangTime.x += g_dt;
		boomerangTime.y += g_dt;
		if (posCurr.x >= initialPos.x + projectileRange || posCurr.x <= initialPos.x - projectileRange)
		{
			boomerangReturning = true;
		}
	}
}

/******************************************************************************/
/*!
	Creates bullet
*/
/******************************************************************************/
void Projectile::ProjectileCreate(AEVec2* startPosition, AEVec2* vel, float angle)
{
	gameObjInstCreate(TYPE_OBJECT_BULLET, 1.0f, startPosition, vel, angle);
	//resets counter
	counter = 0;
}

/******************************************************************************/
/*!
	Updates bullet
*/
/******************************************************************************/
void Projectile::ProjectileUpdate()
{
	counter += g_dt;
	//time before bullet despawns
	if (counter > 2)
	{
		flag = 0;
	}
}

