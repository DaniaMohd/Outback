/******************************************************************************/
/*!
\file		Map.cpp
\author 	Javin Ong J-min
\par    	email: ong.j@digipen.edu
\date   	15/03/2021
\brief      This file contains the definition for the map function.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "Main.h"

//Setting intital binary map values
extern int					BINARY_MAP_WIDTH = 0;
extern int					BINARY_MAP_HEIGHT = 0;
extern int**				MapData = nullptr;
extern int**				BinaryCollisionArray = nullptr;

int ImportMapDataFromFile(const char* FileName)
{
	FILE* fptr = NULL;
	errno_t error = fopen_s(&fptr, FileName, "r");

	if (error == 0 && fptr != NULL)
	{
		fscanf_s(fptr, "Width %d", &BINARY_MAP_WIDTH);
		fscanf_s(fptr, "\nHeight %d", &BINARY_MAP_HEIGHT);

		MapData = new int* [BINARY_MAP_HEIGHT];
		BinaryCollisionArray = new int* [BINARY_MAP_HEIGHT];
		for (int i = BINARY_MAP_HEIGHT - 1; i >= 0; i--)
		{
			MapData[i] = new int[BINARY_MAP_WIDTH];
			BinaryCollisionArray[i] = new int[BINARY_MAP_WIDTH];
			for (int j = 0; j < BINARY_MAP_WIDTH; j++)
			{
				fscanf_s(fptr, "%d", &MapData[i][j]);

				BinaryCollisionArray[i][j] = MapData[i][j];

				if (BinaryCollisionArray[i][j] != 1)
				{
					BinaryCollisionArray[i][j] = 0;
				}
			}
		}
		fclose(fptr);
		return 1;
	}
	return 0;
}

void FreeMapData(void)
{
	for (int i = 0; i < BINARY_MAP_HEIGHT; i++)
	{
		delete[] MapData[i];
		delete[] BinaryCollisionArray[i];
	}
	delete[] MapData;
	delete[] BinaryCollisionArray;
}

void PrintRetrievedInformation(void)
{
	for (int i = BINARY_MAP_HEIGHT - 1; i >= 0; i--)
	{
		for (int j = 0; j < BINARY_MAP_WIDTH; j++)
		{
			printf("%d ", MapData[i][j]);
		}
		printf("\n");
	}
}

int GetCellValue(int X, int Y)
{
	if (X >= 0 && X < BINARY_MAP_WIDTH && Y >= 0 && Y < BINARY_MAP_HEIGHT)
	{
		return	BinaryCollisionArray[Y][X];
	}
	return 0;
}

void SnapToCell(float* Coordinate)
{
	*Coordinate = (int)(*Coordinate) + 0.5f;
}

int CheckInstanceBinaryMapCollision(float PosX, float PosY,
	float scaleX, float scaleY)
{
	int flag = 0;

	// Left-side top
	float left_t_x = PosX - scaleX / 2;
	float left_t_y = PosY + scaleY / 4;

	// Left-side bottom
	float left_b_x = PosX - scaleX / 2;
	float left_b_y = PosY - scaleY / 4;

	// Right-side top
	float right_t_x = PosX + scaleX / 2;
	float right_t_y = PosY + scaleY / 4;

	// Right-side bottom
	float right_b_x = PosX + scaleX / 2;
	float right_b_y = PosY - scaleY / 4;

	// Top-side left
	float top_l_x = PosX - scaleX / 4;
	float top_l_y = PosY + scaleY / 2;

	// Top-side right
	float top_r_x = PosX + scaleX / 4;
	float top_r_y = PosY + scaleY / 2;

	// Bottom-side left
	float bottom_l_x = PosX - scaleX / 4;
	float bottom_l_y = PosY - scaleY / 2;

	// Bottom-side right
	float bottom_r_x = PosX + scaleX / 4;
	float bottom_r_y = PosY - scaleY / 2;

	// check for left side collision
	if (GetCellValue((int)left_t_x, (int)left_t_y) || GetCellValue((int)left_b_x, (int)left_b_y))
	{
		flag = flag | COLLISION_LEFT;
		//printf("hitting left");
	}
	// Check for right side collision
	if (GetCellValue((int)right_t_x, (int)right_t_y) || GetCellValue((int)right_b_x, (int)right_b_y))
	{
		flag = flag | COLLISION_RIGHT;
		//printf("hitting right");
	}
	// Check for top collision
	if (GetCellValue((int)top_l_x, (int)top_l_y) || GetCellValue((int)top_r_x, (int)top_r_y))
	{
		flag = flag | COLLISION_TOP;
		//printf("hitting top");
	}
	// Check for bottom collision
	if (GetCellValue((int)bottom_l_x, (int)bottom_l_y) || GetCellValue((int)bottom_r_x, (int)bottom_r_y))
	{
		flag = flag | COLLISION_BOTTOM;
		//printf("hitting bottom");
	}
	return flag;
}