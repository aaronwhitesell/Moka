#ifndef INTERACTIVE_OBJECT_H
#define INTERACTIVE_OBJECT_H

#include <string>


class InteractiveObject
{
public:
							InteractiveObject(std::string name, std::string type, std::string attachedTo, float x, float y, float width, float height);
							// ALW - Allow copy construction and assignment

	const std::string &		getName() const;
	const std::string &		getType() const;
	const std::string &		getAttachedTo() const;
	float					getX() const;
	float					getY() const;
	float					getWidth() const;
	float					getHeight() const;


private:
	std::string				mName;
	std::string				mType;
	std::string				mAttachedTo;
	float					mX;
	float					mY;
	float					mWidth;
	float					mHeight;
};

#endif
