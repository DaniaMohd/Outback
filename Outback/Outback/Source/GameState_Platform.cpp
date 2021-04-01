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

// list of original objects
GameObj* sGameObjList;
unsigned long			sGameObjNum;

// list of enemies
static Enemy*			sEnemies;

// list of coins
static GameObjInst*		sBlocks;

// list of particles
static GameObjInst*		sParticles;

// list of enemies
static Projectile*		sProjectiles;
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
//YuXi
bool					win;
static GameObjInst		sGoal;
static int				totalGoals = 3;
bool					newGame;
extern bool				endless;

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

		sBlocks = (GameObjInst*)calloc(GAME_OBJ_INST_NUM_MAX, sizeof(GameObjInst));

		sParticles = (GameObjInst*)calloc(GAME_OBJ_INST_NUM_MAX, sizeof(GameObjInst));

		sProjectiles = (Projectile*)calloc(GAME_OBJ_INST_NUM_MAX, sizeof(Projectile));
	}

	GameObj* pObj;

	//0
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

	//1
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

	//2
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

	//3
	//Goal
	{
		pObj = sGameObjList + sGameObjNum++;
		pObj->type = TYPE_OBJECT_GOAL;


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
		AE_ASSERT_MESG(pObj->pMesh, "Failed to create range Goal Mesh!");
		pObj->pTex = AEGfxTextureLoad("..\\Resources\\Textures\\Hero.png");
		AE_ASSERT_MESG(pObj->pTex, "Failed to create range texture!!");
	}

	//4
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

	//5
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

	//6
	//Creating the Ladder object
	{
		pObj = sGameObjList + sGameObjNum++;
		pObj->type = TYPE_OBJECT_LADDER;

		AEGfxMeshStart();

		AEGfxTriAdd(
			-0.5f, -0.5f, 0xFFFFFF00, 0.0f, 1.0f,
			0.5f, -0.5f, 0xFFFFFF00, 1.0f, 1.0f,
			-0.5f, 0.5f, 0xFFFFFF00, 0.0f, 0.0f);
		AEGfxTriAdd(
			0.5f, -0.5f, 0xFFFFFF00, 1.0f, 1.0f,
			0.5f, 0.5f, 0xFFFFFF00, 1.0f, 0.0f,
			-0.5f, 0.5f, 0xFFFFFF00, 0.0f, 0.0f);

		pObj->pMesh = AEGfxMeshEnd();
		AE_ASSERT_MESG(pObj->pMesh, "Failed to create object!!");
		pObj->pTex = AEGfxTextureLoad("..\\Resources\\Textures\\Ladder.png");
		AE_ASSERT_MESG(pObj->pTex, "Failed to create Ladder!");
	}
	
	//7
	//Creating the Particle object
	{
		pObj = sGameObjList + sGameObjNum++;
		pObj->type = TYPE_OBJECT_PARTICLES;

		AEGfxMeshStart();

		AEGfxTriAdd(
			-0.5f, -0.5f, 0xFFFFFF00, 0.0f, 1.0f,
			0.5f, -0.5f, 0xFFFFFF00, 1.0f, 1.0f,
			-0.5f, 0.5f, 0xFFFFFF00, 0.0f, 0.0f);
		AEGfxTriAdd(
			0.5f, -0.5f, 0xFFFFFF00, 1.0f, 1.0f,
			0.5f, 0.5f, 0xFFFFFF00, 1.0f, 0.0f,
			-0.5f, 0.5f, 0xFFFFFF00, 0.0f, 0.0f);

		pObj->pMesh = AEGfxMeshEnd();
		AE_ASSERT_MESG(pObj->pMesh, "Failed to create object!!");
		pObj->pTex = AEGfxTextureLoad("..\\Resources\\Textures\\Particle.png");
		AE_ASSERT_MESG(pObj->pTex, "Failed to create particles!");
	}

	//8
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

	//9
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
	
	//10
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

	//11
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

	//12
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

	//13
	//max health
	{
		pObj = sGameObjList + sGameObjNum++;
		pObj->type = TYPE_OBJECT_MAXHP;


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

	//14
	//current health
	{
		pObj = sGameObjList + sGameObjNum++;
		pObj->type = TYPE_OBJECT_CURRHP;


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

	int blkCount = 0;
	int enemyCount = 0;
	sBoomNum = 0;
	win = false;

	//Empty and Collidable blocks
	{
		pBlackInstance.gameObjInstCreate(TYPE_OBJECT_EMPTY, 1.0f, 0, 0, 0.0f);
		pBlackInstance.flag ^= FLAG_VISIBLE;
		pBlackInstance.flag |= FLAG_NON_COLLIDABLE;

		pWhiteInstance.gameObjInstCreate(TYPE_OBJECT_COLLISION, 1.0f, 0, 0, 0.0f);
		pWhiteInstance.flag ^= FLAG_VISIBLE;
		pWhiteInstance.flag |= FLAG_NON_COLLIDABLE;
	}

	AEVec2 Pos;

	for (y = 0; y < BINARY_MAP_HEIGHT; ++y)
	{
		for (x = 0; x < BINARY_MAP_WIDTH; ++x)
		{
			if (MapData[y][x] == TYPE_OBJECT_EMPTY || MapData[y][x] == TYPE_OBJECT_COLLISION)
				continue;

			Pos.x = (float)(x);
			Pos.y = (float)(y);

			if (MapData[y][x] == TYPE_OBJECT_HERO)
			{
				pHero.playerCreate(&Pos);
				if (newGame == true)
				{
					pHero.resetPower();
					newGame = false;
				}

				SnapToCell(&pHero.posCurr.x);
				SnapToCell(&pHero.posCurr.y);
			}

			//else if (MapData[y][x] == TYPE_OBJECT_ENEMY1)
			//{
			//	sEnemies[enemyCount].enemyCreate(TYPE_OBJECT_ENEMY1, &Pos);

			//	SnapToCell(&sEnemies[enemyCount].posCurr.x);
			//	SnapToCell(&sEnemies[enemyCount].posCurr.y);
			//	enemyCount++;
			//}

			else if (MapData[y][x] == TYPE_OBJECT_COIN)
			{
				sBlocks[blkCount].gameObjInstCreate(TYPE_OBJECT_COIN, 1.0f, &Pos, nullptr, 0.0f);

				SnapToCell(&sBlocks[blkCount].posCurr.x);
				SnapToCell(&sBlocks[blkCount].posCurr.y);
				blkCount++;
			}
			else if (MapData[y][x] == TYPE_OBJECT_LADDER)
			{
				sBlocks[blkCount].gameObjInstCreate(TYPE_OBJECT_LADDER, 1.0f, &Pos, nullptr, 0.0f);

				SnapToCell(&sBlocks[blkCount].posCurr.x);
				SnapToCell(&sBlocks[blkCount].posCurr.y);
				blkCount++;
			}
			//Yu Xi
			else if (MapData[y][x] == TYPE_OBJECT_GOAL)
			{
				printf("A\n");
				sGoal.gameObjInstCreate(TYPE_OBJECT_GOAL, 1.0f, &Pos, nullptr, 0.0f);

				SnapToCell(&sGoal.posCurr.x);
				SnapToCell(&sGoal.posCurr.y);
			}
		}
	}
	//### create multiple spawns
	for (int a = 0; a < 10; a++)
	{
		enemyspawning(pHero, sEnemies);
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
			pHero.particleEffect(sParticles, P_TRAIL);
		}
	}
	// Right
	else if (AEInputCheckCurr(AEVK_D) || AEInputCheckCurr(AEVK_RIGHT))
	{
		pHero.velCurr.x = MOVE_VELOCITY_HERO;
		if (pHero.gridCollisionFlag == COLLISION_BOTTOM)
		{
			pHero.particleEffect(sParticles, P_TRAIL);
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
		enemyspawning(pHero, sEnemies);
	}
	
	//Next stage
	if (AEInputCheckReleased(AEVK_N))
	{
		/*win = true;
		if (win == true)
		{
			switch (gGameStateCurr)
			{
			case GS_LEVEL1:
				gGameStateNext = GS_LEVEL2;
				win = false;
				break;
			case GS_LEVEL2:
				gGameStateNext = GS_LEVEL3;
				win = false;
				break;
			case GS_LEVEL3:
				gGameStateNext = GS_WIN;
				win = false;
				break;
			default:
				break;
			}
		}*/
		if (endless == true)
		{
			endless = false;
			printf("not endless\n");
		}
		else if (endless == false)
		{
			endless = true;
			printf("endless\n");
		}
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

	//Yu Xi goal update
	sGoal.gameObjInstBoundingBox();

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

	//Block update
	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
	{
		if (0 == (sBlocks[i].flag & FLAG_ACTIVE))
			continue;

		sBlocks[i].gameObjInstBoundingBox();
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
		sEnemies[i].enemyGridFlag();
	}

	//Player grid collision
	pHero.gridCollisionFlag = CheckInstanceBinaryMapCollision(pHero.posCurr.x, pHero.posCurr.y, pHero.scale, pHero.scale);
	pHero.playerGridFlag();

	//play invicibility
	pHero.counter += g_dt;
	pHero.counter = (pHero.counter >= pHero.invincibleTimer) ? pHero.invincibleTimer : pHero.counter;

	//Check if player touches goal
	if((CollisionIntersection_RectRect(pHero.boundingBox, pHero.velCurr, sGoal.boundingBox, sGoal.velCurr)) == true)
	{
		switch (gGameStateCurr)
		{
		case GS_LEVEL1:
			gGameStateNext = GS_LEVEL2;
			win = false;
			break;
		case GS_LEVEL2:
			gGameStateNext = GS_LEVEL3;
			win = false;
			break;
		case GS_LEVEL3:
			if (endless == false)
			{
				gGameStateNext = GS_WIN;
				win = false;
			}
			else
			{
				gGameStateNext = GS_LEVEL1;
				win = false;
			}
			break;
		default:
			break;
		}
	}

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
		if (0 == (sBlocks[i].flag & FLAG_ACTIVE))
			continue;

		//checks if player touches power up
		if ((CollisionIntersection_RectRect(pHero.boundingBox, pHero.velCurr, sBlocks[i].boundingBox, sBlocks[i].velCurr)) == true 
			&& sBlocks[i].pObject->type != TYPE_OBJECT_COIN 
			&& sBlocks[i].pObject->type != TYPE_OBJECT_LADDER)
		{
			if (sBlocks[i].pObject->type == TYPE_OBJECT_DAMAGE)
			{
				pHero.DamageUp();
			}
			else if (sBlocks[i].pObject->type == TYPE_OBJECT_RANGE)
			{
				pHero.RangeUp();
			}
			else if (sBlocks[i].pObject->type == TYPE_OBJECT_SPEED)
			{
				pHero.SpeedUp();
			}
			sBlocks[i].gameObjInstDestroy();
		}

		//Ladder
		if ((CollisionIntersection_RectRect(pHero.boundingBox, pHero.velCurr, sBlocks[i].boundingBox, sBlocks[i].velCurr)) == true
			&& sBlocks[i].pObject->type == TYPE_OBJECT_LADDER)
		{
			if (AEInputCheckCurr(AEVK_W))
			{
				pHero.velCurr.y = 5.0f;
			}
		}

		//checks if boomerang hits coin
		for (j = 0; j < GAME_OBJ_INST_NUM_MAX; ++j)
		{
			if ((CollisionIntersection_RectRect(sProjectiles[j].boundingBox, sProjectiles[j].velCurr, sBlocks[i].boundingBox, sBlocks[i].velCurr)) == true)
			{
				if (sProjectiles[j].pObject->type == TYPE_OBJECT_BOOMERANG && sBlocks[i].pObject->type == TYPE_OBJECT_COIN)
				{
					sBlocks[i].PowerUpCreate(sBlocks[i].posCurr);
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
						sEnemies[j].particleEffect(sParticles, P_HIT);
						printf("enemy hp: %d\n", sEnemies[j].healthPoints);
					}
					if (sEnemies[j].hit2 == false && sProjectiles[i].boomerangReturning == true)
					{
						sEnemies[j].healthPoints -= pHero.powerDamage;
						sEnemies[j].hit2 = true;
						sEnemies[j].particleEffect(sParticles, P_HIT);
						printf("enemy hp: %d\n", sEnemies[j].healthPoints);
					}
					if (sEnemies[j].healthPoints <= 0)
					{
						sEnemies[j].gameObjInstDestroy();
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
			sProjectiles[i].gameObjInstDestroy();
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

	//Computing the transformation matrices of the game object instances
	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
	{
		//only active objects
		if (1 == (sEnemies[i].flag & FLAG_ACTIVE))
			sEnemies[i].gameObjInstTransformMatrix();

		if (1 == (sBlocks[i].flag & FLAG_ACTIVE))
			sBlocks[i].gameObjInstTransformMatrix();

		if (1 == (sParticles[i].flag & FLAG_ACTIVE))
			sParticles[i].gameObjInstTransformMatrix();

		if (1 == (sProjectiles[i].flag & FLAG_ACTIVE))
			sProjectiles[i].gameObjInstTransformMatrix();
	}
	pHero.gameObjInstTransformMatrix();

	//Yu Xi
	sGoal.gameObjInstTransformMatrix();

	//### death
	if (pHero.currentHealth <= 0)
	{
		gGameStateNext = GS_WIN;	//lose
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
		// Camera does not go out of bounds
		if (pHero.posCurr.x >= 10.0f && pHero.posCurr.x <= BINARY_MAP_WIDTH - 10.0f)
			camX = (float)(pHero.posCurr.x - BINARY_MAP_WIDTH / 2) * PIXEL;
		if (pHero.posCurr.y >= 7.5f && pHero.posCurr.y <= BINARY_MAP_HEIGHT - 7.5f)
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

	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		//only active objects
		if (1 == (sEnemies[i].flag & FLAG_ACTIVE) || 1 == (sEnemies[i].flag & FLAG_VISIBLE))
			sEnemies[i].gameObjInstDrawObject(&MapTransform);

		if (1 == (sBlocks[i].flag & FLAG_ACTIVE) || 1 == (sBlocks[i].flag & FLAG_VISIBLE))
			sBlocks[i].gameObjInstDrawObject(&MapTransform);

		if (1 == (sParticles[i].flag & FLAG_ACTIVE) || 1 == (sParticles[i].flag & FLAG_VISIBLE))
			sParticles[i].gameObjInstDrawObject(&MapTransform);

		if (1 == (sProjectiles[i].flag & FLAG_ACTIVE) || 1 == (sProjectiles[i].flag & FLAG_VISIBLE))
			sProjectiles[i].gameObjInstDrawObject(&MapTransform);
	}

	pHero.gameObjInstDrawObject(&MapTransform);

	//Yu Xi
	sGoal.gameObjInstDrawObject(&MapTransform);

	//draw health bar
	pHero.healthDisplay(camX, camY);

	//printing debug
	if (onChange == true)
	{
		//printf("DEBUGGING!\n");
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
		sBlocks[i].gameObjInstDestroy();
		sProjectiles[i].gameObjInstDestroy();
	}
	pWhiteInstance.gameObjInstDestroy();
	pBlackInstance.gameObjInstDestroy();
	pHero.gameObjInstDestroy();
	sGoal.gameObjInstDestroy();
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
	free(sBlocks);
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
