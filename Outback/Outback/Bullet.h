#pragma once
typedef struct eneBul {
	float ex, ey;
	float delta;
	float EvelocityX, EvelocityY;
	int edir;
	float diameter;
	int status;
	float damage;
}eneBul;

void createBullet(void);