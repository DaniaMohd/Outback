//#include "main.h"

const unsigned int	GAME_OBJ_NUM_MAX = 32;	//The total number of different objects (Shapes)
const unsigned int	GAME_OBJ_INST_NUM_MAX = 1024;	//The total number of different game object instances

//Gameplay related variables and values
const float			GRAVITY = -20.0f;
const float			JUMP_VELOCITY = 12.5f;
const float			MOVE_VELOCITY_HERO = 7.0f;
const float			MOVE_VELOCITY_ENEMY = 7.5f;
const double		ENEMY_IDLE_TIME = 2.0;
const int			HERO_LIVES = 3;

//Flags
const unsigned int	FLAG_ACTIVE = 0x00000001;
const unsigned int	FLAG_VISIBLE = 0x00000002;
const unsigned int	FLAG_NON_COLLIDABLE = 0x00000004;

class GameObjInst;
class Enemy;
class Player;
class Projectile;

enum TYPE_OBJECT
{
	TYPE_OBJECT_EMPTY,			//0
	TYPE_OBJECT_COLLISION,		//1
	TYPE_OBJECT_HERO,			//2
	TYPE_OBJECT_ENEMY1,			//3
	TYPE_OBJECT_COIN,			//4
	TYPE_OBJECT_PARTICLES,		//5
	TYPE_OBJECT_BULLET,			//6
	TYPE_OBJECT_BOOMERANG		//7
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

	void			gameObjInstCreate(unsigned int type, float scale1, AEVec2* pPos, AEVec2* pVel, float dir);
	void			gameObjInstDestroy();
	void			gameObjInstBoundingBox();
	void			gameObjInstUpdatePos();
	void			gameObjInstTransformMatrix();
	void			gameObjInstDrawObject(AEMtx33* map);
};

class Enemy : public GameObjInst
{
public:
	//Used to hold the current 
	int				gridCollisionFlag;

	//State of the object instance
	enum			STATE state;
	enum			INNER_STATE innerState;

	int				detectionRange;
	int				firingRange;

	int				healthPoints;
	//int			damage;

	void			enemyCreate(unsigned int enemyType, AEVec2* pPos);
	void			EnemyStateMachine();		//State machine functions

	//Enemy shoot player with bullet(enemy class object calls function, input target and where to store bullet instance)
	void			enemyFire(Player character, Projectile *bullet);

	//enemy charging function, input player
};

class Player : public GameObjInst
{
public:
	//Used to hold the current 
	int				gridCollisionFlag;

	float			boomerangRange;

	//int			healthPoints;
	int				damage;

	int				projectileMax;
	int				powerRange;

	void			playerCreate(AEVec2* pPos);
	void			playerFire(Projectile *boomerang);
	
	//probably additional functions to aid in upgrade pickups
};

class Projectile : public GameObjInst
{
public:
	//int				range;
	float			projectileRange;
	bool			boomerangReturning;

	AEVec2			initialPos;
	AEVec2			boomerangTime;

	void			boomerangCreate(AEVec2* startPosition, AEVec2* vel, float range);
	void			boomerangUpdate(Player character);

	void			ProjectileCreate(AEVec2* startPosition, AEVec2* vel, float angle);
	void			ProjectileUpdate();
};

//function of input type player and enemy pointer to spawn enemies arounf the player???

extern GameObj*				sGameObjList;
extern unsigned long		sGameObjNum;

//extern to carry over player data, probs