#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "../Core/Vector2D.h"
/**
 * Axis-aligned bounding box (AABB)
 */
class BoundingBox
{
public:
	BoundingBox(float minX, float minY, float maxX, float maxY);
	BoundingBox(Vector2D minPoint, Vector2D maxPoint);
	~BoundingBox(void);
	/** Left-top point */
	Vector2D getFirst();
	/** Right-top point */
	Vector2D getSecond();
	/** Right-bottom point */
	Vector2D getThird();
	/** Left-bottom point */
	Vector2D getFourth();
	/** X-pos of left border */
	float minX;
	/** Y-pos of top border */
	float minY;
	/** X-pos of right border */
	float maxX;
	/** Y-pos of bottom border */
	float maxY;
};

#endif
