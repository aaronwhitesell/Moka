#include "interactiveObject.h"


InteractiveObject::InteractiveObject(std::string name, std::string type, std::string attachedTo, int x, int y, int width, int height)
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

int InteractiveObject::getX() const
{
	return mX;
}

int InteractiveObject::getY() const
{
	return mY;
}

int InteractiveObject::getWidth() const
{
	return mWidth;
}

int InteractiveObject::getHeight() const
{
	return mHeight;
}
