#ifndef HERO_H
#define HERO_H

#include "BaseProperties.h"

class Hero
{
private:
	/** ��������� ������ � ������� ������������" */
	Vector Location;
	/** ����� � ������� �������� ����� */
	Vector NextLocation;
	/** �������� ����������� ����� � ������ */
	float Speed;
public:
	Hero(void);
	virtual ~Hero(void);

	/** ���������� ���������������� ��� ����� �������� */
	void Process(int deltatime);

	/** ���������� ����� ��������� ����� � ������� ������������ */
	void SetLocation(Vector location);
	/** �������� ��������� ����� */
	Vector GetLocation();
	/** ��������� ����� ������������ � ����� ������� */
	void MoveTo(Vector newLocation);
};

#endif