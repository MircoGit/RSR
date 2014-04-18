#include "stdafx.h"
#include "RoadSign.h"


RoadSign::RoadSign(void)
{
}

RoadSign::RoadSign(Vec3f circle, string name)
{
	this->circle = circle;
	this->name = name;
}

RoadSign::~RoadSign(void)
{
}

Vec3f RoadSign::getCircle()
{
	return this->circle;
}

string RoadSign::getName()
{
	return this->name;
}
