#pragma once

#include <string>

using namespace std;
class RoadSignPath
{
public:
	RoadSignPath(void);
	RoadSignPath(string path, string name);
	~RoadSignPath(void);

	string getPath();
	string getName();

private:
	string path;
	string name;
};

