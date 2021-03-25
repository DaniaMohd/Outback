/******************************************************************************/
/*!
\file		GameState_Platform.cpp
\author
\par    	email:
\date   	??/??/2021
\brief      This file contains the definition for the Game State Platform function.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "main.h"

/******************************************************************************/
/*!
	File globals
*/
/******************************************************************************/
static int				HeroLives;
static int				Hero_Initial_X;
static int				Hero_Initial_Y;
static int				TotalCoins;
static int              CoinsCollected;

// list of original objects
GameObj* sGameObjList;
unsigned long			sGameObjNum;

// list of enemies
static Enemy* sEnemies;
static unsigned int		sEnemyNum;

// list of coins
static GameObjInst* sCoins;
static unsigned int		sCoinNum;

// list of particles
static GameObjInst* sParticles;

// list of enemies
static Projectile* sProjectiles;
static unsigned int		sBoomNum;

// Binary map data
static GameObjInst		pBlackInstance;
static GameObjInst		pWhiteInstance;
static AEMtx33			MapTransform;

// Hero instance
static Player			pHero;

const int				PIXEL = 40;

float                   camX = 0.0f; //camera on x-axis
float                   camY = 0.0f; //camera on y-axis

static bool             onChange = true; //when touching an enemy or coin

/******************************************************************************/
/*!
	"Load" Platform
*/
/******************************************************************************/
void GameStatePlatformLoad(void)
{
	//Allocating space
	{
		sGameObjList = (GameObj*)calloc(GAME_OBJ_NUM_MAX, sizeof(GameObj));
		sGameObjNum = 0;

		sEnemies = (Enemy*)calloc(GAME_OBJ_INST_NUM_MAX, sizeof(Enemy));
		sEnemyNum = 0;

		sCoins = (GameObjInst*)calloc(GAME_OBJ_INST_NUM_MAX, sizeof(GameObjInst));

		sParticles = (GameObjInst*)calloc(GAME_OBJ_INST_NUM_MAX, sizeof(GameObjInst));

		sProjectiles = (Projectile*)calloc(GAME_OBJ_INST_NUM_MAX, sizeof(Projectile));
	}

	GameObj* pObj;

	//Creating the black object
	{
		pObj = sGameObjList + sGameObjNum++;
		pObj->type = TYPE_OBJECT_EMPTY;


		AEGfxMeshStart();
		AEGfxTriAdd(
			-0.5f, -0.5f, 0xFF5FCDE4, 0.0f, 0.0f,
			0.5f, -0.5f, 0xFF5FCDE4, 0.0f, 0.0f,
			-0.5f, 0.5f, 0xFF5FCDE4, 0.0f, 0.0f);

		AEGfxTriAdd(
			0.5f, -0.5f, 0xFF5FCDE4, 0.0f, 0.0f,
			0.5f, 0.5f, 0xFF5FCDE4, 0.0f, 0.0f,
			-0.5f, 0.5f, 0xFF5FCDE4, 0.0f, 0.0f);
		pObj->pMesh = AEGfxMeshEnd();
		AE_ASSERT_MESG(pObj->pMesh, "fail to create object!!");
		pObj->pTex = AEGfxTextureLoad("..\\Resources\\Textures\\Background.png");
		AE_ASSERT_MESG(pObj->pTex, "Failed to create background Texture!");
	}

	//Creating the white object
	{
		pObj = sGameObjList + sGameObjNum++;
		pObj->type = TYPE_OBJECT_COLLISION;


		AEGfxMeshStart();
		AEGfxTriAdd(
			-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
			0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
			-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);
		AEGfxTriAdd(
			0.5, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
			0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
			-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);


		pObj->pMesh = AEGfxMeshEnd();
		AE_ASSERT_MESG(pObj->pMesh, "Failed to create Block Mesh!");
		pObj->pTex = AEGfxTextureLoad("..\\Resources\\Textures\\Block.png");
		AE_ASSERT_MESG(pObj->pTex, "Failed to create Block Texture!");
	}

	//Creating the hero object
	{
		pObj = sGameObjList + sGameObjNum++;
		pObj->type = TYPE_OBJECT_HERO;


		AEGfxMeshStart();
		AEGfxTriAdd(
			-0.5f, -0.5f, 0xFF0000FF, 0.0f, 1.0f,
			0.5f, -0.5f, 0xFF0000FF, 1.0f, 1.0f,
			-0.5f, 0.5f, 0xFF0000FF, 0.0f, 0.0f);
		AEGfxTriAdd(
			0.5, -0.5f, 0xFF0000FF, 1.0f, 1.0f,
			0.5f, 0.5f, 0xFF0000FF, 1.0f, 0.0f,
			-0.5f, 0.5f, 0xFF0000FF, 0.0f, 0.0f);

		pObj->pMesh = AEGfxMeshEnd();
		AE_ASSERT_MESG(pObj->pMesh, "Failed to create Hero Mesh!");
		pObj->pTex = AEGfxTextureLoad("..\\Resources\\Textures\\Hero.png");
		AE_ASSERT_MESG(pObj->pTex, "Failed to create Hero Texture!");
	}

	//Creating the enemey1 object
	{
		pObj = sGameObjList + sGameObjNum++;
		pObj->type = TYPE_OBJECT_ENEMY1;


		AEGfxMeshStart();
		AEGfxTriAdd(
			-0.5f, -0.5f, 0xFFFF0000, 0.0f, 1.0f,
			0.5f, -0.5f, 0xFFFF0000, 1.0f, 1.0f,
			-0.5f, 0.5f, 0xFFFF0000, 0.0f, 0.0f);
		AEGfxTriAdd(
			0.5, -0.5f, 0xFFFF0000, 1.0f, 1.0f,
			0.5f, 0.5f, 0xFFFF0000, 1.0f, 0.0f,
			-0.5f, 0.5f, 0xFFFF0000, 0.0f, 0.0f);

		pObj->pMesh = AEGfxMeshEnd();
		AE_ASSERT_MESG(pObj->pMesh, "Failed to create Enemy Mesh!");
		pObj->pTex = AEGfxTextureLoad("..\\Resources\\Textures\\Spider.png");
		AE_ASSERT_MESG(pObj->pTex, "Failed to create Enemy Texture!");
	}

	//Creating the Coin object
	{
		pObj = sGameObjList + sGameObjNum++;
		pObj->type = TYPE_OBJECT_COIN;


		AEGfxMeshStart();
		AEGfxTriAdd(
			-0.5f, -0.5f, 0xFFFFFF00, 0.0f, 1.0f,
			0.5f, -0.5f, 0xFFFFFF00, 1.0f, 1.0f,
			-0.5f, 0.5f, 0xFFFFFF00, 0.0f, 0.0f);
		AEGfxTriAdd(
			0.5, -0.5f, 0xFFFFFF00, 1.0f, 1.0f,
			0.5f, 0.5f, 0xFFFFFF00, 1.0f, 0.0f,
			-0.5f, 0.5f, 0xFFFFFF00, 0.0f, 0.0f);

		pObj->pMesh = AEGfxMeshEnd();
		AE_ASSERT_MESG(pObj->pMesh, "Failed to create Coin Mesh!");
		pObj->pTex = AEGfxTextureLoad("..\\Resources\\Textures\\Coin.png");
		AE_ASSERT_MESG(pObj->pTex, "Failed to create Coin Texture!");
	}

	//Creating the Particle object
	{
		pObj = sGameObjList + sGameObjNum++;
		pObj->type = TYPE_OBJECT_PARTICLES;

		AEGfxMeshStart();

		AEGfxTriAdd(
			-0.1f, -0.1f, 0xFFFFFF00, 0.0f, 1.0f,
			0.1f, -0.1f, 0xFFFFFF00, 1.0f, 1.0f,
			-0.1f, 0.1f, 0xFFFFFF00, 0.0f, 0.0f);
		AEGfxTriAdd(
			0.1f, -0.1f, 0xFFFFFF00, 1.0f, 1.0f,
			0.1f, 0.1f, 0xFFFFFF00, 1.0f, 0.0f,
			-0.1f, 0.1f, 0xFFFFFF00, 0.0f, 0.0f);

		pObj->pMesh = AEGfxMeshEnd();
		AE_ASSERT_MESG(pObj->pMesh, "Failed to create object!!");
		pObj->pTex = AEGfxTextureLoad("..\\Resources\\Textures\\Particle.png");
		AE_ASSERT_MESG(pObj->pTex, "Failed to create particles!");
	}

	//Creating the Bullets
	{
		pObj = sGameObjList + sGameObjNum++;
		pObj->type = TYPE_OBJECT_BULLET;

		AEGfxMeshStart();
		AEGfxTriAdd(
			-0.5f, -0.5f, 0xFFFFFF00, 0.0f, 1.0f,
			0.5f, -0.5f, 0xFFFFFF00, 1.0f, 1.0f,
			-0.5f, 0.5f, 0xFFFFFF00, 0.0f, 0.0f);
		AEGfxTriAdd(
			0.5, -0.5f, 0xFFFFFF00, 1.0f, 1.0f,
			0.5f, 0.5f, 0xFFFFFF00, 1.0f, 0.0f,
			-0.5f, 0.5f, 0xFFFFFF00, 0.0f, 0.0f);

		pObj->pMesh = AEGfxMeshEnd();
		AE_ASSERT_MESG(pObj->pMesh, "fail to create object!!");
		pObj->pTex = AEGfxTextureLoad("..\\Resources\\Textures\\arrow.png");
		AE_ASSERT_MESG(pObj->pTex, "Failed to create bullets!");
	}

	//Creating the Boomerang
	{
		pObj = sGameObjList + sGameObjNum++;
		pObj->type = TYPE_OBJECT_BOOMERANG;

		AEGfxMeshStart();
		AEGfxTriAdd(
			-0.5f, -0.5f, 0xFFFFFF00, 0.0f, 1.0f,
			0.5f, -0.5f, 0xFFFFFF00, 1.0f, 1.0f,
			-0.5f, 0.5f, 0xFFFFFF00, 0.0f, 0.0f);
		AEGfxTriAdd(
			0.5, -0.5f, 0xFFFFFF00, 1.0f, 1.0f,
			0.5f, 0.5f, 0xFFFFFF00, 1.0f, 0.0f,
			-0.5f, 0.5f, 0xFFFFFF00, 0.0f, 0.0f);

		pObj->pMesh = AEGfxMeshEnd();
		AE_ASSERT_MESG(pObj->pMesh, "fail to create object!!");
		pObj->pTex = AEGfxTextureLoad("..\\Resources\\Textures\\Boomerang.png");
		AE_ASSERT_MESG(pObj->pTex, "Failed to create boomerang!");
	}

	//Damage upgrade
	{
		pObj = sGameObjList + sGameObjNum++;
		pObj->type = TYPE_OBJECT_DAMAGE;


		AEGfxMeshStart();
		AEGfxTriAdd(
			-0.5f, -0.5f, 0xFFFFFF00, 0.0f, 1.0f,
			0.5f, -0.5f, 0xFFFFFF00, 1.0f, 1.0f,
			-0.5f, 0.5f, 0xFFFFFF00, 0.0f, 0.0f);
		AEGfxTriAdd(
			0.5, -0.5f, 0xFFFFFF00, 1.0f, 1.0f,
			0.5f, 0.5f, 0xFFFFFF00, 1.0f, 0.0f,
			-0.5f, 0.5f, 0xFFFFFF00, 0.0f, 0.0f);

		pObj->pMesh = AEGfxMeshEnd();
		AE_ASSERT_MESG(pObj->pMesh, "Failed to create Speed Mesh!");
		pObj->pTex = AEGfxTextureLoad("..\\Resources\\Textures\\DamageUp.png");
		AE_ASSERT_MESG(pObj->pTex, "Failed to create damage texture!!");
	}

	//Range upgrade
	{
		pObj = sGameObjList + sGameObjNum++;
		pObj->type = TYPE_OBJECT_RANGE;


		AEGfxMeshStart();
		AEGfxTriAdd(
			-0.5f, -0.5f, 0xFFFFFF00, 0.0f, 1.0f,
			0.5f, -0.5f, 0xFFFFFF00, 1.0f, 1.0f,
			-0.5f, 0.5f, 0xFFFFFF00, 0.0f, 0.0f);
		AEGfxTriAdd(
			0.5, -0.5f, 0xFFFFFF00, 1.0f, 1.0f,
			0.5f, 0.5f, 0xFFFFFF00, 1.0f, 0.0f,
			-0.5f, 0.5f, 0xFFFFFF00, 0.0f, 0.0f);

		pObj->pMesh = AEGfxMeshEnd();
		AE_ASSERT_MESG(pObj->pMesh, "Failed to create range Mesh!");
		pObj->pTex = AEGfxTextureLoad("..\\Resources\\Textures\\RangeUp.png");
		AE_ASSERT_MESG(pObj->pTex, "Failed to create range texture!!");
	}

	//Speed upgrade
	{
		pObj = sGameObjList + sGameObjNum++;
		pObj->type = TYPE_OBJECT_SPEED;


		AEGfxMeshStart();
		AEGfxTriAdd(
			-0.5f, -0.5f, 0xFFFFFF00, 0.0f, 1.0f,
			0.5f, -0.5f, 0xFFFFFF00, 1.0f, 1.0f,
			-0.5f, 0.5f, 0xFFFFFF00, 0.0f, 0.0f);
		AEGfxTriAdd(
			0.5, -0.5f, 0xFFFFFF00, 1.0f, 1.0f,
			0.5f, 0.5f, 0xFFFFFF00, 1.0f, 0.0f,
			-0.5f, 0.5f, 0xFFFFFF00, 0.0f, 0.0f);

		pObj->pMesh = AEGfxMeshEnd();
		AE_ASSERT_MESG(pObj->pMesh, "Failed to create range Mesh!");
		pObj->pTex = AEGfxTextureLoad("..\\Resources\\Textures\\SpeedUp.png");
		AE_ASSERT_MESG(pObj->pTex, "Failed to create speed texture!!");
	}

	//max health
	{
		pObj = sGameObjList + sGameObjNum++;
		pObj->type = 100;


		AEGfxMeshStart();
		AEGfxTriAdd(
			-0.5f, -0.5f, 0xFF000000, 0.0f, 1.0f,
			0.5f, -0.5f, 0xFF000000, 1.0f, 1.0f,
			-0.5f, 0.5f, 0xFF000000, 0.0f, 0.0f);
		AEGfxTriAdd(
			0.5, -0.5f, 0xFF000000, 1.0f, 1.0f,
			0.5f, 0.5f, 0xFF000000, 1.0f, 0.0f,
			-0.5f, 0.5f, 0xFF000000, 0.0f, 0.0f);

		pObj->pMesh = AEGfxMeshEnd();
		AE_ASSERT_MESG(pObj->pMesh, "Failed to create range Mesh!");
		pObj->pTex = AEGfxTextureLoad("..\\Resources\\Textures\\fullhealth.png");
		AE_ASSERT_MESG(pObj->pTex, "Failed to create damage texture!!");
	}

	//current health
	{
		pObj = sGameObjList + sGameObjNum++;
		pObj->type = 99;


		AEGfxMeshStart();
		AEGfxTriAdd(
			-0.5f, -0.5f, 0xFFFF0000, 0.0f, 1.0f,
			0.5f, -0.5f, 0xFFFF0000, 1.0f, 1.0f,
			-0.5f, 0.5f, 0xFFFF0000, 0.0f, 0.0f);
		AEGfxTriAdd(
			0.5, -0.5f, 0xFFFF0000, 1.0f, 1.0f,
			0.5f, 0.5f, 0xFFFF0000, 1.0f, 0.0f,
			-0.5f, 0.5f, 0xFFFF0000, 0.0f, 0.0f);

		pObj->pMesh = AEGfxMeshEnd();
		AE_ASSERT_MESG(pObj->pMesh, "Failed to create range Mesh!");
		pObj->pTex = AEGfxTextureLoad("..\\Resources\\Textures\\currenthealth.png");
		AE_ASSERT_MESG(pObj->pTex, "Failed to create damage texture!!");
	}

	// Loading the levels
	if (gGameStateCurr == GS_LEVEL1)
	{
		Level1BGMLoad();
		//Importing Data
		if (!ImportMapDataFromFile("..\\Resources\\Levels\\Exported.txt"))
			gGameStateNext = GS_QUIT;
	}
	else if (gGameStateCurr == GS_LEVEL2)
	{
		Level2BGMLoad();
		if (!ImportMapDataFromFile("..\\Resources\\Levels\\Exported1.txt"))
			gGameStateNext = GS_QUIT;
	}
	else if (gGameStateCurr == GS_LEVEL3)
	{
		Level3BGMLoad();
		if (!ImportMapDataFromFile("..\\Resources\\Levels\\Exported2.txt"))
			gGameStateNext = GS_QUIT;
	}

	AEMtx33 scale, trans;

	AEMtx33Trans(&trans, -(float)BINARY_MAP_WIDTH / 2, -(float)BINARY_MAP_HEIGHT / 2);

	// Whole level fits in the screen
	if (gGameStateCurr == GS_LEVEL1 || gGameStateCurr == GS_LEVEL2)
	{
		AEMtx33Scale(&scale, (float)(AEGetWindowWidth() / BINARY_MAP_WIDTH), (float)(AEGetWindowHeight() / BINARY_MAP_HEIGHT));
	}
	else
	{
		// Each block is set
		AEMtx33Scale(&scale, PIXEL, PIXEL);
	}

	AEMtx33Concat(&MapTransform, &scale, &trans);
}

