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
static int				enemyCount;
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

float pixelWidth;
float pixelHeight;

float                   camX = 0.0f; //camera on x-axis
float                   camY = 0.0f; //camera on y-axis

int						level;
static GameObjInst		sGoal;
int						sGoalNum;
int						spawnGoal;
int						totalGoals;
bool					newGame;
extern bool				endless;
int						ranSpawn;
int						currSpawn;
int						totalSpawn;
bool					gameIsPaused;
AEGfxVertexList*		pauseMesh;
AEGfxTexture*			pauseTex;
char					pause[100], conti[100];
extern					s8 fontID;
float EnemySpawnTime;
float enemySpawnRate = 5;
float goalTimer;
float startingTime;
bool timerStart;

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

	//MESHES
	{
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
			pObj->pTex = AEGfxTextureLoad("..\\Resources\\Textures\\AussieCowboy.png");
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
			pObj->pTex = AEGfxTextureLoad("..\\Resources\\Textures\\Car.png");
			AE_ASSERT_MESG(pObj->pTex, "Failed to create range texture!!");
		}

		//4
		//Creating the enemey1 object spider
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
		//Creating the enemey2 object bee
		{
			pObj = sGameObjList + sGameObjNum++;
			pObj->type = TYPE_OBJECT_ENEMY2;


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
			pObj->pTex = AEGfxTextureLoad("..\\Resources\\Textures\\Bee.png");
			AE_ASSERT_MESG(pObj->pTex, "Failed to create Enemy Texture!");
		}

		//6
		//Creating the enemey3 object cactus
		{
			pObj = sGameObjList + sGameObjNum++;
			pObj->type = TYPE_OBJECT_ENEMY3;


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
			pObj->pTex = AEGfxTextureLoad("..\\Resources\\Textures\\Caccy.png");
			AE_ASSERT_MESG(pObj->pTex, "Failed to create Enemy Texture!");
		}

		//7
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
			pObj->pTex = AEGfxTextureLoad("..\\Resources\\Textures\\Chest.png");
			AE_ASSERT_MESG(pObj->pTex, "Failed to create Coin Texture!");
		}

		//8
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

		//9
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

		//10
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

		//11
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

		//12
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

		//13
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
			pObj->pTex = AEGfxTextureLoad("..\\Resources\\Textures\\Damage.png");
			AE_ASSERT_MESG(pObj->pTex, "Failed to create damage texture!!");
		}

		//14
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
			pObj->pTex = AEGfxTextureLoad("..\\Resources\\Textures\\Range.png");
			AE_ASSERT_MESG(pObj->pTex, "Failed to create range texture!!");
		}

		//15
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
			pObj->pTex = AEGfxTextureLoad("..\\Resources\\Textures\\Speed1.png");
			AE_ASSERT_MESG(pObj->pTex, "Failed to create speed texture!!");
		}

		//16
		//HP up
		{
			pObj = sGameObjList + sGameObjNum++;
			pObj->type = TYPE_OBJECT_HPUP;


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
			pObj->pTex = AEGfxTextureLoad("..\\Resources\\Textures\\Heart.png");
			AE_ASSERT_MESG(pObj->pTex, "Failed to create damage texture!!");
		}

		//17
		//HP up
		{
			pObj = sGameObjList + sGameObjNum++;
			pObj->type = TYPE_OBJECT_VAMP;


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
			pObj->pTex = AEGfxTextureLoad("..\\Resources\\Textures\\Vamp.png");
			AE_ASSERT_MESG(pObj->pTex, "Failed to create damage texture!!");
		}

		//18
		//HP up
		{
			pObj = sGameObjList + sGameObjNum++;
			pObj->type = TYPE_OBJECT_REGEN;


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
			pObj->pTex = AEGfxTextureLoad("..\\Resources\\Textures\\Regen.png");
			AE_ASSERT_MESG(pObj->pTex, "Failed to create damage texture!!");
		}

		//19
		//Creating the Particle object
		{
			pObj = sGameObjList + sGameObjNum++;
			pObj->type = TYPE_OBJECT_PARTICLES_HIT;

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

		//20
		//Creating the Particle object
		{
			pObj = sGameObjList + sGameObjNum++;
			pObj->type = TYPE_OBJECT_PARTICLES_REGEN;

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
			pObj->pTex = AEGfxTextureLoad("..\\Resources\\Textures\\regen_1.png");
			AE_ASSERT_MESG(pObj->pTex, "Failed to create particles!");
		}

		//21
		//Creating the Particle object
		{
			pObj = sGameObjList + sGameObjNum++;
			pObj->type = TYPE_OBJECT_PARTICLES_HEAL;

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
			pObj->pTex = AEGfxTextureLoad("..\\Resources\\Textures\\Health.png");
			AE_ASSERT_MESG(pObj->pTex, "Failed to create particles!");
		}

		//PAUSE MESH
		{
			AEGfxMeshStart();
			AEGfxTriAdd(
				-300.0f, -200.0f, 0x00FFFFFF, 0.0f, 1.0f,
				300.0f, -200.0f, 0x00FFFFFF, 1.0f, 1.0f,
				-300.0f, 200.0f, 0x0000FFFF, 0.0f, 0.0f);
			AEGfxTriAdd(
				300.0f, -200.0f, 0x00FFFFFF, 1.0f, 1.0f,
				300.0f, 200.0f, 0x00FFFFFF, 1.0f, 0.0f,
				-300.0f, 200.0f, 0x00FFFFFF, 0.0f, 0.0f);

			pauseMesh = AEGfxMeshEnd();
			AE_ASSERT_MESG(pauseMesh, "Failed to create range Mesh!");
			pauseTex = AEGfxTextureLoad("..\\Resources\\Textures\\Pause.png");
			AE_ASSERT_MESG(pauseTex, "Failed to create pause text!!");
			memset(pause, 0, 100 * sizeof(char));
			sprintf_s(pause, "PAUSED");
			memset(conti, 0, 100 * sizeof(char));
			sprintf_s(conti, "Press ESC to continue, Press BACKSPACE to go Menu");
		}
	}

	//LOAD LEVELS
	if (gGameStateCurr == GS_TUTORIAL)
	{
		Level1BGMLoad();
		if (!ImportMapDataFromFile("..\\Resources\\Levels\\Tutorial.txt"))
			gGameStateNext = GS_QUIT;
		goalTimer = 30.0f;
		startingTime = 30.0f;
	}
	if (gGameStateCurr == GS_LEVEL1)
	{
		Level1BGMLoad();
		if (!ImportMapDataFromFile("..\\Resources\\Levels\\Exported.txt"))
			gGameStateNext = GS_QUIT;
		goalTimer = 60.0f;
		startingTime = 60.0f;
	}
	else if (gGameStateCurr == GS_LEVEL2)
	{
		Level2BGMLoad();
		if (!ImportMapDataFromFile("..\\Resources\\Levels\\Exported1.txt"))
			gGameStateNext = GS_QUIT;
		goalTimer = 60.0f;
		startingTime = 60.0f;
	}
	else if (gGameStateCurr == GS_LEVEL3)
	{
		Level3BGMLoad();
		if (!ImportMapDataFromFile("..\\Resources\\Levels\\Exported2.txt"))
			gGameStateNext = GS_QUIT;
		goalTimer = 60.0f;
		startingTime = 60.0f;
	}
	timerStart = false;

	//PIXEL SIZE
	AEMtx33 scale, trans;

	AEMtx33Trans(&trans, -(float)BINARY_MAP_WIDTH / 2, -(float)BINARY_MAP_HEIGHT / 2);

	// Whole level fits in the screen
	if (gGameStateCurr == GS_TUTORIAL)
	{
		AEMtx33Scale(&scale, (float)(AEGetWindowWidth() / BINARY_MAP_WIDTH), (float)(AEGetWindowHeight() / BINARY_MAP_HEIGHT));
	}
	//### take out after all 3 levels are done
	if (gGameStateCurr == GS_LEVEL1 || gGameStateCurr == GS_LEVEL2)
	{
		AEMtx33Scale(&scale, (float)(AEGetWindowWidth() / BINARY_MAP_WIDTH), (float)(AEGetWindowHeight() / BINARY_MAP_HEIGHT));
	}
	else
	{
		// Each block is set
		pixelWidth = AEGetWindowWidth() / 20;
		pixelHeight = AEGetWindowHeight() / 15;
		AEMtx33Scale(&scale, pixelWidth, pixelHeight);
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
	sGoalNum = 0;
	currSpawn = 0;
	totalSpawn = 0;
	totalGoals = 0;
	enemyCount = 0;
	++level;
	for (int y = 0; y < BINARY_MAP_HEIGHT; ++y)
	{
		for (int x = 0; x < BINARY_MAP_WIDTH; ++x)
		{
			if (MapData[y][x] == TYPE_OBJECT_HERO)
				++totalSpawn;
			else if (MapData[y][x] == TYPE_OBJECT_GOAL)
				++totalGoals;
		}
	}
	int x, y;

	int blkCount = 0;
	
	sBoomNum = 0;
	
	spawnGoal = rand() % totalGoals;
	
	ranSpawn = rand() % totalSpawn;

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
				if (currSpawn == ranSpawn)
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
				++currSpawn;
			}

			else if (MapData[y][x] == TYPE_OBJECT_ENEMY1)
			{
				sEnemies[enemyCount].enemyCreate(TYPE_OBJECT_ENEMY1, &Pos);

				SnapToCell(&sEnemies[enemyCount].posCurr.x);
				SnapToCell(&sEnemies[enemyCount].posCurr.y);
				enemyCount++;
			}
			else if (MapData[y][x] == TYPE_OBJECT_ENEMY2)
			{
				sEnemies[enemyCount].enemyCreate(TYPE_OBJECT_ENEMY2, &Pos);

				SnapToCell(&sEnemies[enemyCount].posCurr.x);
				SnapToCell(&sEnemies[enemyCount].posCurr.y);
				enemyCount++;
			}
			else if (MapData[y][x] == TYPE_OBJECT_ENEMY3)
			{
				sEnemies[enemyCount].enemyCreate(TYPE_OBJECT_ENEMY3, &Pos);

				SnapToCell(&sEnemies[enemyCount].posCurr.x);
				SnapToCell(&sEnemies[enemyCount].posCurr.y);
				enemyCount++;
			}

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
				if (sGoalNum == spawnGoal)
				{
					sGoal.gameObjInstCreate(TYPE_OBJECT_GOAL, 1.0f, &Pos, nullptr, 0.0f);

					SnapToCell(&sGoal.posCurr.x);
					SnapToCell(&sGoal.posCurr.y);
				}
				++sGoalNum;
			}
		}
	}

	//create multiple enemies on level load
	if (gGameStateCurr != GS_TUTORIAL)
	{
		for (int a = 0; a < 5; a++)
		{
			enemyspawning(pHero, sEnemies);
		}
	}

	gameIsPaused = false;
}

