#pragma once

#include <string>

using std::string;

class ITile {
public:
	string getName();
	void update(float dt);
	
	
	
protected:
	
	string mName;
};
