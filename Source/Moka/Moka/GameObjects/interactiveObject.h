#ifndef INTERACTIVE_OBJECT_H
#define INTERACTIVE_OBJECT_H

#include <string>


class InteractiveObject
{
public:
						InteractiveObject(std::string name, std::string type, float x, float y, float width, float height
							, float alternateXCoord1, float alternateXCoord2, float alternateXCoord3
							, float alternateYCoord1, float alternateYCoord2, float alternateYCoord3
							, std::string attachedTo, int beds, float collisionBoxHeight, float collisionBoxWidth
						    , float collisionBoxXCoord, float collisionBoxYCoord
							, std::string color, int nets, int residents, std::string style);
						// ALW - Allow copy construction and assignment

	int					getBeds() const;
	int					getNets() const;
	int					getResidents() const;

	float				getX() const;
	float				getY() const;
	float				getWidth() const;
	float				getHeight() const;
	float				getAlternateXCoord1() const;
	float				getAlternateYCoord1() const;
	float				getAlternateXCoord2() const;
	float				getAlternateYCoord2() const;
	float				getAlternateXCoord3() const;
	float				getAlternateYCoord3() const;
	float               getCollisionBoxXCoord() const;
	float               getCollisionBoxYCoord() const;
	float               getCollisionBoxWidth() const;
	float               getCollisionBoxHeight() const;

	std::string		    getName() const;
	std::string		    getType() const;
	std::string		    getAttachedTo() const;
	std::string		    getColor() const;
	std::string		    getStyle() const;


private:
	std::string			mName;
	std::string			mType;
	float				mX;
	float				mY;
	float				mWidth;
	float				mHeight;
	float				mAlternateXCoord1;
	float				mAlternateXCoord2;
	float				mAlternateXCoord3;
	float				mAlternateYCoord1;
	float				mAlternateYCoord2;
	float				mAlternateYCoord3;
	std::string			mAttachedTo;
	int					mBeds;
	float               mCollisionBoxHeight;
	float               mCollisionBoxWidth;
	float               mCollisionBoxXCoord;
	float               mCollisionBoxYCoord;
	std::string			mColor;
	int					mNets;
	int					mResidents;
	std::string			mStyle;
};

#endif
