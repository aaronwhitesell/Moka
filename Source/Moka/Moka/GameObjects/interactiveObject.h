#ifndef INTERACTIVE_OBJECT_H
#define INTERACTIVE_OBJECT_H

#include <string>


class InteractiveObject
{
public:
							InteractiveObject(std::string name, std::string type, float x, float y, float width, float height
								, float posX0, float posY0, float posX1, float posY1, float posX2, float posY2, std::string attachedTo
								, int beds, std::string color, int nets, int residents, std::string style);
							// ALW - Allow copy construction and assignment

	const std::string		getName() const;
	const std::string		getType() const;
	float					getX() const;
	float					getY() const;
	float					getWidth() const;
	float					getHeight() const;

	float					getPosX0() const;
	float					getPosY0() const;
	float					getPosX1() const;
	float					getPosY1() const;
	float					getPosX2() const;
	float					getPosY2() const;
	const std::string		getAttachedTo() const;
	int						getBeds() const;
	const std::string		getColor() const;
	int						getNets() const;
	int						getResidents() const;
	const std::string		getStyle() const;


private:
	std::string				mName;
	std::string				mType;
	float					mX;
	float					mY;
	float					mWidth;
	float					mHeight;

	float					mPosX0;
	float					mPosY0;
	float					mPosX1;
	float					mPosY1;
	float					mPosX2;
	float					mPosY2;
	std::string				mAttachedTo;
	int						mBeds;
	std::string				mColor;
	int						mNets;
	int						mResidents;
	std::string				mStyle;


};

#endif
