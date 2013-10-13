#ifndef HERO_H
#define HERO_H

#include "..\src\BaseProperties.h"

class Hero
{
private:
	/** ��������� ������ � ������� ������������" */
	Vector2D Location;
	/** ����� � ������� �������� ����� */
	Vector2D NextLocation;
	/** �������� ����������� ����� � ������ */
	float Speed;
public:
	Hero(void);
	virtual ~Hero(void);

	/** ���������� ���������������� ��� ����� �������� */
	void Process(int deltatime);

	/** ���������� ����� ��������� ����� � ������� ������������ */
	void SetLocation(Vector2D location);
	/** �������� ��������� ����� */
	Vector2D GetLocation();
	/** ��������� ����� ������������ � ����� ������� */
	void MoveTo(Vector2D newLocation);
};

#endif