/******************************************************************************/
/*!
	"Update" Platform
*/
/******************************************************************************/
void GameStatePlatformUpdate(void)
{
	int i, j;

	if (AEInputCheckTriggered(AEVK_BACK) && gameIsPaused == true)
	{
		gGameStateNext = GS_MAINMENU;
	}

	//Pause game when window is minimized
	if (AESysGetWindowHandle() != GetActiveWindow())
	{
		gameIsPaused = true;
		ToggleAudioPause();
	}

	if (AEInputCheckTriggered(AEVK_ESCAPE))
	{

		if (gameIsPaused == false)
		{
			gameIsPaused = true;
			printf("game paused\n, %d", gameIsPaused);		
			ToggleAudioPause();
		}
		else
		{
			gameIsPaused = false;
			printf("game play\n");
			ToggleAudioPause();
		}
	}
	if (gameIsPaused == false)
	{
		//KEY INPUTS
		{
			// Left
			if (AEInputCheckCurr(AEVK_A) || AEInputCheckCurr(AEVK_LEFT))
			{
				pHero.velCurr.x = -MOVE_VELOCITY_HERO;
			}
			// Right
			else if (AEInputCheckCurr(AEVK_D) || AEInputCheckCurr(AEVK_RIGHT))
			{
				pHero.velCurr.x = MOVE_VELOCITY_HERO;
			}
			// Stop
			else
			{
				pHero.velCurr.x = 0;
			}

			// Jump
			if ((AEInputCheckTriggered(AEVK_SPACE) || AEInputCheckTriggered(AEVK_UP)) && pHero.gridCollisionFlag & COLLISION_BOTTOM)
			{
				pHero.velCurr.y = JUMP_VELOCITY;
				pHero.gridCollisionFlag = 0;
			}

			//firing
			if (AEInputCheckCurr(AEVK_J) && sBoomNum < pHero.projectileMax)
			{
				pHero.playerFire(sProjectiles);
				++sBoomNum;
			}
		}

		//### Next stage DEBUG
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

		//BOUNDING BOXES & UPDATES
		{
			//Blocks
			{
				//Goal Bounding box
				sGoal.gameObjInstBoundingBox();
				//Block update
				for (i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
				{
					if (0 == (sBlocks[i].flag & FLAG_ACTIVE))
						continue;

					sBlocks[i].gameObjInstBoundingBox();
				}
				//Particle update
				for (i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
				{
					if (0 == (sParticles[i].flag & FLAG_ACTIVE))
						continue;

					sParticles[i].gameObjInstUpdatePos();
					sParticles[i].counter -= g_dt;
					if (sParticles[i].counter <= 0)
					{
						sParticles[i].gameObjInstDestroy();
					}
				}
			}

			//Enemies
			{
				for (i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
				{
					if (0 == (sEnemies[i].flag & FLAG_ACTIVE))
						continue;

					//spiders
					if (sEnemies[i].pObject->type == TYPE_OBJECT_ENEMY1)
					{
						sEnemies[i].EnemyStateMachine();
						sEnemies[i].velCurr.y = GRAVITY * g_dt + sEnemies[i].velCurr.y;
					}
					//bees
					if (sEnemies[i].pObject->type == TYPE_OBJECT_ENEMY2)
					{
						sEnemies[i].velCurr.x = cos(sEnemies[i].counter1);
						sEnemies[i].velCurr.y = sin(sEnemies[i].counter1);
						sEnemies[i].facePlayer(pHero);
						sEnemies[i].counter += g_dt;
						sEnemies[i].counter1 += g_dt;
						sEnemies[i].enemyFire(pHero, sProjectiles);
					}
					//cacti
					if (sEnemies[i].pObject->type == TYPE_OBJECT_ENEMY3)
					{
						sEnemies[i].facePlayer(pHero);
						sEnemies[i].velCurr.y = GRAVITY * g_dt + sEnemies[i].velCurr.y;
					}

					//bounding box
					sEnemies[i].gameObjInstUpdatePos();
					sEnemies[i].gameObjInstBoundingBox();
				}

				//Enemy spawning
				if (gGameStateCurr == GS_TUTORIAL && goalTimer < startingTime)
				{
					EnemySpawnTime += g_dt;
					if (EnemySpawnTime > enemySpawnRate)
					{
						EnemySpawnTime = 0;
						enemyspawning(pHero, sEnemies);
					}
				}
				else if(gGameStateCurr != GS_TUTORIAL)
				{
					EnemySpawnTime += g_dt;
					if (EnemySpawnTime > enemySpawnRate)
					{
						EnemySpawnTime = 0;
						enemyspawning(pHero, sEnemies);
					}
				}
			}

			//Player
			{
				pHero.velCurr.y = GRAVITY * g_dt + pHero.velCurr.y;
				pHero.gameObjInstUpdatePos();
				pHero.gameObjInstBoundingBox();

				//Player regen health
				pHero.regenCounter += g_dt;
				if (pHero.regenCounter >= 1 && pHero.regeneration > 0 && pHero.currentHealth < pHero.maxHealth)
				{
					pHero.currentHealth += pHero.regeneration;
					pHero.particleEffect(sParticles, P_HEAL, P_REGEN);
					pHero.regenCounter = 0;
				}

				//Un-clipping
				if (0 != GetCellValue((int)pHero.posCurr.x, (int)pHero.posCurr.y))
				{
					pHero.posCurr.y += 1;
				}

				//Player invicibility
				pHero.counter += g_dt;
				pHero.counter = (pHero.counter >= pHero.invincibleTimer) ? pHero.invincibleTimer : pHero.counter;

				//death
				if (pHero.currentHealth <= 0)
				{
					newGame = true;
					gGameStateNext = GS_GAMEOVER;	//lose
				}
			}

			//Projectile
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
		}

		//GRID COLLISIONS
		{
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
		}

		//INTERACTIONS
		{
			//Item loop
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
						pHero.particleEffect(sParticles, P_UPGRADE, P_U_DAMAGE);
						pHero.DamageUp();
					}
					if (sBlocks[i].pObject->type == TYPE_OBJECT_RANGE)
					{
						pHero.particleEffect(sParticles, P_UPGRADE, P_U_RANGE);
						pHero.RangeUp();
					}
					if (sBlocks[i].pObject->type == TYPE_OBJECT_SPEED)
					{
						pHero.particleEffect(sParticles, P_UPGRADE, P_U_SPEED);
						pHero.SpeedUp();
					}
					if (sBlocks[i].pObject->type == TYPE_OBJECT_HPUP)
					{
						pHero.particleEffect(sParticles, P_UPGRADE, P_U_HPUP);
						pHero.HpUp();
					}
					if (sBlocks[i].pObject->type == TYPE_OBJECT_VAMP)
					{
						pHero.particleEffect(sParticles, P_UPGRADE, P_U_VAMP);
						pHero.VampUp();
					}
					if (sBlocks[i].pObject->type == TYPE_OBJECT_REGEN)
					{
						pHero.particleEffect(sParticles, P_UPGRADE, P_U_REGEN);
						pHero.RegenUp();
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

			//Enemy loop
			for (i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
			{
				if (0 == (sEnemies[i].flag & FLAG_ACTIVE))
					continue;

				if ((CollisionIntersection_RectRect(pHero.boundingBox, pHero.velCurr, sEnemies[i].boundingBox, sEnemies[i].velCurr)) == true)
				{
					//Deals damage and activates player invincibility when enemy touches player
					if (pHero.counter >= pHero.invincibleTimer)
					{
						pHero.currentHealth -= sEnemies[i].damage;
						pHero.particleEffect(sParticles, P_HIT);
						pHero.counter = 0;
					}
				}
				//Respawn enemy when spawned at the map edges
				if (sEnemies[i].posCurr.x <= 0 || sEnemies[i].posCurr.x >= BINARY_MAP_WIDTH || sEnemies[i].posCurr.y <= 0 || sEnemies[i].posCurr.y >= BINARY_MAP_HEIGHT)
				{
					sEnemies[i].gameObjInstDestroy();
					enemyspawning(pHero, sEnemies);
				}
			}

			//Check if player touches goal
			if ((CollisionIntersection_RectRect(pHero.boundingBox, pHero.velCurr, sGoal.boundingBox, sGoal.velCurr)) == true && goalTimer == startingTime)
			{
				timerStart = true;
				if (gGameStateCurr == GS_TUTORIAL)
				{
					enemySpawnRate = 5.0f;
				}
				else
				{
					enemySpawnRate = 2.5f;
				}
			}
			if ((CollisionIntersection_RectRect(pHero.boundingBox, pHero.velCurr, sGoal.boundingBox, sGoal.velCurr)) == true && goalTimer <= 0)
			{
				switch (gGameStateCurr)
				{
				case GS_TUTORIAL:
					gGameStateNext = GS_WIN;
					break;
				case GS_LEVEL1:
					gGameStateNext = GS_LEVEL2;
					break;
				case GS_LEVEL2:
					gGameStateNext = GS_LEVEL3;
					break;
				case GS_LEVEL3:
					if (endless == false)
					{
						gGameStateNext = GS_WIN;
					}
					else
					{
						gGameStateNext = GS_LEVEL1;
					}
					break;
				default:
					break;
				}
			}

			if (timerStart == true)
			{
				goalTimer -= g_dt;
			}

			//Projectile loop
			for (i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
			{
				if (0 == (sProjectiles[i].flag & FLAG_ACTIVE))
					continue;

				//checks if boomernang hits enemy
				if (sProjectiles[i].pObject->type == TYPE_OBJECT_BOOMERANG)
				{
					//Enemy loop
					for (j = 0; j < GAME_OBJ_INST_NUM_MAX; ++j)
					{
						if (0 == (sEnemies[j].flag & FLAG_ACTIVE))
							continue;

						if (CollisionIntersection_RectRect(sEnemies[j].boundingBox, sEnemies[j].velCurr, sProjectiles[i].boundingBox, sProjectiles[i].velCurr) == true)
						{
							//boomerang flying OUT and hitting
							if (sEnemies[j].hit1 == false && sProjectiles[i].boomerangReturning == false)
							{
								sEnemies[j].healthPoints -= pHero.powerDamage;
								sEnemies[j].hit1 = true;
								sEnemies[j].particleEffect(sParticles, P_HIT);
							}
							//boomerang flying BACK and hitting
							if (sEnemies[j].hit2 == false && sProjectiles[i].boomerangReturning == true)
							{
								sEnemies[j].healthPoints -= pHero.powerDamage;
								sEnemies[j].hit2 = true;
								sEnemies[j].particleEffect(sParticles, P_HIT);
							}
							//Enemy dies
							if (sEnemies[j].healthPoints <= 0)
							{
								//Player heals if he has that ability
								if (pHero.vampirism > 0)
								{
									pHero.currentHealth += pHero.vampirism;
									pHero.particleEffect(sParticles, P_HEAL, P_VAMP);
								}
								sEnemies[j].gameObjInstDestroy();
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

				//Checks if enemy fire hits player
				if ((CollisionIntersection_RectRect(pHero.boundingBox, pHero.velCurr, sProjectiles[i].boundingBox, sProjectiles[i].velCurr)) == true
					&& sProjectiles[i].pObject->type == TYPE_OBJECT_BULLET)
				{
					sProjectiles[i].gameObjInstDestroy();
					pHero.currentHealth -= level;
					pHero.particleEffect(sParticles, P_HIT);
				}

				//Boomerang returning to player
				if ((CollisionIntersection_RectRect(pHero.boundingBox, pHero.velCurr, sProjectiles[i].boundingBox, sProjectiles[i].velCurr)) == true
					&& sProjectiles[i].pObject->type == TYPE_OBJECT_BOOMERANG && sProjectiles[i].boomerangReturning == true)
				{
					--sBoomNum;
					sProjectiles[i].gameObjInstDestroy();
				}
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
		sGoal.gameObjInstTransformMatrix();

		//Camera settings
		if (gGameStateCurr == GS_TUTORIAL)
		{
			// Camera fixed
			camX = BINARY_MAP_WIDTH / 2 - 10.0f;
			camY = BINARY_MAP_HEIGHT / 2 - 10.0f;
		}
		//### take out after all 3 levels are done
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
			{
				camX = (float)(pHero.posCurr.x - BINARY_MAP_WIDTH / 2) * pixelWidth;
			}
			if (pHero.posCurr.y >= 7.5f && pHero.posCurr.y <= BINARY_MAP_HEIGHT - 7.5f)
			{
				camY = (float)(pHero.posCurr.y - BINARY_MAP_HEIGHT / 2) * pixelHeight;
			}

			//Camera lock when at map edges
			//left limit
			if (pHero.posCurr.x < 10.0f)
			{
				camX = (float)(10.0f - BINARY_MAP_WIDTH / 2) * pixelWidth;
			}
			//right limit
			if (pHero.posCurr.x > BINARY_MAP_WIDTH - 10.0f)
			{
				camX = (float)((BINARY_MAP_WIDTH - 10.0f) - BINARY_MAP_WIDTH / 2) * pixelWidth;
			}
			//bottom limit
			if (pHero.posCurr.y < 7.5f)
			{
				camY = (float)(7.5f - BINARY_MAP_HEIGHT / 2) * pixelHeight;
			}
			//top limit
			if (pHero.posCurr.y > BINARY_MAP_HEIGHT - 7.5f)
			{
				camY = (float)((BINARY_MAP_HEIGHT - 7.5f) - BINARY_MAP_HEIGHT / 2) * pixelHeight;
			}
		}
		AEGfxSetCamPosition(camX, camY);
	}
	//Player Health does not overshoot max health
	if (pHero.currentHealth > pHero.maxHealth)
	{
		pHero.currentHealth = pHero.maxHealth;
	}
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

	AEGfxTextureSet(NULL, 0, 0);

	//Grid map
	for (i = 0; i < BINARY_MAP_HEIGHT; ++i)
	{
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
	}

	//Active game objects
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
	sGoal.gameObjInstDrawObject(&MapTransform);

	//Player health bar
	pHero.healthDisplay(camX, camY);

	objectiveDisplay(camX, camY, goalTimer, startingTime, gGameStateCurr);

	//Pause screen
	if (gameIsPaused == true)
	{
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetTextureMode(AE_GFX_TM_PRECISE);
		AEGfxSetPosition(camX, camY);
		AEGfxTextureSet(pauseTex, 0, 0);
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxMeshDraw(pauseMesh, AE_GFX_MDM_TRIANGLES);

		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxTextureSet(NULL, 0, 0);
		AEGfxSetTransparency(1.0f);
		AEGfxPrint(fontID, pause, -0.1f, 0.45f, 1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxPrint(fontID, conti, -0.65f, 0.3f, 1.0f, 1.0f, 1.0f, 1.0f);
		char txt[100];
		sprintf_s(txt, "X %d", pHero.powerDamage / 10 - 1);
		AEGfxPrint(fontID, txt, -0.4f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		sprintf_s(txt, "X %d", pHero.powerRange - 1);
		AEGfxPrint(fontID, txt, 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		sprintf_s(txt, "X %d", pHero.powerSpeed - 1);
		AEGfxPrint(fontID, txt, 0.6f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		sprintf_s(txt, "X %d", pHero.hpInc);
		AEGfxPrint(fontID, txt, -0.4f, -0.45f, 1.0f, 1.0f, 1.0f, 1.0f);
		sprintf_s(txt, "X %d", pHero.vampirism);
		AEGfxPrint(fontID, txt, 0.1f, -0.45f, 1.0f, 1.0f, 1.0f, 1.0f);
		sprintf_s(txt, "X %d", pHero.regeneration);
		AEGfxPrint(fontID, txt, 0.6f, -0.45f, 1.0f, 1.0f, 1.0f, 1.0f);
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
	free(pauseMesh);
	free(pauseTex);

	/*********
	Free the map data
	*********/
	FreeMapData();

	Level1BGMUnload();
	Level2BGMUnload();
	Level3BGMUnload();
}
