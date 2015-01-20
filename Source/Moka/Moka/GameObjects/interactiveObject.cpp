#include "interactiveObject.h"


InteractiveObject::InteractiveObject(std::string name, std::string type, float x, float y, float width, float height
	, float alternateXCoord1, float alternateXCoord2, float alternateXCoord3, float alternateYCoord1, float alternateYCoord2
	, float alternateYCoord3, std::string attachedTo, int beds, float collisionBoxHeight, float collisionBoxWidth
	, float collisionBoxXCoord, float collisionBoxYCoord, std::string color, int nets, int residents, std::string style)
: mName(name)
, mType(type)
, mX(x)
, mY(y)
, mWidth(width)
, mHeight(height)
, mAlternateXCoord1(alternateXCoord1)
, mAlternateXCoord2(alternateXCoord2)
, mAlternateXCoord3(alternateXCoord3)
, mAlternateYCoord1(alternateYCoord1)
, mAlternateYCoord2(alternateYCoord2)
, mAlternateYCoord3(alternateYCoord3)
, mAttachedTo(attachedTo)
, mBeds(beds)
, mCollisionBoxXCoord(collisionBoxXCoord)
, mCollisionBoxYCoord(collisionBoxYCoord)
, mCollisionBoxWidth(collisionBoxWidth)
, mCollisionBoxHeight(collisionBoxHeight)
, mColor(color)
, mNets(nets)
, mResidents(residents)
, mStyle(style)
{
}

int InteractiveObject::getBeds() const
{
	return mBeds;
}

int InteractiveObject::getNets() const
{
	return mNets;
}

int InteractiveObject::getResidents() const
{
	return mResidents;
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

float InteractiveObject::getAlternateXCoord1() const
{
	return mAlternateXCoord1;
}

float InteractiveObject::getAlternateYCoord1() const
{
	return mAlternateYCoord1;
}

float InteractiveObject::getAlternateXCoord2() const
{
	return mAlternateXCoord2;
}

float InteractiveObject::getAlternateYCoord2() const
{
	return mAlternateYCoord2;
}

float InteractiveObject::getAlternateXCoord3() const
{
	return mAlternateXCoord3;
}

float InteractiveObject::getAlternateYCoord3() const
{
	return mAlternateYCoord3;
}

float InteractiveObject::getCollisionBoxXCoord() const
{
	return mCollisionBoxXCoord;
}

float InteractiveObject::getCollisionBoxYCoord() const
{
	return mCollisionBoxYCoord;
}

float InteractiveObject::getCollisionBoxWidth() const
{
	return mCollisionBoxWidth;
}

float InteractiveObject::getCollisionBoxHeight() const
{
	return mCollisionBoxHeight;
}

std::string InteractiveObject::getName() const
{
	return mName;
}

std::string InteractiveObject::getType() const
{
	return mType;
}

std::string InteractiveObject::getAttachedTo() const
{
	return mAttachedTo;
}

std::string InteractiveObject::getColor() const
{
	return mColor;
}

std::string InteractiveObject::getStyle() const
{
	return mStyle;
}
