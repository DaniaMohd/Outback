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

#include "main.h"

bool CollisionIntersection_RectRect(const AABB& aabb1, const AEVec2& vel1,
	const AABB& aabb2, const AEVec2& vel2)
{
	UNREFERENCED_PARAMETER(aabb1);
	UNREFERENCED_PARAMETER(vel1);
	UNREFERENCED_PARAMETER(aabb2);
	UNREFERENCED_PARAMETER(vel2);

	AEVec2 Relative_Vel;
	AEVec2 First;
	AEVec2 Last;
	float TimeFirst = 0.0f;
	double TimeLast = AEFrameRateControllerGetFrameTime();
	Relative_Vel.x = vel2.x - vel1.x;
	Relative_Vel.y = vel2.y - vel1.y;

	if (aabb1.max.x < aabb2.min.x || aabb1.min.y > aabb2.max.y || aabb1.min.x > aabb2.max.x || aabb1.max.y < aabb2.min.y)
	{
		if (Relative_Vel.x < 0)
		{
			if (aabb1.min.x > aabb2.max.x)
			{
				return false;
			}

			//case 1 for x
			if (aabb1.max.x < aabb2.min.x)
			{
				First.x = aabb1.max.x - aabb2.min.x;

				if ((First.x / Relative_Vel.x) > TimeFirst)
				{
					TimeFirst = First.x / Relative_Vel.x;
				}
			}

			//case 4 for x
			if (aabb1.min.x < aabb2.max.x)
			{
				Last.x = aabb1.min.x - aabb2.max.x;

				if ((Last.x / Relative_Vel.x) < TimeLast)
				{
					TimeLast = Last.x / Relative_Vel.x;
				}
			}

			else if (Relative_Vel.x > 0)
			{
				if (aabb1.max.x < aabb2.min.x)
				{
					return false;
				}

				//case 3 for x
				if (aabb1.min.x > aabb2.max.x)
				{
					First.x = aabb1.min.x - aabb2.max.x;

					if ((First.x / Relative_Vel.x) > TimeFirst)
					{
						TimeFirst = First.x / Relative_Vel.x;
					}
				}

				//case 2 for x
				if (aabb1.max.x > aabb2.min.x)
				{
					Last.x = aabb1.max.x - aabb2.min.x;

					if ((Last.x / Relative_Vel.x) < TimeLast)
					{
						TimeLast = Last.x / Relative_Vel.x;
					}
				}

				//case 5 for x
				else
				{
					return false;
				}
			}
		}

		if (Relative_Vel.y < 0)
		{
			if (aabb1.min.y > aabb2.max.y)
			{
				return false;
			}

			//case 1 for y
			if (aabb1.max.y < aabb2.min.y)
			{
				First.y = aabb1.max.y - aabb2.min.y;

				if ((First.y / Relative_Vel.y) > TimeFirst)
				{
					TimeFirst = First.y / Relative_Vel.y;
				}
			}

			//case 4 for y
			if (aabb1.min.y < aabb2.max.y)
			{
				Last.y = aabb1.min.y - aabb2.max.y;

				if ((Last.y / Relative_Vel.y) < TimeLast)
				{
					TimeLast = Last.y / Relative_Vel.y;
				}
			}

			else if (Relative_Vel.y > 0)
			{
				if (aabb1.max.y < aabb2.min.y)
				{
					return false;
				}

				//case 3 for y
				if (aabb1.min.y > aabb2.max.y)
				{
					First.y = aabb1.min.y - aabb2.max.y;

					if ((First.y / Relative_Vel.y) > TimeFirst)
					{
						TimeFirst = First.y / Relative_Vel.y;
					}
				}

				//case 2 for y
				if (aabb1.max.y > aabb2.min.y)
				{
					Last.y = aabb1.max.y - aabb2.min.y;

					if ((Last.y / Relative_Vel.y) < TimeLast)
					{
						TimeLast = Last.y / Relative_Vel.y;
					}
				}

				//case 5 for y
				else
				{
					return false;
				}
			}
		}
	}
	else
	{
		return true;
	}

	return false;
}