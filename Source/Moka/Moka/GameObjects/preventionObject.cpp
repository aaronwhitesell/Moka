#include "preventionObject.h"


PreventionObject::PreventionObject(std::string name, std::string type, std::string attachedTo, int x, int y, int width, int height)
: mName(name)
, mType(type)
, mAttachedTo(attachedTo)
, mX(x)
, mY(y)
, mWidth(width)
, mHeight(height)
{
}

const std::string & PreventionObject::getName() const
{
	return mName;
}

const std::string & PreventionObject::getType() const
{
	return mType;
}

const std::string & PreventionObject::getAttachedTo() const
{
	return mAttachedTo;
}

int PreventionObject::getX() const
{
	return mX;
}

int PreventionObject::getY() const
{
	return mY;
}

int PreventionObject::getWidth() const
{
	return mWidth;
}

int PreventionObject::getHeight() const
{
	return mHeight;
}
