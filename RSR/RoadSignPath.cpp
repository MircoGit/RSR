#include "stdafx.h"
#include "RoadSignPath.h"


RoadSignPath::RoadSignPath(void)
{
}

RoadSignPath::RoadSignPath(string path, string name)
{
	this->path = path;
	this->name = name;
}

RoadSignPath::~RoadSignPath(void)
{
}

string RoadSignPath::getPath()
{	
	return this->path;
}

string RoadSignPath::getName()
{	
	return this->name;
}