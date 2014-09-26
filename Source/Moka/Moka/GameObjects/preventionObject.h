#ifndef PREVENTION_OBJECT_H
#define PREVENTION_OBJECT_H

#include <string>


class PreventionObject
{
public:
							PreventionObject(std::string name, std::string type, std::string attachedTo, int x, int y, int width, int height);
							// ALW - Allow copy construction and assignment

	const std::string &		getName() const;
	const std::string &		getType() const;
	const std::string &		getAttachedTo() const;
	int						getX() const;
	int						getY() const;
	int						getWidth() const;
	int						getHeight() const;


private:
	std::string				mName;
	std::string				mType;
	std::string				mAttachedTo;
	int						mX;
	int						mY;
	int						mWidth;
	int						mHeight;
};

#endif
