#include "houseUpdateNode.h"
#include "../GameObjects/interactiveObject.h"

#include "Trambo/Events/event.h"
#include "Trambo/Events/eventStr.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <cassert>


HouseUpdateNode::HouseUpdateNode(const InteractiveObject &interactiveObject)
: mInteractiveObject(interactiveObject)
, mAddNet1(0x43702f1a)
, mAddNet2(0xc84d3fea)
, mAddNet3(0x68edab82)
, mRemoveNet1(0xe1636e98)
, mRemoveNet2(0xadd9292e)
, mRemoveNet3(0x15f4a6d9)
, mRepairNet1(0xbd7bb606)
, mRepairNet2(0x1b5cf244)
, mRepairNet3(0x2bf40c2c)
, mUnrepairNet1(0x529e6299)
, mUnrepairNet2(0xfeb3945)
, mUnrepairNet3(0x2aac4ae8)
, mBed1()
, mBed2()
, mBed3()
, mSheet1()
, mSheet2()
, mSheet3()
, mNet1()
, mNet2()
, mNet3()
, mDrawBed1(false)
, mDrawBed2(false)
, mDrawBed3(false)
, mDrawNet1(false)
, mDrawNet2(false)
, mDrawNet3(false)
{
	const int bed0 = 0, bed1 = 1, bed2 = 2, bed3 = 3;
	const int totalBeds = mInteractiveObject.getBeds();
	switch (totalBeds)
	{
	case bed0:
		mDrawBed1 = false;
		mDrawBed2 = false;
		mDrawBed3 = false;
		break;
	case bed1:
		mDrawBed1 = true;
		mDrawBed2 = false;
		mDrawBed3 = false;
		break;
	case bed2:
		mDrawBed1 = true;
		mDrawBed2 = true;
		mDrawBed3 = false;
		break;
	case bed3:
		mDrawBed1 = true;
		mDrawBed2 = true;
		mDrawBed3 = true;
		break;
	default:
		assert(("ALW - Logic Error: The interactive object's Beds property is out of range (0-3 are legal)!", false));
	}

	const int net0 = 0, net1 = 1, net2 = 2, net3 = 3;
	const int totalOldNets = mInteractiveObject.getNets();
	switch (totalOldNets)
	{
	case net0:
		mDrawNet1 = false;
		mDrawNet2 = false;
		mDrawNet3 = false;
		break;
	case net1:
		mDrawNet1 = true;
		mDrawNet2 = false;
		mDrawNet3 = false;
		break;
	case net2:
		mDrawNet1 = true;
		mDrawNet2 = true;
		mDrawNet3 = false;
		break;
	case net3:
		mDrawNet1 = true;
		mDrawNet2 = true;
		mDrawNet3 = true;
		break;
	default:
		assert(("ALW - Logic Error: The interactive object's Beds property is out of range (0-3 are legal)!", false));
	}

	// ALW - Bed
	const sf::Vector2f bedSize = sf::Vector2f(20.0f, 10.0f);
	mBed1.setSize(bedSize);
	mBed2.setSize(bedSize);
	mBed3.setSize(bedSize);

	const sf::Vector2f housePosition = sf::Vector2f(mInteractiveObject.getX(), mInteractiveObject.getY());

	switch (mInteractiveObject.getBeds())
	{
	case (OneBed):
		mBed1.setPosition(housePosition.x + mInteractiveObject.getAlternateXCoord1(), housePosition.y + mInteractiveObject.getAlternateYCoord1());
		break;
	case (TwoBeds):
		mBed2.setPosition(housePosition.x + mInteractiveObject.getAlternateXCoord1(), housePosition.y + mInteractiveObject.getAlternateYCoord1());
		mBed1.setPosition(housePosition.x + mInteractiveObject.getAlternateXCoord2(), housePosition.y + mInteractiveObject.getAlternateYCoord2());
		break;
	case (ThreeBeds):
		mBed3.setPosition(housePosition.x + mInteractiveObject.getAlternateXCoord1(), housePosition.y + mInteractiveObject.getAlternateYCoord1());
		mBed2.setPosition(housePosition.x + mInteractiveObject.getAlternateXCoord2(), housePosition.y + mInteractiveObject.getAlternateYCoord2());
		mBed1.setPosition(housePosition.x + mInteractiveObject.getAlternateXCoord3(), housePosition.y + mInteractiveObject.getAlternateYCoord3());
		break;
	default:
		assert(("The total number of beds is out of range!", false));
	}

	mBed1.setFillColor(sf::Color(255u, 255u, 255u, 255u)); // ALW - White "pillow"
	mBed2.setFillColor(sf::Color(255u, 255u, 255u, 255u)); // ALW - White "pillow"
	mBed3.setFillColor(sf::Color(255u, 255u, 255u, 255u)); // ALW - White "pillow"

	mBed1.setOutlineThickness(1.0f);
	mBed2.setOutlineThickness(1.0f);
	mBed3.setOutlineThickness(1.0f);
	mBed1.setOutlineColor(sf::Color(0u, 0u, 0u, 255u));
	mBed2.setOutlineColor(sf::Color(0u, 0u, 0u, 255u));
	mBed3.setOutlineColor(sf::Color(0u, 0u, 0u, 255u));

	// ALW - Sheets
	const sf::Vector2f sheetSize = sf::Vector2f(16.0f, 10.0f);
	mSheet1.setSize(sheetSize);
	mSheet2.setSize(sheetSize);
	mSheet3.setSize(sheetSize);

	const float offset = 4.0f;

	switch (mInteractiveObject.getBeds())
	{
	case (OneBed):
		mSheet1.setPosition(housePosition.x + mInteractiveObject.getAlternateXCoord1() + offset, housePosition.y + mInteractiveObject.getAlternateYCoord1());
		break;
	case (TwoBeds):
		mSheet2.setPosition(housePosition.x + mInteractiveObject.getAlternateXCoord1() + offset, housePosition.y + mInteractiveObject.getAlternateYCoord1());
		mSheet1.setPosition(housePosition.x + mInteractiveObject.getAlternateXCoord2() + offset, housePosition.y + mInteractiveObject.getAlternateYCoord2());
		break;
	case (ThreeBeds):
		mSheet3.setPosition(housePosition.x + mInteractiveObject.getAlternateXCoord1() + offset, housePosition.y + mInteractiveObject.getAlternateYCoord1());
		mSheet2.setPosition(housePosition.x + mInteractiveObject.getAlternateXCoord2() + offset, housePosition.y + mInteractiveObject.getAlternateYCoord2());
		mSheet1.setPosition(housePosition.x + mInteractiveObject.getAlternateXCoord3() + offset, housePosition.y + mInteractiveObject.getAlternateYCoord3());
		break;
	default:
		assert(("The total number of beds is out of range!", false));
	}

	mSheet1.setFillColor(sf::Color(128u, 128u, 128u, 255u)); // ALW - Grey
	mSheet2.setFillColor(sf::Color(128u, 128u, 128u, 255u)); // ALW - Grey
	mSheet3.setFillColor(sf::Color(128u, 128u, 128u, 255u)); // ALW - Grey

	// ALW - Net
	const sf::Vector2f netSize = sf::Vector2f(20.0f, 10.0f);
	mNet1.setSize(netSize);
	mNet2.setSize(netSize);
	mNet3.setSize(netSize);

	switch (mInteractiveObject.getBeds())
	{
	case (OneBed) :
		mNet1.setPosition(housePosition.x + mInteractiveObject.getAlternateXCoord1(), housePosition.y + mInteractiveObject.getAlternateYCoord1());
		break;
	case (TwoBeds) :
		mNet2.setPosition(housePosition.x + mInteractiveObject.getAlternateXCoord1(), housePosition.y + mInteractiveObject.getAlternateYCoord1());
		mNet1.setPosition(housePosition.x + mInteractiveObject.getAlternateXCoord2(), housePosition.y + mInteractiveObject.getAlternateYCoord2());
		break;
	case (ThreeBeds) :
		mNet3.setPosition(housePosition.x + mInteractiveObject.getAlternateXCoord1(), housePosition.y + mInteractiveObject.getAlternateYCoord1());
		mNet2.setPosition(housePosition.x + mInteractiveObject.getAlternateXCoord2(), housePosition.y + mInteractiveObject.getAlternateYCoord2());
		mNet1.setPosition(housePosition.x + mInteractiveObject.getAlternateXCoord3(), housePosition.y + mInteractiveObject.getAlternateYCoord3());
		break;
	default:
		assert(("The total number of beds is out of range!", false));
	}

	mNet1.setFillColor(sf::Color(255u, 0u, 0u, 150u));	// ALW - Transparent red
	mNet2.setFillColor(sf::Color(255u, 0u, 0u, 150u));	// ALW - Transparent red
	mNet3.setFillColor(sf::Color(255u, 0u, 0u, 150u));	// ALW - Transparent red

	mNet1.setOutlineThickness(1.0f);
	mNet2.setOutlineThickness(1.0f);
	mNet3.setOutlineThickness(1.0f);
	mNet1.setOutlineColor(sf::Color(0u, 0u, 0u, 255u));
	mNet2.setOutlineColor(sf::Color(0u, 0u, 0u, 255u));
	mNet3.setOutlineColor(sf::Color(0u, 0u, 0u, 255u));
}

