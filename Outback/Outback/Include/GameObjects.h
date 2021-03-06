/****************************************************************************** /
/*!
\file			GameObjects.h
\project name	Outback
\author(s)	 	Primary:	Yu Xi Lim						(40%)
				Secondary:	Javin Ong J-Min					(30%)
				Secondary:	Binte Mohd Rizal, Nurul Dania	(30%)

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

const unsigned int	GAME_OBJ_NUM_MAX = 32;	//The total number of different objects (Shapes)
const unsigned int	GAME_OBJ_INST_NUM_MAX = 1024;	//The total number of different game object instances

//Gameplay related variables and values
const float			GRAVITY = -15.0f;
const float			JUMP_VELOCITY = 12.5f;
const float			MOVE_VELOCITY_HERO = 7.0f;
const float			MOVE_VELOCITY_ENEMY = 7.5f;
const double		ENEMY_IDLE_TIME = 2.0;

//Flags
const unsigned int	FLAG_ACTIVE = 0x00000001;
const unsigned int	FLAG_VISIBLE = 0x00000002;
const unsigned int	FLAG_NON_COLLIDABLE = 0x00000004;

class GameObjInst;
class Enemy;
class Player;
class Projectile;

//### probs need to rearrange this to be better suited for bit map
enum TYPE_OBJECT
{
	TYPE_OBJECT_EMPTY,			//0
	TYPE_OBJECT_COLLISION,		//1
	TYPE_OBJECT_HERO,			//2
	TYPE_OBJECT_GOAL,			//3
	TYPE_OBJECT_ENEMY1,			//4
	TYPE_OBJECT_ENEMY2,			//5
	TYPE_OBJECT_ENEMY3,			//6
	TYPE_OBJECT_COIN,			//7
	TYPE_OBJECT_LADDER,			//8
	TYPE_OBJECT_BULLET,			//9
	TYPE_OBJECT_BOOMERANG,		//10
	TYPE_OBJECT_MAXHP,			//11
	TYPE_OBJECT_CURRHP,			//12
	TYPE_OBJECT_DAMAGE,			//13
	TYPE_OBJECT_RANGE,			//14
	TYPE_OBJECT_SPEED,			//15
	TYPE_OBJECT_HPUP,			//16
	TYPE_OBJECT_VAMP,			//17
	TYPE_OBJECT_REGEN,			//18
	TYPE_OBJECT_PARTICLES_HIT,	//19
	TYPE_OBJECT_PARTICLES_REGEN,//20
	TYPE_OBJECT_PARTICLES_HEAL,	//21
	TYPE_OBJECT_WATER			//22
};

//### Particle Types
enum PARTICLE_TYPE
{
	P_TRAIL,	//0
	P_HIT,		//1
	P_HEAL,		//2
	P_REGEN,	//3
	P_VAMP,		//4
	P_UPGRADE,	//5
	P_U_DAMAGE,	//6
	P_U_RANGE,	//7
	P_U_SPEED,	//8
	P_U_HPUP,	//9
	P_U_VAMP,	//10
	P_U_REGEN	//11
};

enum class STATE
{
	STATE_NONE,
	STATE_GOING_LEFT,
	STATE_GOING_RIGHT
};

//State machine inner states
enum class INNER_STATE
{
	INNER_STATE_ON_ENTER,
	INNER_STATE_ON_UPDATE,
	INNER_STATE_ON_EXIT
};

struct GameObj
{
	unsigned long		type;		// object type
	AEGfxVertexList*	pMesh;		// This will hold the triangles which will form the shape of the object
	AEGfxTexture*		pTex;		// texture
};

class GameObjInst
{
public:
	GameObj*		pObject;	// pointer to the 'original'
	unsigned int	flag;		// bit flag or-ed together
	float			scale;		// scale of object
	AEVec2			posCurr;	// object current position
	AEVec2			velCurr;	// object current velocity
	float			dirCurr;	// object current direction
	bool			dirFaceR;	// check is object is facing the right
	AEMtx33			transform;	// object drawing matrix

	double			counter;	//General purpose counter (This variable will be used for the enemy state machine)

	AABB			boundingBox;// object bouding box that encapsulates the object

	void			gameObjInstCreate(unsigned int type, float scale1, AEVec2* pPos, AEVec2* pVel, float dir);	//create game object instances
	void			gameObjInstDestroy();	//destroy game object instances
	void			gameObjInstBoundingBox();	//set game object instances bounding box
	void			gameObjInstUpdatePos();		//update game object instances movement
	void			gameObjInstTransformMatrix();	//set game object instances transformation matrix
	void			gameObjInstDrawObject(AEMtx33* map);	//draw game object instances

	void			PowerUpCreate(AEVec2 pos);	//creates power ups

	void			particleEffect(GameObjInst* particle, unsigned int type, unsigned int specific = 0);
};

class Enemy : public GameObjInst
{
public:

	float			counter1;
	int				gridCollisionFlag;

	enum			STATE state;
	enum			INNER_STATE innerState;

	float			detectionRadius;

	int				healthPoints;
	int				damage;

	bool			hit1;	//hit detection from the front
	bool			hit2;	//hit detection from the back

	void			enemyCreate(unsigned int enemyType, AEVec2* pPos);
	void			EnemyStateMachine();		//State machine functions
	void			enemyGridFlag();
	void			enemyFire(Player character, Projectile* bullet);//Enemy shoot player with bullet(enemy class object calls function, input target and where to store bullet instance)
	void			facePlayer(Player character);
};

class Player : public GameObjInst
{
public:
	int				gridCollisionFlag;

	float			boomerangRange;

	//HEALTH
	int 			maxHealth;
	int				currentHealth;
	float			regenCounter;

	//How long player is invicible after touching enemy
	float			invincibleTimer;

	int baseRange;
	int baseDamage;
	int baseSpeed;

	int	projectileMax;
	int	powerRange;
	int	powerDamage;
	int	powerSpeed;
	int hpInc;
	int vampirism;
	int regeneration;

	void			playerCreate(AEVec2* pPos);
	void			playerFire(Projectile* boomerang);
	void			playerGridFlag();
	void			healthDisplay(float camX, float camY);
	void			resetPower();

	void			RangeUp();
	void			DamageUp();
	void			SpeedUp();
	void			HpUp();
	void			VampUp();
	void			RegenUp();
};

class Projectile : public GameObjInst
{
public:
	float		projectileRange;
	AEVec2		initialPos;
	AEVec2		boomerangTime;
	bool		boomerangReturning;

	void		boomerangCreate(AEVec2* startPosition, AEVec2* vel, float range);
	void		boomerangUpdate(Player character);

	void		ProjectileCreate(AEVec2* startPosition, AEVec2* vel, float angle);
	void		ProjectileUpdate();
};

void enemyspawning(Player player, Enemy* enemies);
void objectiveDisplay(float camX, float camY, float time, float start, unsigned int state, int level);

extern GameObj*				sGameObjList;
extern unsigned long		sGameObjNum;