/******************************************************************************/
/*!
	"Initialize" Platform
*/
/******************************************************************************/
void GameStatePlatformInit(void)
{
	int x, y;

	TotalCoins = 0;
	CoinsCollected = 0;
	sBoomNum = 0;

	//Empty and Collidable blocks
	{
		pBlackInstance.gameObjInstCreate(TYPE_OBJECT_EMPTY, 1.0f, 0, 0, 0.0f);
		pBlackInstance.flag ^= FLAG_VISIBLE;
		pBlackInstance.flag |= FLAG_NON_COLLIDABLE;

		pWhiteInstance.gameObjInstCreate(TYPE_OBJECT_COLLISION, 1.0f, 0, 0, 0.0f);
		pWhiteInstance.flag ^= FLAG_VISIBLE;
		pWhiteInstance.flag |= FLAG_NON_COLLIDABLE;
	}

	//Setting the inital number of hero lives
	HeroLives = HERO_LIVES;

	AEVec2 Pos;

	for (y = 0; y < BINARY_MAP_HEIGHT; ++y)
		for (x = 0; x < BINARY_MAP_WIDTH; ++x)
		{
			if (MapData[y][x] == TYPE_OBJECT_EMPTY || MapData[y][x] == TYPE_OBJECT_COLLISION)
				continue;

			Pos.x = (float)(x);
			Pos.y = (float)(y);

			if (MapData[y][x] == TYPE_OBJECT_HERO)
			{
				Hero_Initial_X = x;
				Hero_Initial_Y = y;

				pHero.playerCreate(&Pos);

				SnapToCell(&pHero.posCurr.x);
				SnapToCell(&pHero.posCurr.y);
			}

			else if (MapData[y][x] == TYPE_OBJECT_ENEMY1)
			{
				sEnemies[sEnemyNum].enemyCreate(TYPE_OBJECT_ENEMY1, &Pos);

				SnapToCell(&sEnemies[sEnemyNum].posCurr.x);
				SnapToCell(&sEnemies[sEnemyNum].posCurr.y);
				sEnemyNum++;
			}

			else if (MapData[y][x] == TYPE_OBJECT_COIN)
			{
				sCoins[TotalCoins].gameObjInstCreate(TYPE_OBJECT_COIN, 1.0f, &Pos, nullptr, 0.0f);

				SnapToCell(&sCoins[TotalCoins].posCurr.x);
				SnapToCell(&sCoins[TotalCoins].posCurr.y);
				TotalCoins++;
			}
		}
}

