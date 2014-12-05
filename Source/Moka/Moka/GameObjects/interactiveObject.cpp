#include "interactiveObject.h"


InteractiveObject::InteractiveObject(std::string name, std::string type, float x, float y, float width, float height
	, float posX0, float posY0, float posX1, float posY1, std::string attachedTo, std::string size, std::string style)
: mName(name)
, mType(type)
, mX(x)
, mY(y)
, mWidth(width)
, mHeight(height)
, mPosX0(posX0)
, mPosY0(posY0)
, mPosX1(posX1)
, mPosY1(posY1)
, mAttachedTo(attachedTo)
, mSize(size)
, mStyle(style)
{
}

const std::string InteractiveObject::getName() const
{
	return mName;
}

const std::string InteractiveObject::getType() const
{
	return mType;
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

float InteractiveObject::getPosX0() const
{
	return mPosX0;
}

float InteractiveObject::getPosY0() const
{
	return mPosY0;
}

float InteractiveObject::getPosX1() const
{
	return mPosX1;
}

float InteractiveObject::getPosY1() const
{
	return mPosY1;
}

const std::string InteractiveObject::getAttachedTo() const
{
	return mAttachedTo;
}

const std::string InteractiveObject::getSize() const
{
	return mSize;
}

const std::string InteractiveObject::getStyle() const
{
	return mStyle;
}
