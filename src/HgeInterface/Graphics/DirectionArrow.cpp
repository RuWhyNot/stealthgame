#include "DirectionArrow.h"


DirectionArrow::DirectionArrow(HGE *hge) : centerLocation(ZERO_VECTOR),
	direction(0.0f)
{
	this->hge = hge;

	this->arrowTexture = this->hge->Texture_Load("arrow.png");
		
	this->arrowSprite = new hgeSprite(this->arrowTexture, 0, 0, 32, 32);
	this->arrowSprite->SetColor(0xFF00AA00);
	this->arrowSprite->SetHotSpot(16, 16);

	bDrawable = false;
}

DirectionArrow::~DirectionArrow(void)
{
	delete this->arrowSprite;
	this->hge->Texture_Free(this->arrowTexture);
}

void DirectionArrow::setScreenLocation(const Vector2D& scrLocation)
{
	this->centerLocation = scrLocation;
}

void DirectionArrow::setDirection(Rotator direction)
{
	this->bDrawable = true;
	this->direction = direction;
}


void DirectionArrow::setVDirection(Vector2D vectDirection)
{
	if (vectDirection != ZERO_VECTOR)
	{
		this->bDrawable = true;
		this->direction = vectDirection.rotation();
	}
	else
	{
		bDrawable = false;
	}
}

void DirectionArrow::setColor(DWORD color)
{
	this->arrowSprite->SetColor(color);
}

void DirectionArrow::render() const
{
	if (!this->bDrawable)
		return;
	
	Vector2D location = this->centerLocation + Vector2D(this->direction) * 50;
	this->arrowSprite->RenderEx(location.x, location.y, this->direction.getValue());
}

Vector2D DirectionArrow::getScreenLocation() const
{
	return this->centerLocation;
}

bool DirectionArrow::click()
{
	return false;
}