/******************************************************************************/
/*!
	"Update" Platform
*/
/******************************************************************************/
void GameStatePlatformUpdate(void)
{
	int i, j;

	// Left
	if (AEInputCheckCurr(AEVK_A) || AEInputCheckCurr(AEVK_LEFT))
	{
		pHero.velCurr.x = -MOVE_VELOCITY_HERO;
		if (pHero.gridCollisionFlag == COLLISION_BOTTOM)
		{
			//### should probs make this into a function
			for (unsigned int k = 0; k < GAME_OBJ_INST_NUM_MAX; k++)
			{
				if (sParticles[k].flag == 0)
				{
					AEVec2 pos = { pHero.posCurr.x + 0.5f, pHero.posCurr.y - 0.5f };
					AEVec2 vel = { AERandFloat() * 10 , AERandFloat() * 10 };
					sParticles[k].gameObjInstCreate(TYPE_OBJECT_PARTICLES, 1.0f, &pos, &vel, 0);
					sParticles[k].counter = 0.25;
					break;
				}
			}
		}
	}
	// Right
	else if (AEInputCheckCurr(AEVK_D) || AEInputCheckCurr(AEVK_RIGHT))
	{
		pHero.velCurr.x = MOVE_VELOCITY_HERO;
		if (pHero.gridCollisionFlag == COLLISION_BOTTOM)
		{
			//### should probs make this into a function
			for (unsigned int k = 0; k < GAME_OBJ_INST_NUM_MAX; k++)
			{
				if (sParticles[k].flag == 0)
				{
					AEVec2 pos = { pHero.posCurr.x - 0.5f, pHero.posCurr.y - 0.5f };
					AEVec2 vel = { AERandFloat() * -10 , AERandFloat() * 10 };
					sParticles[k].gameObjInstCreate(TYPE_OBJECT_PARTICLES, 1.0f, &pos, &vel, 0);
					sParticles[k].counter = 0.25;
					break;
				}
			}
		}
	}
	else
		pHero.velCurr.x = 0;

	// Jump
	if ((AEInputCheckTriggered(AEVK_SPACE) || AEInputCheckTriggered(AEVK_UP)) && pHero.gridCollisionFlag & COLLISION_BOTTOM)
	{

		pHero.velCurr.y = JUMP_VELOCITY;

		pHero.gridCollisionFlag = 0;
	}

	//enemy firing debug
	if (AEInputCheckTriggered(AEVK_K))
	{
		for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
		{
			// skip non-active object
			if (0 == (sEnemies[i].flag & FLAG_ACTIVE))
				continue;

			sEnemies[i].enemyFire(pHero, sProjectiles);
		}
	}

	// Main Menu
	if (AEInputCheckTriggered(AEVK_BACK))
	{
		gGameStateNext = GS_MAINMENU;
	}

	//firing
	if (AEInputCheckCurr(AEVK_J) && sBoomNum < pHero.projectileMax)
	{
		pHero.playerFire(sProjectiles);
		++sBoomNum;
	}

	//stats debug 
	if (AEInputCheckTriggered(AEVK_P))
	{
		printf("damage: %d\nrange: %d\nspeed: %d\n\n", pHero.powerDamage, pHero.powerRange, pHero.powerSpeed);
		printf("%f\t%f\n", camX, camY);
	}
	//debug
	if (AEInputCheckTriggered(AEVK_M))
	{
		pHero.SpeedUp();
	}

	//Enemy update
	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
	{
		if (0 == (sEnemies[i].flag & FLAG_ACTIVE))
			continue;

		//update
		sEnemies[i].velCurr.y = GRAVITY * g_dt + sEnemies[i].velCurr.y;
		sEnemies[i].EnemyStateMachine();
		//bounding box
		sEnemies[i].gameObjInstUpdatePos();
		sEnemies[i].gameObjInstBoundingBox();

		if (sEnemies[i].innerState == INNER_STATE::INNER_STATE_ON_ENTER)
		{
			sEnemies[i].enemyFire(pHero, sProjectiles);
		}
	}

	//player update
	pHero.velCurr.y = GRAVITY * g_dt + pHero.velCurr.y;
	pHero.gameObjInstUpdatePos();
	pHero.gameObjInstBoundingBox();

	//Particle update
	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
	{
		if (0 == (sParticles[i].flag & FLAG_ACTIVE))
			continue;

		sParticles[i].gameObjInstUpdatePos();
		sParticles[i].counter -= g_dt;
		sParticles[i].velCurr.y = GRAVITY * g_dt + sParticles[i].velCurr.y;
		if (sParticles[i].counter <= 0)
		{
			sParticles[i].gameObjInstDestroy();
		}
	}

	//Coin update
	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
	{
		if (0 == (sCoins[i].flag & FLAG_ACTIVE))
			continue;

		sCoins[i].gameObjInstBoundingBox();
	}

	//Projectile Update
	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
	{
		if (0 == (sProjectiles[i].flag & FLAG_ACTIVE))
			continue;

		sProjectiles[i].gameObjInstUpdatePos();
		sProjectiles[i].gameObjInstBoundingBox();
		if (sProjectiles[i].pObject->type == TYPE_OBJECT_BULLET)
		{
			sProjectiles[i].ProjectileUpdate();
		}
		else
			sProjectiles[i].boomerangUpdate(pHero);
	}

	//Enemy grid collision
	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
	{
		// skip non-active object
		if (0 == (sEnemies[i].flag & FLAG_ACTIVE))
			continue;

		sEnemies[i].gridCollisionFlag = CheckInstanceBinaryMapCollision(sEnemies[i].posCurr.x, sEnemies[i].posCurr.y, sEnemies[i].scale, sEnemies[i].scale);

		//### should change to a function, probs in update
		if (sEnemies[i].gridCollisionFlag & COLLISION_BOTTOM)
		{
			SnapToCell(&sEnemies[i].posCurr.y);
			sEnemies[i].velCurr.y = 0;
		}
		if (sEnemies[i].gridCollisionFlag & COLLISION_TOP)
		{
			SnapToCell(&sEnemies[i].posCurr.y);
			sEnemies[i].velCurr.y = 0;
		}
		if (sEnemies[i].gridCollisionFlag & COLLISION_LEFT)
		{
			SnapToCell(&sEnemies[i].posCurr.x);
			sEnemies[i].velCurr.x = 0;
		}
		if (sEnemies[i].gridCollisionFlag & COLLISION_RIGHT)
		{
			SnapToCell(&sEnemies[i].posCurr.x);
			sEnemies[i].velCurr.x = 0;
		}
	}

	//Player grid collision
	pHero.gridCollisionFlag = CheckInstanceBinaryMapCollision(pHero.posCurr.x, pHero.posCurr.y, pHero.scale, pHero.scale);

	//### should change to a function, probs in update
	if (pHero.gridCollisionFlag & COLLISION_BOTTOM)
	{
		SnapToCell(&pHero.posCurr.y);
		pHero.velCurr.y = 0;
	}
	if (pHero.gridCollisionFlag & COLLISION_TOP)
	{
		SnapToCell(&pHero.posCurr.y);
		pHero.velCurr.y = 0;
	}
	if (pHero.gridCollisionFlag & COLLISION_LEFT)
	{
		SnapToCell(&pHero.posCurr.x);
		pHero.velCurr.x = 0;
	}
	if (pHero.gridCollisionFlag & COLLISION_RIGHT)
	{
		SnapToCell(&pHero.posCurr.x);
		pHero.velCurr.x = 0;
	}

	//play invicibility
	pHero.counter += g_dt;
	pHero.counter = (pHero.counter >= pHero.invincibleTimer) ? pHero.invincibleTimer : pHero.counter;


	//check if player touches an enemy
	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
	{
		if (0 == (sEnemies[i].flag & FLAG_ACTIVE))
			continue;

		if ((CollisionIntersection_RectRect(pHero.boundingBox, pHero.velCurr, sEnemies[i].boundingBox, sEnemies[i].velCurr)) == true)
		{

			if (pHero.counter >= pHero.invincibleTimer)
			{
				onChange = true;
				pHero.currentHealth -= sEnemies[i].damage;
				pHero.counter = 0;
			}
		}
		//pHero.invincibleTimer = 0.0f;
	}

	//checks if player touches coin
	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
	{
		if (0 == (sCoins[i].flag & FLAG_ACTIVE))
			continue;

		//checks if player touches power up
		if ((CollisionIntersection_RectRect(pHero.boundingBox, pHero.velCurr, sCoins[i].boundingBox, sCoins[i].velCurr)) == true && sCoins[i].pObject->type != TYPE_OBJECT_COIN)
		{
			if (sCoins[i].pObject->type == TYPE_OBJECT_DAMAGE)
			{
				pHero.DamageUp();
			}
			else if (sCoins[i].pObject->type == TYPE_OBJECT_RANGE)
			{
				pHero.RangeUp();
			}
			else if (sCoins[i].pObject->type == TYPE_OBJECT_SPEED)
			{
				pHero.SpeedUp();
			}
			sCoins[i].gameObjInstDestroy();
		}
		//checks if boomerang hits coin
		for (j = 0; j < GAME_OBJ_INST_NUM_MAX; ++j)
		{
			if ((CollisionIntersection_RectRect(sProjectiles[j].boundingBox, sProjectiles[j].velCurr, sCoins[i].boundingBox, sCoins[i].velCurr)) == true)
			{
				if (sProjectiles[j].pObject->type == TYPE_OBJECT_BOOMERANG && sCoins[i].pObject->type == TYPE_OBJECT_COIN)
				{
					sCoins[i].PowerUpCreate(sCoins[i].posCurr);
				}
			}

		}
	}

	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
	{
		if (0 == (sProjectiles[i].flag & FLAG_ACTIVE))
			continue;

		//checks if boomernang hits enemy
		if (sProjectiles[i].pObject->type == TYPE_OBJECT_BOOMERANG)
		{
			for (j = 0; j < GAME_OBJ_INST_NUM_MAX; ++j)
			{
				if (0 == (sEnemies[j].flag & FLAG_ACTIVE))
					continue;

				if (CollisionIntersection_RectRect(sEnemies[j].boundingBox, sEnemies[j].velCurr, sProjectiles[i].boundingBox, sProjectiles[i].velCurr) == true)
				{
					if (sEnemies[j].hit1 == false && sProjectiles[i].boomerangReturning == false)
					{
						sEnemies[j].healthPoints -= pHero.powerDamage;
						sEnemies[j].hit1 = true;
						printf("enemy hp: %d\n", sEnemies[j].healthPoints);
					}
					if (sEnemies[j].hit2 == false && sProjectiles[i].boomerangReturning == true)
					{
						sEnemies[j].healthPoints -= pHero.powerDamage;
						sEnemies[j].hit2 = true;
						printf("enemy hp: %d\n", sEnemies[j].healthPoints);
					}
					if (sEnemies[j].healthPoints <= 0)
					{
						sEnemies[j].gameObjInstDestroy();
						sEnemyNum--;
						printf("enemy ded\n");
					}
				}
				else if (sEnemies[j].hit1 == true)
				{
					sEnemies[j].hit1 = false;
				}
				else if (sEnemies[j].hit2 == true)
				{
					sEnemies[j].hit2 = false;
				}

			}
		}

		//### checks if enemy fire hits player
		if ((CollisionIntersection_RectRect(pHero.boundingBox, pHero.velCurr, sProjectiles[i].boundingBox, sProjectiles[i].velCurr)) == true
			&& sProjectiles[i].pObject->type == TYPE_OBJECT_BULLET)
		{
			if (pHero.counter >= pHero.invincibleTimer)
			{
				onChange = true;
				pHero.currentHealth -= 1;
				pHero.counter = 0;
			}
		}

		//boomerang returning to player
		if ((CollisionIntersection_RectRect(pHero.boundingBox, pHero.velCurr, sProjectiles[i].boundingBox, sProjectiles[i].velCurr)) == true
			&& sProjectiles[i].pObject->type == TYPE_OBJECT_BOOMERANG && sProjectiles[i].boomerangReturning == true)
		{
			--sBoomNum;
			sProjectiles[i].gameObjInstDestroy();
		}
	}

	//### combine all this into 1 loop
	//Computing the transformation matrices of the game object instances
	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
	{
		// skip non-active object
		if (0 == (sEnemies[i].flag & FLAG_ACTIVE))
			continue;

		sEnemies[i].gameObjInstTransformMatrix();
	}
	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
	{
		// skip non-active object
		if (0 == (sCoins[i].flag & FLAG_ACTIVE))
			continue;

		sCoins[i].gameObjInstTransformMatrix();
	}
	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
	{
		if (0 == (sParticles[i].flag & FLAG_ACTIVE))
			continue;

		sParticles[i].gameObjInstTransformMatrix();
	}
	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
	{
		if (0 == (sProjectiles[i].flag & FLAG_ACTIVE))
			continue;

		sProjectiles[i].gameObjInstTransformMatrix();
	}
	pHero.gameObjInstTransformMatrix();

	//### death
	if (pHero.currentHealth <= 0)
	{
		HeroLives--;
		pHero.currentHealth = pHero.maxHealth;
		if (HeroLives > 0)
		{
			AEVec2Set(&pHero.posCurr, (float)Hero_Initial_X + 0.5f, (float)Hero_Initial_Y + 0.5f);
		}
	}
	else if (HeroLives == 0)
	{
		printf("Try Again!\n");
		gGameStateNext = GS_RESTART;

		HeroLives = 3;
	}
	//### win
	if (sEnemyNum <= 0)
	{
		printf("Win!\n");
		gGameStateNext = GS_MAINMENU;
	}

	//camera settings
	if (gGameStateCurr == GS_LEVEL1 || gGameStateCurr == GS_LEVEL2)
	{
		// Camera fixed
		camX = BINARY_MAP_WIDTH / 2 - 10.0f;
		camY = BINARY_MAP_HEIGHT / 2 - 10.0f;
	}
	else
	{
		//### add camera stopping when at the walls
		// Camera tracking
		camX = (float)(pHero.posCurr.x - BINARY_MAP_WIDTH / 2) * PIXEL;
		camY = (float)(pHero.posCurr.y - BINARY_MAP_HEIGHT / 2) * PIXEL;
	}
	AEGfxSetCamPosition(camX, camY);
}

