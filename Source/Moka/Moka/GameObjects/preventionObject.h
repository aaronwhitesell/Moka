#ifndef PREVENTION_OBJECT_H
#define PREVENTION_OBJECT_H

#include <string>


class PreventionObject
{
							friend class PreventionNode;

public:
							PreventionObject(std::string name, std::string type, int x, int y, int width, int height);
							// ALW - Allow copy construction and assignment


private:
	std::string				mName;
	std::string				mType;
	int						mX;
	int						mY;
	int						mWidth;
	int						mHeight;
};

#endif
