#include "interactiveObject.h"


InteractiveObject::InteractiveObject(std::string name, std::string type, std::string attachedTo, float x, float y, float width, float height)
: mName(name)
, mType(type)
, mAttachedTo(attachedTo)
, mX(x)
, mY(y)
, mWidth(width)
, mHeight(height)
{
}

const std::string & InteractiveObject::getName() const
{
	return mName;
}

const std::string & InteractiveObject::getType() const
{
	return mType;
}

const std::string & InteractiveObject::getAttachedTo() const
{
	return mAttachedTo;
}

float InteractiveObject::getX() const
{
	return mX;
}

float InteractiveObject::getY() const
{
	return mY;
}

float InteractiveObject::getWidth() const
{
	return mWidth;
}

float InteractiveObject::getHeight() const
{
	return mHeight;
}