/******************************************************************************/
/*!
	"Draw" Platform
*/
/******************************************************************************/
void GameStatePlatformDraw(void)
{
	//Drawing the tile map (the grid)
	float x, y;
	int i, j;
	AEMtx33 cellTranslation, cellFinalTransformation;

	//AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxTextureSet(NULL, 0, 0);
	for (i = 0; i < BINARY_MAP_HEIGHT; ++i)
		for (j = 0; j < BINARY_MAP_WIDTH; ++j)
		{
			x = (float)j;
			y = (float)i;

			SnapToCell(&x);
			SnapToCell(&y);

			AEGfxSetBlendMode(AE_GFX_BM_BLEND);
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxSetTextureMode(AE_GFX_TM_PRECISE);

			AEMtx33Trans(&cellTranslation, x, y);
			AEMtx33Concat(&cellFinalTransformation, &MapTransform, &cellTranslation);
			AEGfxSetTransform(cellFinalTransformation.m);

			if (BinaryCollisionArray[i][j] == TYPE_OBJECT_EMPTY)
			{
				AEGfxTextureSet(pBlackInstance.pObject->pTex, 0, 0);

				AEGfxSetTransparency(1.0f);

				AEGfxMeshDraw(pBlackInstance.pObject->pMesh, AE_GFX_MDM_TRIANGLES);
			}

			if (BinaryCollisionArray[i][j] == TYPE_OBJECT_COLLISION)
			{
				AEGfxTextureSet(pWhiteInstance.pObject->pTex, 0, 0);

				AEGfxSetTransparency(1.0f);

				AEGfxMeshDraw(pWhiteInstance.pObject->pMesh, AE_GFX_MDM_TRIANGLES);
			}
		}

	//### combine all this into 1 loop
	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		// skip non-active object
		if (0 == (sEnemies[i].flag & FLAG_ACTIVE) || 0 == (sEnemies[i].flag & FLAG_VISIBLE))
			continue;

		sEnemies[i].gameObjInstDrawObject(&MapTransform);
	}
	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		// skip non-active object
		if (0 == (sCoins[i].flag & FLAG_ACTIVE) || 0 == (sCoins[i].flag & FLAG_VISIBLE))
			continue;

		sCoins[i].gameObjInstDrawObject(&MapTransform);
	}
	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		// skip non-active object
		if (0 == (sParticles[i].flag & FLAG_ACTIVE) || 0 == (sParticles[i].flag & FLAG_VISIBLE))
			continue;

		sParticles[i].gameObjInstDrawObject(&MapTransform);
	}
	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		// skip non-active object
		if (0 == (sProjectiles[i].flag & FLAG_ACTIVE) || 0 == (sProjectiles[i].flag & FLAG_VISIBLE))
			continue;

		sProjectiles[i].gameObjInstDrawObject(&MapTransform);
	}
	pHero.gameObjInstDrawObject(&MapTransform);

	//draw health bar
	pHero.healthDisplay(camX, camY);

	//printing debug
	if (onChange == true)
	{
		char strBuffer[100];
		memset(strBuffer, 0, 100 * sizeof(char));
		sprintf_s(strBuffer, "Lives Left: %i", HeroLives);
		printf("%s \n", strBuffer);
		sprintf_s(strBuffer, "Coins Collected: %i", CoinsCollected);
		printf("%s \n", strBuffer);
		if (CoinsCollected == TotalCoins)
		{
			printf("YOU WON!!!\n");
		}
		if (CoinsCollected % 15 == 0 && CoinsCollected < 35 && CoinsCollected != 0)
		{
			printf("JUMP HEIGHT INCREASED!\n");
		}
		onChange = false;
	}

}

/******************************************************************************/
/*!
	"Free" Platform
*/
/******************************************************************************/
void GameStatePlatformFree(void)
{
	// kill all object in the list
	for (unsigned int i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		sEnemies[i].gameObjInstDestroy();
		sCoins[i].gameObjInstDestroy();
		sProjectiles[i].gameObjInstDestroy();
	}
	pWhiteInstance.gameObjInstDestroy();
	pBlackInstance.gameObjInstDestroy();
	pHero.gameObjInstDestroy();
}

/******************************************************************************/
/*!
	"Unload" Platform
*/
/******************************************************************************/
void GameStatePlatformUnload(void)
{
	// free all CREATED mesh
	for (u32 i = 0; i < sGameObjNum; i++)
	{
		AEGfxMeshFree(sGameObjList[i].pMesh);

		if (sGameObjList[i].pTex)
			AEGfxTextureUnload(sGameObjList[i].pTex);
	}

	free(sEnemies);
	free(sCoins);
	free(sGameObjList);
	free(sParticles);
	free(sProjectiles);

	/*********
	Free the map data
	*********/
	FreeMapData();

	Level1BGMUnload();
	Level2BGMUnload();
	Level3BGMUnload();
}
