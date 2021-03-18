/******************************************************************************/
/*!
\file		GameState_Platform.cpp
\author 	Javin Ong J-min
\par    	email: ong.j@digipen.edu
\date   	15/03/2021
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
GameObj*				sGameObjList;
unsigned long			sGameObjNum;

// list of enemies
static Character*		sEnemies;
static unsigned int		sEnemyNum;

//Bullet
static GameObjInst*		sBullet;
unsigned int			sBulletNum;

// list of coins
static GameObjInst*		sCoins;
static unsigned int		sCoinNum;

// list of particles
static GameObjInst*		sParticles;

// list of enemies
static Character*		sBoomerang;

// Binary map data
static GameObjInst		pBlackInstance;
static GameObjInst		pWhiteInstance;
static AEMtx33			MapTransform;

// Hero instance
static Character		pHero;

const int				PIXEL = 20;

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
	sGameObjList = (GameObj *)calloc(GAME_OBJ_NUM_MAX, sizeof(GameObj));
	sGameObjNum = 0;

	sEnemies = (Character*)calloc(GAME_OBJ_INST_NUM_MAX, sizeof(Character));
	sEnemyNum = 0;

	sCoins = (GameObjInst*)calloc(GAME_OBJ_INST_NUM_MAX, sizeof(GameObjInst));

	sParticles = (GameObjInst*)calloc(GAME_OBJ_INST_NUM_MAX, sizeof(GameObjInst));

	sBoomerang = (Character*)calloc(GAME_OBJ_NUM_MAX, sizeof(Character));


	GameObj* pObj;

	//Creating the black object
	pObj = sGameObjList + sGameObjNum++;
	pObj->type = TYPE_OBJECT_EMPTY;


	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFF0000FF, 0.0f, 0.0f,
		0.5f, -0.5f, 0xFF0000FF, 0.0f, 0.0f,
		-0.5f, 0.5f, 0xFF0000FF, 0.0f, 0.0f);

	AEGfxTriAdd(
		0.5f, -0.5f, 0xFF0000FF, 0.0f, 0.0f,
		0.5f, 0.5f, 0xFF0000FF, 0.0f, 0.0f,
		-0.5f, 0.5f, 0xFF0000FF, 0.0f, 0.0f);
	pObj->pMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pObj->pMesh, "fail to create object!!");


	//Creating the white object
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


	//Creating the hero object
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


	//Creating the enemey1 object
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
	pObj->pTex = AEGfxTextureLoad("..\\Resources\\Textures\\Enemy.png");
	AE_ASSERT_MESG(pObj->pTex, "Failed to create Enemy Texture!");


	//Creating the Coin object
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

	//Creating the Particle object
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


	//Creating the Bullets
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
	pObj->pTex = AEGfxTextureLoad("..\\Resources\\Textures\\Particle.png");
	AE_ASSERT_MESG(pObj->pTex, "Failed to create bullets!");

	// Loading the levels
	if (gGameStateCurr == GS_LEVEL1)
	{
		//Importing Data
		if (!ImportMapDataFromFile("..\\Resources\\Levels\\Exported.txt"))
			gGameStateNext = GS_QUIT;
	}
	else if (gGameStateCurr == GS_LEVEL2)
	{
		if (!ImportMapDataFromFile("..\\Resources\\Levels\\Exported1.txt"))
			gGameStateNext = GS_QUIT;
	}
	else if (gGameStateCurr == GS_LEVEL3)
	{
		if (!ImportMapDataFromFile("..\\Resources\\Levels\\Exported2.txt"))
			gGameStateNext = GS_QUIT;
	}


	//Computing the matrix which take a point out of the normalized coordinates system
	//of the binary map
	/***********
	Compute a transformation matrix and save it in "MapTransform".
	This transformation transforms any point from the normalized coordinates system of the binary map.
	Later on, when rendering each object instance, we should concatenate "MapTransform" with the
	object instance's own transformation matrix

	Compute a translation matrix (-Grid width/2, -Grid height/2) and save it in "trans"
	Compute a scaling matrix and save it in "scale")
	Concatenate scale and translate and save the result in "MapTransform"
	***********/
	AEMtx33 scale, trans;

	AEMtx33Trans(&trans, -(float)BINARY_MAP_WIDTH / 2, -(float)BINARY_MAP_HEIGHT / 2);

	// Whole level fits in the screen
	if (gGameStateCurr == GS_LEVEL1 || gGameStateCurr == GS_LEVEL2)
	{
		AEMtx33Scale(&scale, (float)(AEGetWindowWidth()/BINARY_MAP_WIDTH), (float)(AEGetWindowHeight() / BINARY_MAP_HEIGHT));
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

	//Create an object instance representing the black cell.
	//This object instance should not be visible. When rendering the grid cells, each time we have
	//a non collision cell, we position this instance in the correct location and then we render it
	pBlackInstance.gameObjInstCreate(TYPE_OBJECT_EMPTY, 1.0f, 0, 0, 0.0f);
	pBlackInstance.flag ^= FLAG_VISIBLE;
	pBlackInstance.flag |= FLAG_NON_COLLIDABLE;

	//Create an object instance representing the white cell.
	//This object instance should not be visible. When rendering the grid cells, each time we have
	//a collision cell, we position this instance in the correct location and then we render it
	pWhiteInstance.gameObjInstCreate(TYPE_OBJECT_COLLISION, 1.0f, 0, 0, 0.0f);
	pWhiteInstance.flag ^= FLAG_VISIBLE;
	pWhiteInstance.flag |= FLAG_NON_COLLIDABLE;

	//Setting the inital number of hero lives
	HeroLives = HERO_LIVES;

	AEVec2 Pos;

	// creating the main character, the enemies and the coins according 
	// to their initial positions in MapData

	/***********
	Loop through all the array elements of MapData 
	(which was initialized in the "GameStatePlatformLoad" function
	from the .txt file
		if the element represents a collidable or non collidable area
			don't do anything

		if the element represents the hero
			Create a hero instance
			Set its position depending on its array indices in MapData
			Save its array indices in Hero_Initial_X and Hero_Initial_Y 
			(Used when the hero dies and its position needs to be reset)

		if the element represents an enemy
			Create an enemy instance
			Set its position depending on its array indices in MapData
			
		if the element represents a coin
			Create a coin instance
			Set its position depending on its array indices in MapData
			
	***********/
	for(y = 0; y < BINARY_MAP_HEIGHT; ++y)
		for(x = 0; x < BINARY_MAP_WIDTH; ++x)
		{
			if (MapData[y][x] == TYPE_OBJECT_EMPTY || MapData[y][x] == TYPE_OBJECT_COLLISION)
				continue;

			Pos.x = (float)(x);
			Pos.y = (float)(y);

			if (MapData[y][x] == TYPE_OBJECT_HERO)
			{
				Hero_Initial_X = x;
				Hero_Initial_Y = y;

				pHero.gameObjInstCreate(TYPE_OBJECT_HERO, 1.0f, &Pos, nullptr, 0.0f);

				SnapToCell(&pHero.posCurr.x);
				SnapToCell(&pHero.posCurr.y);
			}

			else if (MapData[y][x] == TYPE_OBJECT_ENEMY1)
			{
				sEnemies[sEnemyNum].characterCreate(TYPE_OBJECT_ENEMY1, 1.0f, &Pos, nullptr, 0.0f, STATE::STATE_GOING_LEFT);

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
	int i;
	//Handle Input
	/***********
	if right is pressed
		Set hero velocity X to MOVE_VELOCITY_HERO
	else
	if left is pressed
		Set hero velocity X to -MOVE_VELOCITY_HERO
	else
		Set hero velocity X to 0

	if space is pressed AND Hero is colliding from the bottom
		Set hero velocity Y to JUMP_VELOCITY

	if Escape is pressed
		Exit to menu
	***********/
	// Left
	if (AEInputCheckCurr(AEVK_A) || AEInputCheckCurr(AEVK_LEFT))
	{
		pHero.velCurr.x = -MOVE_VELOCITY_HERO;
		//pHero.dirCurr = PI;
		if (pHero.gridCollisionFlag == COLLISION_BOTTOM)
		{
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
		//pHero.dirCurr = 0;
		if (pHero.gridCollisionFlag == COLLISION_BOTTOM)
		{
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
		if (CoinsCollected >= 0 && CoinsCollected < 16)
		{
			pHero.velCurr.y = JUMP_VELOCITY;
		}
		else if (CoinsCollected >= 15 && CoinsCollected < 31)
		{
			pHero.velCurr.y = JUMP_VELOCITY * 1.5;
		}
		else if (CoinsCollected >= 30)
		{
			pHero.velCurr.y = JUMP_VELOCITY * 2;
		}
		pHero.gridCollisionFlag = 0;
	}

	if (AEInputCheckTriggered(AEVK_K))
	{
		for (int i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
		{
			// skip non-active object
			if (0 == (sEnemies[i].flag & FLAG_ACTIVE))
				continue;
			for (int j = 0; j < GAME_OBJ_NUM_MAX; j++)
			{
				if (sBoomerang[j].flag == 0)
				{
					sBoomerang[j].ProjectileCreate(pHero, sEnemies[i]);
					break;
				}
			}
		}
	}

	// Main Menu
	if (AEInputCheckTriggered(AEVK_BACK))
	{
		gGameStateNext = GS_MAINMENU;
	}

	if (AEInputCheckTriggered(AEVK_J))
	{
		for (i = 0; i < GAME_OBJ_NUM_MAX; ++i)
		{
			if (0 == (sBoomerang[i].flag & FLAG_ACTIVE))
			{
				sBoomerang[i].boomerangCreate(pHero);
				printf("FIRE!!!");
				break;
			}
		}
	}
	
	//Update object instances physics and behavior
	for(i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
	{
		// skip non-active object
		if (0 == (sEnemies[i].flag & FLAG_ACTIVE))
			continue;

		/****************
		Apply gravity
			Velocity Y = Gravity * Frame Time + Velocity Y

		If object instance is an enemy
			Apply enemy state machine
		****************/
		sEnemies[i].velCurr.y = GRAVITY * g_dt + sEnemies[i].velCurr.y;
		sEnemies[i].EnemyStateMachine();
	}
	pHero.velCurr.y = GRAVITY * g_dt + pHero.velCurr.y;

	//Update object instances positions
	for(i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
	{
		// skip non-active object
		if (0 == (sEnemies[i].flag & FLAG_ACTIVE))
			continue;

		/**********
		update the position using: P1 = V1*dt + P0
		Get the bouding rectangle of every active instance:
			boundingRect_min = -BOUNDING_RECT_SIZE * instance->scale + instance->pos
			boundingRect_max = BOUNDING_RECT_SIZE * instance->scale + instance->pos
		**********/

		sEnemies[i].gameObjInstUpdatePos();
		sEnemies[i].gameObjInstBoundingBox();
	}

	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
	{
		// skip non-active object
		if (0 == (sParticles[i].flag & FLAG_ACTIVE))
			continue;

		/**********
		update the position using: P1 = V1*dt + P0
		Get the bouding rectangle of every active instance:
			boundingRect_min = -BOUNDING_RECT_SIZE * instance->scale + instance->pos
			boundingRect_max = BOUNDING_RECT_SIZE * instance->scale + instance->pos
		**********/

		sParticles[i].gameObjInstUpdatePos();
		sParticles[i].counter -= g_dt;
		sParticles[i].velCurr.y = GRAVITY * g_dt + sParticles[i].velCurr.y;
		if (sParticles[i].counter <= 0)
		{
			sParticles[i].gameObjInstDestroy();
		}
	}

	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
	{
		// skip non-active object
		if (0 == (sCoins[i].flag & FLAG_ACTIVE))
			continue;

		/**********
		update the position using: P1 = V1*dt + P0
		Get the bouding rectangle of every active instance:
			boundingRect_min = -BOUNDING_RECT_SIZE * instance->scale + instance->pos
			boundingRect_max = BOUNDING_RECT_SIZE * instance->scale + instance->pos
		**********/

		sCoins[i].gameObjInstBoundingBox();
	}

	pHero.gameObjInstUpdatePos();
	pHero.gameObjInstBoundingBox();

	for (i = 0; i < GAME_OBJ_NUM_MAX; ++i)
	{
		// skip non-active object
		if (0 == (sBoomerang[i].flag & FLAG_ACTIVE))
			continue;

		sBoomerang[i].gameObjInstUpdatePos();
		sBoomerang[i].gameObjInstBoundingBox();
		if (sBoomerang[i].pObject->type == TYPE_OBJECT_BULLET)
		{
			sBoomerang[i].ProjectileUpdate();

			//counter set to 1 because the veloci
			//if (sBoomerang[i].counter > 1)
			//{
			//	sBoomerang[i].gameObjInstDestroy();
			//}
		}
		else
		sBoomerang[i].boomerangReturn(pHero);
	}

	//Check for grid collision
	for(i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
	{
		// skip non-active object
		if (0 == (sEnemies[i].flag & FLAG_ACTIVE))
			continue;

		/*************
		Update grid collision flag

		if collision from bottom
			Snap to cell on Y axis
			Velocity Y = 0

		if collision from top
			Snap to cell on Y axis
			Velocity Y = 0
	
		if collision from left
			Snap to cell on X axis
			Velocity X = 0

		if collision from right
			Snap to cell on X axis
			Velocity X = 0
		*************/
		sEnemies[i].gridCollisionFlag = CheckInstanceBinaryMapCollision(sEnemies[i].posCurr.x, sEnemies[i].posCurr.y, sEnemies[i].scale, sEnemies[i].scale);

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

	pHero.gridCollisionFlag = CheckInstanceBinaryMapCollision(pHero.posCurr.x, pHero.posCurr.y, pHero.scale, pHero.scale);

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

	//Checking for collision among object instances:
	//Hero against enemies
	//Hero against coins

	/**********
	for each game object instance
		Skip if it's inactive or if it's non collidable

		If it's an enemy
			If collision between the enemy instance and the hero (rectangle - rectangle)
				Decrement hero lives
				Reset the hero's position in case it has lives left, otherwise RESTART the level

		If it's a coin
			If collision between the coin instance and the hero (rectangle - rectangle)
				Remove the coin and decrement the coin counter.
				Quit the game level to the menu in case no more coins are left
	**********/
	
	for(i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
	{
		if (0 == (sEnemies[i].flag & FLAG_ACTIVE))
			continue;

		if ((CollisionIntersection_RectRect(pHero.boundingBox, pHero.velCurr, sEnemies[i].boundingBox, sEnemies[i].velCurr)) == true)
		{
			onChange = true;
			HeroLives--;

			if (HeroLives > 0)
			{
				AEVec2Set(&pHero.posCurr, (float)Hero_Initial_X + 0.5f, (float)Hero_Initial_Y + 0.5f);
			}

			else if (HeroLives == 0)
			{
				printf("Try Again!\n");
				gGameStateNext = GS_RESTART;

				HeroLives = 3;

				CoinsCollected = 0;
			}
		}
	}

	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
	{
		if (0 == (sCoins[i].flag & FLAG_ACTIVE))
			continue;

		if ((CollisionIntersection_RectRect(pHero.boundingBox, pHero.velCurr, sCoins[i].boundingBox, sCoins[i].velCurr)) == true)
		{
			onChange = true;

			sCoins[i].gameObjInstDestroy();

			CoinsCollected++;

			if (CoinsCollected == TotalCoins)
			{
				if (gGameStateNext == GS_LEVEL1)
				{
					gGameStateNext = GS_LEVEL2;
				}
				else if (gGameStateNext == GS_LEVEL2)
				{
					gGameStateNext = GS_LEVEL3;
				}
				else if (gGameStateNext == GS_LEVEL3)
				{
					gGameStateNext = GS_WIN;
				}
			}
		}
	}
	for (i = 0; i < GAME_OBJ_NUM_MAX; ++i)
	{
		if (0 == (sBoomerang[i].flag & FLAG_ACTIVE))
			continue;

		if ((CollisionIntersection_RectRect(pHero.boundingBox, pHero.velCurr, sBoomerang[i].boundingBox, sBoomerang[i].velCurr)) == true && sBoomerang[i].projectileReturning == true)
		{
			printf("returned\n");
			sBoomerang[i].gameObjInstDestroy();
		}
	}
	
	//Computing the transformation matrices of the game object instances
	for(i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
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
	for (i = 0; i < GAME_OBJ_NUM_MAX; ++i)
	{
		if (0 == (sBoomerang[i].flag & FLAG_ACTIVE))
			continue;

		sBoomerang[i].gameObjInstTransformMatrix();
	}
	pHero.gameObjInstTransformMatrix();

	if (gGameStateCurr == GS_LEVEL1 || gGameStateCurr == GS_LEVEL2)
	{
		// Camera fixed
		camX = BINARY_MAP_WIDTH / 2 - 10.0f;
		camY = BINARY_MAP_HEIGHT / 2 - 10.0f;
	}
	else
	{
		// Camera tracking
		if (camX != AEGetWindowWidth() / 2)
			camX = (float)(pHero.posCurr.x - BINARY_MAP_WIDTH / 2) * PIXEL;

		if (camY != AEGetWindowHeight() / 2)
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

	//Drawing the tile map

	/******REMINDER*****
	You need to concatenate MapTransform with the transformation matrix 
	of any object you want to draw. MapTransform transform the instance 
	from the normalized coordinates system of the binary map
	*******************/

	/*********
	for each array element in BinaryCollisionArray (2 loops)
		Compute the cell's translation matrix acoording to its 
		X and Y coordinates and save it in "cellTranslation"
		Concatenate MapTransform with the cell's transformation 
		and save the result in "cellFinalTransformation"
		Send the resultant matrix to the graphics manager using "AEGfxSetTransform"

		Draw the instance's shape depending on the cell's value using "AEGfxMeshDraw"
			Use the black instance in case the cell's value is TYPE_OBJECT_EMPTY
			Use the white instance in case the cell's value is TYPE_OBJECT_COLLISION
	*********/
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxTextureSet(NULL, 0, 0);
	for(i = 0; i < BINARY_MAP_HEIGHT; ++i)
		for(j = 0; j < BINARY_MAP_WIDTH; ++j)
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

	//Drawing the object instances
	/**********
	For each active and visible object instance
		Concatenate MapTransform with its transformation matrix
		Send the resultant matrix to the graphics manager using "AEGfxSetTransform"
		Draw the instance's shape using "AEGfxMeshDraw"
	**********/
	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		// skip non-active object
		if (0 == (sEnemies[i].flag & FLAG_ACTIVE) || 0 == (sEnemies[i].flag & FLAG_VISIBLE))
			continue;

		sEnemies[i].gameObjInstDrawObject(&MapTransform);
		//AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);

		//AEGfxSetBlendMode(AE_GFX_BM_BLEND);

		//AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

		//AEGfxSetTextureMode(AE_GFX_TM_PRECISE);

		//AEGfxSetTransparency(1.0f);

		//AEMtx33Concat(&cellFinalTransformation, &MapTransform, &sEnemies[i].transform);

		//AEGfxSetTransform(cellFinalTransformation.m);

		//AEGfxTextureSet(sEnemies[i].pObject->pTex, 0, 0);

		//AEGfxMeshDraw(sEnemies[i].pObject->pMesh, AE_GFX_MDM_TRIANGLES);
		//Don't forget to concatenate the MapTransform matrix with the transformation of each game object instance
	}
	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		// skip non-active object
		if (0 == (sCoins[i].flag & FLAG_ACTIVE) || 0 == (sCoins[i].flag & FLAG_VISIBLE))
			continue;
		sCoins[i].gameObjInstDrawObject(&MapTransform);
		//AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);

		//AEGfxSetBlendMode(AE_GFX_BM_BLEND);

		//AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

		//AEGfxSetTextureMode(AE_GFX_TM_PRECISE);

		//AEGfxSetTransparency(1.0f);

		//AEMtx33Concat(&cellFinalTransformation, &MapTransform, &sCoins[i].transform);

		//AEGfxSetTransform(cellFinalTransformation.m);

		//AEGfxTextureSet(sCoins[i].pObject->pTex, 0, 0);

		//AEGfxMeshDraw(sCoins[i].pObject->pMesh, AE_GFX_MDM_TRIANGLES);
		//Don't forget to concatenate the MapTransform matrix with the transformation of each game object instance
	}
	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		// skip non-active object
		if (0 == (sParticles[i].flag & FLAG_ACTIVE) || 0 == (sParticles[i].flag & FLAG_VISIBLE))
			continue;
		sParticles[i].gameObjInstDrawObject(&MapTransform);
		//AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);

		//AEGfxSetBlendMode(AE_GFX_BM_BLEND);

		//AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

		//AEGfxSetTextureMode(AE_GFX_TM_PRECISE);

		//AEGfxSetTransparency(1.0f);

		//AEMtx33Concat(&cellFinalTransformation, &MapTransform, &sParticles[i].transform);

		//AEGfxSetTransform(cellFinalTransformation.m);

		//AEGfxTextureSet(sParticles[i].pObject->pTex, 0, 0);

		//AEGfxMeshDraw(sParticles[i].pObject->pMesh, AE_GFX_MDM_TRIANGLES);
		//Don't forget to concatenate the MapTransform matrix with the transformation of each game object instance
	}
	for (i = 0; i < GAME_OBJ_NUM_MAX; i++)
	{
		// skip non-active object
		if (0 == (sBoomerang[i].flag & FLAG_ACTIVE) || 0 == (sBoomerang[i].flag & FLAG_VISIBLE))
			continue;
		sBoomerang[i].gameObjInstDrawObject(&MapTransform);
		//AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);

		//AEGfxSetBlendMode(AE_GFX_BM_BLEND);

		//AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

		//AEGfxSetTextureMode(AE_GFX_TM_PRECISE);

		//AEGfxSetTransparency(1.0f);

		//AEMtx33Concat(&cellFinalTransformation, &MapTransform, &sBoomerang[i].transform);

		//AEGfxSetTransform(cellFinalTransformation.m);

		//AEGfxTextureSet(sBoomerang[i].pObject->pTex, 0, 0);

		//AEGfxMeshDraw(sBoomerang[i].pObject->pMesh, AE_GFX_MDM_TRIANGLES);
		//Don't forget to concatenate the MapTransform matrix with the transformation of each game object instance
	}
	pHero.gameObjInstDrawObject(&MapTransform);

	//AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);

	//AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	//AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	//AEGfxSetTextureMode(AE_GFX_TM_PRECISE);

	//AEGfxSetTransparency(1.0f);

	//AEMtx33Concat(&cellFinalTransformation, &MapTransform, &pHero.transform);

	//AEGfxSetTransform(cellFinalTransformation.m);

	//AEGfxTextureSet(pHero.pObject->pTex, 0, 0);

	//AEGfxMeshDraw(pHero.pObject->pMesh, AE_GFX_MDM_TRIANGLES);

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
	}
	for (unsigned int i = 0; i < GAME_OBJ_NUM_MAX; i++)
	{
		sBoomerang[i].gameObjInstDestroy();
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
	free(sBoomerang);

	/*********
	Free the map data
	*********/
	FreeMapData();
}
