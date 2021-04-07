#include "main.h"

extern s8 fontID;

//GameObjInst functions
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
	if (pObject->type != TYPE_OBJECT_ENEMY2 && pObject->type != TYPE_OBJECT_ENEMY3)
	{
		if (velCurr.x < 0)
			dirFaceR = false;
		else if (velCurr.x > 0)
			dirFaceR = true;
	}

	if (pObject->type == TYPE_OBJECT_BOOMERANG)
	{
		dirCurr -= 10.0f * PI * g_dt;
	}
	//### probs can update gravity and collisionflag here
}

/******************************************************************************/
/*!
	Sets game object transformation matrix
*/
/******************************************************************************/
void GameObjInst::gameObjInstTransformMatrix()
{
	AEMtx33 scale1, rot, trans, result;
	if (pObject->type == TYPE_OBJECT_BULLET )
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
	Particle effects
*/
/******************************************************************************/
void GameObjInst::particleEffect(GameObjInst* particle, unsigned int type)
{
	AEVec2 pos; //position to come out from
	AEVec2 vel;
	if (type == P_TRAIL)
	{
		for (unsigned int i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
		{
			if (particle[i].flag == 0)
			{
				if (dirFaceR)
				{
					pos = { posCurr.x - 0.5f, posCurr.y - 0.5f };
					vel = { AERandFloat() * -10 , AERandFloat() * 10 };
				}
				else
				{
					pos = { posCurr.x + 0.5f, posCurr.y - 0.5f };
					vel = { AERandFloat() * 10 , AERandFloat() * 10 };
				}
				
				particle[i].gameObjInstCreate(TYPE_OBJECT_PARTICLES, 0.25f, &pos, &vel, 0);
				particle[i].counter = 0.25;
				break;
			}
		}
	}
	if (type == P_HIT)
	{
		for (unsigned int i = 0, j = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
		{
			pos = { posCurr.x, posCurr.y };
			if (particle[i].flag == 0)
			{
				if (j >= 4)
				{
					break;
				}
				else if (j == 0)
				{
					vel = { 5 , 5 };
				}
				else if (j == 1)
				{
					vel = { -5 , 5 };
				}
				else if (j == 2)
				{
					vel = { 5 , -5 };
				}
				else if (j == 3)
				{
					vel = { -5 , -5 };
				}

				particle[i].gameObjInstCreate(TYPE_OBJECT_PARTICLES, 0.25f, &pos, &vel, 0);
				particle[i].counter = 0.25;
				j++;
			}
		}
	}
}

/******************************************************************************/
/*!
	Spawns random upgrades
*/
/******************************************************************************/
void GameObjInst::PowerUpCreate(AEVec2 pos)
{
	int powerNum = rand() % 6;
	AEVec2 zero;
	AEVec2Zero(&zero);
	switch (powerNum)
	{
	case 0:
		gameObjInstCreate(TYPE_OBJECT_DAMAGE, 1.0f, &pos, &zero, 0);
		break;
	case 1:
		gameObjInstCreate(TYPE_OBJECT_RANGE, 1.0f, &pos, &zero, 0);
		break;
	case 2:
		gameObjInstCreate(TYPE_OBJECT_SPEED, 1.0f, &pos, &zero, 0);
		break;
	case 3:
		gameObjInstCreate(TYPE_OBJECT_HPUP, 1.0f, &pos, &zero, 0);
		break;
	case 4:
		gameObjInstCreate(TYPE_OBJECT_VAMP, 1.0f, &pos, &zero, 0);
		break;
	case 5:
		gameObjInstCreate(TYPE_OBJECT_REGEN, 1.0f, &pos, &zero, 0);
		break;
	default:
		break;
	}
}





//Enemy functions
/******************************************************************************/
/*!
	Creates Enemy
*/
/******************************************************************************/
void Enemy::enemyCreate(unsigned int enemyType, AEVec2* pPos)
{
	AEVec2 vel;
	AEVec2Zero(&vel);
	//pPos->y += 1; //cuz enemy resized 2 times bigger
	gameObjInstCreate(enemyType, 1.0f, pPos, &vel, 0);

	state = STATE::STATE_GOING_LEFT;
	innerState = INNER_STATE::INNER_STATE_ON_ENTER;
	healthPoints = 50;
	hit1 = false;
	hit2 = false;
	damage = 10;
	detectionRadius = 10.0f;
	counter1 = float(rand()) / float((RAND_MAX));
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
	Enemy grid collision flag update
*/
/******************************************************************************/
void Enemy::enemyGridFlag()
{
	if (gridCollisionFlag & COLLISION_BOTTOM)
	{
		SnapToCell(&posCurr.y);
		velCurr.y = 0;
	}
	if (gridCollisionFlag & COLLISION_TOP)
	{
		SnapToCell(&posCurr.y);
		velCurr.y = 0;
	}
	if (gridCollisionFlag & COLLISION_LEFT)
	{
		SnapToCell(&posCurr.x);
		velCurr.x = 0;
	}
	if (gridCollisionFlag & COLLISION_RIGHT)
	{
		SnapToCell(&posCurr.x);
		velCurr.x = 0;
	}
}

/******************************************************************************/
/*!
	Enemy creates bullet
*/
/******************************************************************************/
void Enemy::enemyFire(Player character, Projectile* bullet)
{
	float mag = sqrt(pow((double)character.posCurr.x - posCurr.x, 2.0f) + pow((double)character.posCurr.y - posCurr.y, 2.0f));
	//detect player and can shoot
	if (mag <= detectionRadius && counter >= 2.0f)
	{
		counter = 0;
		// Weird warning if there are no (double) cast
		// normalize
		float x = (float)(((double)character.posCurr.x - posCurr.x) / mag);
		float y = (float)(((double)character.posCurr.y - posCurr.y) / mag);

		float angle = (float)acos(x);

		//bullet velocity
		//float x or y * 10(velocity)
		AEVec2 vel = { x * 5, y * 5 };
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
	}
}

void Enemy::facePlayer(Player character)
{
	if (posCurr.x > character.posCurr.x)
	{
		dirFaceR = false;
	}
	else
	{
		dirFaceR = true;
	}
}





//Player functions
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
	/*powerRange = 5;
	powerDamage = 10;
	powerSpeed = 5;*/
	projectileMax = 1;
	//maxHealth = currentHealth = 100;
	counter = invincibleTimer = 0.25f;
}

/******************************************************************************/
/*!
	Player creates boomerang
*/
/******************************************************************************/
void Player::playerFire(Projectile* boomerang)
{
	AEVec2 vel;
	AEVec2Zero(&vel);

	//### need to tweak
	//initial speed of boomerang
	float newSpeed = baseSpeed * (1 - 1 / (1 + 0.3f * powerSpeed));
	float tmp = 0.5f * newSpeed;
	vel.x = (dirFaceR) ? 10.0f + tmp : -10.0f - tmp;

	float newRange = baseRange * (1 - 1 / (1 + 0.3f * powerRange));

	//find empty slot in the projectile array
	for (size_t i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		if (boomerang[i].flag == 0)
		{
			boomerang[i].boomerangCreate(&posCurr, &vel, newRange);
			break;
		}
	}
}

/******************************************************************************/
/*!
	Player grid collision flag update
*/
/******************************************************************************/
void Player::playerGridFlag()
{
	if (gridCollisionFlag & COLLISION_BOTTOM)
	{
		SnapToCell(&posCurr.y);
		velCurr.y = 0;
	}
	if (gridCollisionFlag & COLLISION_TOP)
	{
		SnapToCell(&posCurr.y);
		velCurr.y = 0;
	}
	if (gridCollisionFlag & COLLISION_LEFT)
	{
		SnapToCell(&posCurr.x);
		velCurr.x = 0;
	}
	if (gridCollisionFlag & COLLISION_RIGHT)
	{
		SnapToCell(&posCurr.x);
		velCurr.x = 0;
	}
}

/******************************************************************************/
/*!
	Display player health
*/
/******************************************************************************/
void Player::healthDisplay(float camX, float camY)
{
	//Position of health base based off the camera position
	float X = camX;
	float Y = camY - 275.0f;
	float barHeight = 25.0f;
	float barWidth = 400.0f;

	char strBuffer[100];
	memset(strBuffer, 0, 100 * sizeof(char));
	sprintf_s(strBuffer, "%4d  /%4d", currentHealth, maxHealth);

	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTextureMode(AE_GFX_TM_PRECISE);
	AEGfxSetTransparency(1.0f);

	AEMtx33 scale1, trans, result;

	for (int i = 0; i < GAME_OBJ_NUM_MAX; i++)
	{
		//current health bar
		if (sGameObjList[i].type == TYPE_OBJECT_CURRHP)
		{
			//change position
			X = camX - ((maxHealth - currentHealth) / 2.0f / maxHealth) * (AEGetWindowWidth() / 2);
			//health bar size
			AEMtx33Scale(&scale1, (barWidth * ((float)currentHealth / maxHealth)), barHeight);
			AEMtx33Trans(&trans, X, Y);
			AEMtx33Concat(&result, &trans, &scale1);
			AEGfxSetTransform(result.m);
			AEGfxTextureSet(sGameObjList[i].pTex, 0, 0);
			AEGfxMeshDraw(sGameObjList[i].pMesh, AE_GFX_MDM_TRIANGLES);
		}
		//max health bar
		if (sGameObjList[i].type == TYPE_OBJECT_MAXHP)
		{
			X = camX;
			//health bar size
			AEMtx33Scale(&scale1, barWidth, barHeight);
			AEMtx33Trans(&trans, X, Y);
			AEMtx33Concat(&result, &trans, &scale1);
			AEGfxSetTransform(result.m);
			AEGfxTextureSet(sGameObjList[i].pTex, 0, 0);
			AEGfxMeshDraw(sGameObjList[i].pMesh, AE_GFX_MDM_TRIANGLES);
		}
	}
	AEGfxPrint(fontID, strBuffer, -0.1f, -275.0f / (AEGetWindowHeight() / 2) - 0.025f, 1.0f, 1.0f, 1.0f, 1.0f);
}

void Player::resetPower()
{
	maxHealth = currentHealth = 100;
	baseRange = 20;
	baseDamage = 10;
	baseSpeed = 20;
	powerRange = 1;
	powerDamage = 10;
	powerSpeed = 1;
	vampirism = 0;
	regeneration = 0;
}

void Player::RangeUp()
{
	powerRange += 1;
}

void Player::DamageUp()
{
	powerDamage += 10;
}

void Player::SpeedUp()
{
	powerSpeed += 1;
}

void Player::HpUp()
{
	int amt = 10;
	currentHealth += amt;
	maxHealth += amt;
}

void Player::VampUp()
{
	vampirism += 1;
}

void Player::RegenUp()
{
	regeneration += 5;
}
//### need a counter for what upgrades player got?





//Projectile functions
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
	initialPos = *startPosition;
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





void enemyspawning(Player player, Enemy* enemies)
{
	bool x_negative = rand() % 2;
	bool y_negative = rand() % 2;

	int x = rand() % 7;
	int y = rand() % 5;

	x += 3;
	y += 3;
	if (x_negative)
	{
		x *= -1;
	}
	if (y_negative)
	{
		y *= -1;
	}

	int type = (rand() % 3) + 4;

	AEVec2 pos;

	pos.x = player.posCurr.x + x;
	pos.y = player.posCurr.y + y;

	while (0 != GetCellValue((int)pos.x, (int)pos.y))
	{
		x_negative = rand() % 2;
		y_negative = rand() % 2;

		x = rand() % 7;
		y = rand() % 5;

		x += 3;
		y += 3;

		if (x_negative)
		{
			x *= -1;
		}
		if (y_negative)
		{
			y *= -1;
		}

		pos.x = player.posCurr.x + x;
		pos.y = player.posCurr.y + y;
	}

	for (size_t i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		if (enemies[i].flag == 0)
		{
			enemies[i].enemyCreate(type, &pos);
			//printf("Spawned %d\n", enemies[i].pObject->type);
			break;
		}
	}
}