void HouseUpdateNode::handleEvent(const trmb::Event &gameEvent)
{
	if (mAddNet1 == gameEvent.getType())
	{
		// ALW - Incidentally confirmed event is an EventStr. Downcasting is safe.
		const trmb::EventStr &eventStr = static_cast<const trmb::EventStr &>(gameEvent);

		if (mInteractiveObject.getName() == eventStr.getString())
		{
			mDrawNet1 = true;
			mNet1.setFillColor(sf::Color(0u, 255u, 0u, 150u));	// ALW - Transparent green
		}
	}
	else if (mAddNet2 == gameEvent.getType())
	{
		// ALW - Incidentally confirmed event is an EventStr. Downcasting is safe.
		const trmb::EventStr &eventStr = static_cast<const trmb::EventStr &>(gameEvent);

		if (mInteractiveObject.getName() == eventStr.getString())
		{
			mDrawNet2 = true;
			mNet2.setFillColor(sf::Color(0u, 255u, 0u, 150u));	// ALW - Transparent green
		}
	}
	else if (mAddNet3 == gameEvent.getType())
	{
		// ALW - Incidentally confirmed event is an EventStr. Downcasting is safe.
		const trmb::EventStr &eventStr = static_cast<const trmb::EventStr &>(gameEvent);

		if (mInteractiveObject.getName() == eventStr.getString())
		{
			mDrawNet3 = true;
			mNet3.setFillColor(sf::Color(0u, 255u, 0u, 150u));	// ALW - Transparent green
		}
	}
	else if (mRemoveNet1 == gameEvent.getType())
	{
		// ALW - Incidentally confirmed event is an EventStr. Downcasting is safe.
		const trmb::EventStr &eventStr = static_cast<const trmb::EventStr &>(gameEvent);

		if (mInteractiveObject.getName() == eventStr.getString())
		{
			mDrawNet1 = false;
		}
	}
	else if (mRemoveNet2 == gameEvent.getType())
	{
		// ALW - Incidentally confirmed event is an EventStr. Downcasting is safe.
		const trmb::EventStr &eventStr = static_cast<const trmb::EventStr &>(gameEvent);

		if (mInteractiveObject.getName() == eventStr.getString())
		{
			mDrawNet2 = false;
		}
	}
	else if (mRemoveNet3 == gameEvent.getType())
	{
		// ALW - Incidentally confirmed event is an EventStr. Downcasting is safe.
		const trmb::EventStr &eventStr = static_cast<const trmb::EventStr &>(gameEvent);

		if (mInteractiveObject.getName() == eventStr.getString())
		{
			mDrawNet3 = false;
		}
	}
	else if (mRepairNet1 == gameEvent.getType())
	{
		// ALW - Incidentally confirmed event is an EventStr. Downcasting is safe.
		const trmb::EventStr &eventStr = static_cast<const trmb::EventStr &>(gameEvent);

		if (mInteractiveObject.getName() == eventStr.getString())
		{
			mNet1.setFillColor(sf::Color(0u, 255u, 0u, 150u));	// ALW - Transparent green
		}
	}
	else if (mRepairNet2 == gameEvent.getType())
	{
		// ALW - Incidentally confirmed event is an EventStr. Downcasting is safe.
		const trmb::EventStr &eventStr = static_cast<const trmb::EventStr &>(gameEvent);

		if (mInteractiveObject.getName() == eventStr.getString())
		{
			mNet2.setFillColor(sf::Color(0u, 255u, 0u, 150u));	// ALW - Transparent green
		}
	}
	else if (mRepairNet3 == gameEvent.getType())
	{
		// ALW - Incidentally confirmed event is an EventStr. Downcasting is safe.
		const trmb::EventStr &eventStr = static_cast<const trmb::EventStr &>(gameEvent);

		if (mInteractiveObject.getName() == eventStr.getString())
		{
			mNet3.setFillColor(sf::Color(0u, 255u, 0u, 150u));	// ALW - Transparent green
		}
	}
	else if (mUnrepairNet1 == gameEvent.getType())
	{
		// ALW - Incidentally confirmed event is an EventStr. Downcasting is safe.
		const trmb::EventStr &eventStr = static_cast<const trmb::EventStr &>(gameEvent);

		if (mInteractiveObject.getName() == eventStr.getString())
		{
			mNet1.setFillColor(sf::Color(255u, 0u, 0u, 150u));	// ALW - Transparent red
		}
	}
	else if (mUnrepairNet2 == gameEvent.getType())
	{
		// ALW - Incidentally confirmed event is an EventStr. Downcasting is safe.
		const trmb::EventStr &eventStr = static_cast<const trmb::EventStr &>(gameEvent);

		if (mInteractiveObject.getName() == eventStr.getString())
		{
			mNet2.setFillColor(sf::Color(255u, 0u, 0u, 150u));	// ALW - Transparent red
		}
	}
	else if (mUnrepairNet3 == gameEvent.getType())
	{
		// ALW - Incidentally confirmed event is an EventStr. Downcasting is safe.
		const trmb::EventStr &eventStr = static_cast<const trmb::EventStr &>(gameEvent);

		if (mInteractiveObject.getName() == eventStr.getString())
		{
			mNet3.setFillColor(sf::Color(255u, 0u, 0u, 150u));	// ALW - Transparent red
		}
	}
}

void HouseUpdateNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (mDrawBed1)
	{
		target.draw(mBed1, states);
		target.draw(mSheet1, states);
		if (mDrawNet1)
			target.draw(mNet1, states);
	}

	if (mDrawBed2)
	{
		target.draw(mBed2, states);
		target.draw(mSheet2, states);
		if (mDrawNet2)
			target.draw(mNet2, states);
	}

	if (mDrawBed3)
	{
		target.draw(mBed3, states);
		target.draw(mSheet3, states);
		if (mDrawNet3)
			target.draw(mNet3, states);
	}
}
