#include "RayTrace.h"

#include "Hero.h"

const byte RayTrace::LEFT_BIT = 1;
const byte RayTrace::RIGHT_BIT = 2;
const byte RayTrace::BOTTOM_BIT = 4;
const byte RayTrace::TOP_BIT = 8;

RayTrace::RayTrace(World * world, Vector2D start, Vector2D end) : startPoint(start),
	endPoint(end),
	angle((end - start).rotation())
{
	this->ownerWorld = world;
	this->rayLength = (end - start).size();
}

RayTrace::RayTrace(World * world, Vector2D start, Rotator rotation, float length) : startPoint(start),
	endPoint(Vector2D(rotation) * length),
	angle(rotation)
{
	this->ownerWorld = world;
	this->rayLength = length;
}

RayTrace::~RayTrace(void)
{
}

byte RayTrace::getDotCode(const BoundingBox *box, const Vector2D *dot)
{
	return (((dot->x < box->minX) ? LEFT_BIT : 0)
		| ((dot->x > box->maxX) ? RIGHT_BIT : 0)
		| ((dot->y < box->minY) ? BOTTOM_BIT : 0)
		| ((dot->y > box->maxY) ? TOP_BIT : 0));
}

inline float SignedArea(Vector2D a, Vector2D b, Vector2D c)
{
	return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

inline void Swap(float &a, float &b)
{
	float temp;
	temp = a;
	a = b;
	b = temp;
}

//
const double EPS = 1E-4;

inline float Det(float a, float b, float c, float d)
{
	return a * d - b * c;
}

inline bool IsBetween(float a, float b, double c)
{
	return std::min(a, b) <= c + EPS && c <= std::max(a, b) + EPS;
}

inline bool IsAAIntersect(float a, float b, float c, float d)
{
	if (a > b)
		Swap(a, b);
	if (c > d)
		Swap(c, d);

	return std::max(a, c) <= std::min(b, d);
}

bool RayTrace::checkIntersect2Lines(Vector2D A1, Vector2D A2, Vector2D B1, Vector2D B2)
{
	if (IsAAIntersect(A1.x, A2.x, B1.x, B2.x)
		&& IsAAIntersect(A1.y, A2.y, B1.y, B2.y)
		&& SignedArea(A1, A2, B1) * SignedArea(A1, A2, B2) <= 0
		&& SignedArea(B1, B2, A1) * SignedArea(B1, B2, A2) <= 0)
	{
		return true;
	}
	return false;
}

Vector2D RayTrace::getPointIntersect2Lines(Vector2D A1, Vector2D A2, Vector2D B1, Vector2D B2)
{
	float DA1 = A1.y - A2.y;
	float DB1 = A2.x - A1.x;
	float DC1 = -DA1 * A1.x - DB1 * A1.y;
	float DA2 = B1.y - B2.y;
	float DB2 = B2.x - B1.x;
	float DC2 = -DA2 * B1.x - DB2 * B1.y;

	float zn = Det(DA1, DB1, DA2, DB2);
	
	// if lines are not parallel
	if (zn != 0)
	{
		float x = (float) (-Det(DC1, DB1, DC2, DB2) * 1. / zn);
		float y = (float) (-Det(DA1, DC1, DA2, DC2) * 1. / zn);

		if (IsBetween(A1.x, A2.x, x) && IsBetween(A1.y, A2.y, y)
			&& IsBetween(B1.x, B2.x, x) && IsBetween(B1.y, B2.y, y))
		{
			return Vector2D(x, y);
		}
	}

	// if lines not intersected
	return ZERO_VECTOR;
}

bool RayTrace::checkIntersectAABBLine(const BoundingBox* box, const Vector2D* first, const Vector2D* last)
{
	float l = box->minX;
	float t = box->minY;
	float r = box->maxX;
	float b = box->maxY;

	float x1 = first->x;
	float y1 = first->y;
	float x2 = last->x;
	float y2 = last->y;

	// ToDo: make it simpler (for axis-aligned borders)
	if (this->checkIntersect2Lines(Vector2D(l, t), Vector2D(l, b), Vector2D(x1, y1), Vector2D(x2, y2))
		||
		this->checkIntersect2Lines(Vector2D(r, t), Vector2D(r, b), Vector2D(x1, y1), Vector2D(x2, y2))
		||
		this->checkIntersect2Lines(Vector2D(l, t), Vector2D(r, t), Vector2D(x1, y1), Vector2D(x2, y2))
		||
		this->checkIntersect2Lines(Vector2D(l, b), Vector2D(r, b), Vector2D(x1, y1), Vector2D(x2, y2)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool RayTrace::checkIntersectVertLineWithLine(Vector2D A1, Vector2D A2, float x, float minY, float maxY)
{
	return true;
}

bool RayTrace::checkIntersectHoryLineWithLine(Vector2D A1, Vector2D A2, float y, float minX, float maxX)
{
	return true;
}

bool RayTrace::fastTrace()
{
	IActor *itActor;
	// for each actor in the world
	for (std::set<IActor*>::iterator i = this->ownerWorld->allActors.begin(), iEnd = this->ownerWorld->allActors.end(); i != iEnd; i++)
	{
		itActor = *i;
		if (itActor->getType() != AT_Light && itActor->getType() != AT_Special)
		{
			// get bounding box of current actor
			BoundingBox box = itActor->getBoundingBox();
			// get cohen's code for start point
			byte a = this->getDotCode(&box, &this->startPoint);
			// get cohen's code for end point
			byte b = this->getDotCode(&box, &this->endPoint);

			// if points on some side of BB
			if ((a & b) != 0)
			{
				// go to the next actor
				continue;
			}

			// if raytrace intersect AABB of this actor
			if (((a == 0 || b == 0) && ((a & b) == 0))					// one point is in BB another is out BB
				||
				((a | b) == 3 || (a | b) == 12)							// points on opposite sides of BB // 0011 or 1100 
				||
				this->checkIntersectAABBLine(&box, &this->startPoint, &this->endPoint))	// other cases of intersection
			{
				//return true;
				// get hull of current actor
                Hull *hull = itActor->getHull();

				// for each border
				for (std::vector<Border>::iterator it2 = hull->borders.begin(); it2 != hull->borders.end(); it2++)
				{
					Vector2D actorsLocation(itActor->getLocation());
					// if ray have different direction with normal
					if (abs((it2->getNormal().rotation() - (this->endPoint - this->startPoint).rotation()).getValue()) > PI/2)
					{
						// if raytrace intersect this border
						if (this->checkIntersect2Lines(actorsLocation + it2->getA(), actorsLocation + it2->getB(),
							this->startPoint, this->endPoint))
						{
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

IActor* RayTrace::trace(Vector2D *outPoint, Vector2D *outNormal)
{
	IActor *currentActor = NULL;

	// nearest actor that intersects ray
	IActor *nearestActor = NULL;
	// hitpoint of nearest actor
	Vector2D nearestHitPoint(ZERO_VECTOR);
	Vector2D nearestNormal(ZERO_VECTOR);
	float minRayLength = (this->startPoint - this->endPoint).size() + 20.0f;

	// for each actor in the world
	for (std::set<IActor*>::iterator i = this->ownerWorld->allActors.begin(), iEnd = this->ownerWorld->allActors.end(); i != iEnd; i++)
	{
		currentActor = *i;
		if (currentActor->getType() != AT_Light && currentActor->getType() != AT_Special)
		{
			// get bounding box of current actor
			BoundingBox box = currentActor->getBoundingBox();
			// get cohen's code for start point
			byte a = this->getDotCode(&box, &this->startPoint);
			// get cohen's code for end point
			byte b = this->getDotCode(&box, &this->endPoint);

			// if points on some side of BB
			if ((a & b) != 0)
			{
				// go to the next actor
				continue;
			}

			// if raytrace intersect AABB of this actor
			if (((a == 0 || b == 0) && ((a & b) == 0))					// one point is in BB another is out BB
				||
				((a | b) == 3 || (a | b) == 12)							// points on opposite sides of BB // 0011 or 1100 
				||
				this->checkIntersectAABBLine(&box, &this->startPoint, &this->endPoint))	// other cases of intersection
			{
				//return true;
				// get hull of current actor
                Hull *hull = currentActor->getHull();

				// for each border
				for (std::vector<Border>::iterator j = hull->borders.begin(), jEnd = hull->borders.end(); j != jEnd; j++)
				{
					Vector2D actorsLocation(currentActor->getLocation());
					// if ray have different direction with normal
					if (abs((j->getNormal().rotation() - (this->endPoint - this->startPoint).rotation()).getValue()) > PI/2)
					{
						// if raytrace intersect this border
						if (this->checkIntersect2Lines(actorsLocation + j->getA(), actorsLocation + j->getB(), this->startPoint, this->endPoint))
						{
							Vector2D hitLocation = this->getPointIntersect2Lines(actorsLocation + j->getA(), actorsLocation + j->getB(),
								this->startPoint, this->endPoint);
							
							rayLength = (this->startPoint - hitLocation).size();
							// if currentActor nearer than last
							if (rayLength < minRayLength)
							{
								minRayLength = rayLength;
								nearestActor = currentActor;
								nearestHitPoint = hitLocation;
								nearestNormal = j->getNormal();
							}
						}
					}
				}
			}
		}
	}

	// return all values
	if (outPoint != NULL)
	{
		*outPoint = nearestHitPoint;
	}

	if (outNormal != NULL)
	{
		*outNormal = nearestNormal;
	}

	return nearestActor;
}
