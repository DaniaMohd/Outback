/****************************************************************************** /
/*!
\file			Map.h
\project name	Outback
\author(s)	 	Primary:	Javin Ong J-Min

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#ifndef MAP_H_
#define MAP_H_

const int	COLLISION_LEFT	= 0x00000001;	//0001
const int	COLLISION_RIGHT	= 0x00000002;	//0010
const int	COLLISION_TOP	= 0x00000004;	//0100
const int	COLLISION_BOTTOM= 0x00000008;	//1000

extern int		BINARY_MAP_WIDTH;
extern int		BINARY_MAP_HEIGHT;
extern int**	MapData;
extern int**	BinaryCollisionArray;

int		ImportMapDataFromFile(const char* FileName);
void	FreeMapData(void);
void	PrintRetrievedInformation(void);
int		GetCellValue(int X, int Y);
void	SnapToCell(float* Coordinate);
int		CheckInstanceBinaryMapCollision(float PosX, float PosY, float scaleX, float scaleY);
#endif // !MAP_H_

