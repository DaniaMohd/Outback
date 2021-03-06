/****************************************************************************** /
/*!
\file			Collision.cpp
\project name	Outback
\author(s)	 	Primary:	Javin Ong J-min

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#ifndef CS230_COLLISION_H_
#define CS230_COLLISION_H_

struct AABB
{
	AEVec2	min;
	AEVec2	max;
};

bool CollisionIntersection_RectRect(const AABB &aabb1, const AEVec2 &vel1, 
									const AABB &aabb2, const AEVec2 &vel2);

#endif // CS1130_COLLISION_H_