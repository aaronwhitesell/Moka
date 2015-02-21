#include "residentUpdateNode.h"
#include "houseNode.h"
#include "../GameObjects/interactiveObject.h"

#include "Trambo/Events/event.h"
#include "Trambo/Events/eventStr.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <cassert>


ResidentUpdateNode::ResidentUpdateNode(int residentID, const HouseNode * const houseNode)
: mCured0(0xfedc3227)
, mCured1(0x72b8b78e)
, mCured2(0x4f0b0c7a)
, mCured3(0x56549eca)
, mCured4(0xee2c7729)
, mCured5(0x58acae8)
, mCured6(0x3dadc569)
, mHasMalaria0(0xfd04c534)
, mHasMalaria1(0x9e203fe1)
, mHasMalaria2(0xe146faa8)
, mHasMalaria3(0xd05a9101)
, mHasMalaria4(0x7809a6c)
, mHasMalaria5(0x8f0a4af3)
, mHasMalaria6(0xc9bc4d1)
, mResidentID(residentID)
, mHouseNode(houseNode)
, mSkin(sf::Color(173u, 145u, 89u, 255u))
, mRed(sf::Color(204u, 24u, 32u, 255u))
, mUI(5.0f, 30u)
{
	const float x = houseNode->getInteractiveObject().getX();
	const float y = houseNode->getInteractiveObject().getY();
	setPosition(sf::Vector2f(x, y));

	mUI.setFillColor(mSkin);
	mUI.setOutlineColor(sf::Color(0u, 0u, 0u, 255u));
	mUI.setOutlineThickness(1.0f);
	setUIPosition();
}

void ResidentUpdateNode::handleEvent(const trmb::Event &gameEvent)
{

	if (mResidentID == Resident0 && mCured0 == gameEvent.getType())
	{
		// ALW - Incidentally confirmed event is an EventStr. Downcasting is safe.
		const trmb::EventStr &eventStr = static_cast<const trmb::EventStr &>(gameEvent);

		if (mHouseNode->getInteractiveObject().getName() == eventStr.getString())
		{
			mUI.setFillColor(mSkin);
		}
	}
	else if (mResidentID == Resident0 && mHasMalaria0 == gameEvent.getType())
	{
		// ALW - Incidentally confirmed event is an EventStr. Downcasting is safe.
		const trmb::EventStr &eventStr = static_cast<const trmb::EventStr &>(gameEvent);

		if (mHouseNode->getInteractiveObject().getName() == eventStr.getString())
		{
			mUI.setFillColor(mRed);
		}
	}
	else if (mResidentID == Resident1 && mCured1 == gameEvent.getType())
	{
		// ALW - Incidentally confirmed event is an EventStr. Downcasting is safe.
		const trmb::EventStr &eventStr = static_cast<const trmb::EventStr &>(gameEvent);

		if (mHouseNode->getInteractiveObject().getName() == eventStr.getString())
		{
			mUI.setFillColor(mSkin);
		}
	}
	else if (mResidentID == Resident1 && mHasMalaria1 == gameEvent.getType())
	{
		// ALW - Incidentally confirmed event is an EventStr. Downcasting is safe.
		const trmb::EventStr &eventStr = static_cast<const trmb::EventStr &>(gameEvent);

		if (mHouseNode->getInteractiveObject().getName() == eventStr.getString())
		{
			mUI.setFillColor(mRed);
		}
	}
	else if (mResidentID == Resident2 && mCured2 == gameEvent.getType())
	{
		// ALW - Incidentally confirmed event is an EventStr. Downcasting is safe.
		const trmb::EventStr &eventStr = static_cast<const trmb::EventStr &>(gameEvent);

		if (mHouseNode->getInteractiveObject().getName() == eventStr.getString())
		{
			mUI.setFillColor(mSkin);
		}
	}
	else if (mResidentID == Resident2 && mHasMalaria2 == gameEvent.getType())
	{
		// ALW - Incidentally confirmed event is an EventStr. Downcasting is safe.
		const trmb::EventStr &eventStr = static_cast<const trmb::EventStr &>(gameEvent);

		if (mHouseNode->getInteractiveObject().getName() == eventStr.getString())
		{
			mUI.setFillColor(mRed);
		}
	}
	else if (mResidentID == Resident3 && mCured3 == gameEvent.getType())
	{
		// ALW - Incidentally confirmed event is an EventStr. Downcasting is safe.
		const trmb::EventStr &eventStr = static_cast<const trmb::EventStr &>(gameEvent);

		if (mHouseNode->getInteractiveObject().getName() == eventStr.getString())
		{
			mUI.setFillColor(mSkin);
		}
	}
	else if (mResidentID == Resident3 && mHasMalaria3 == gameEvent.getType())
	{
		// ALW - Incidentally confirmed event is an EventStr. Downcasting is safe.
		const trmb::EventStr &eventStr = static_cast<const trmb::EventStr &>(gameEvent);

		if (mHouseNode->getInteractiveObject().getName() == eventStr.getString())
		{
			mUI.setFillColor(mRed);
		}
	}
	else if (mResidentID == Resident4 && mCured4 == gameEvent.getType())
	{
		// ALW - Incidentally confirmed event is an EventStr. Downcasting is safe.
		const trmb::EventStr &eventStr = static_cast<const trmb::EventStr &>(gameEvent);

		if (mHouseNode->getInteractiveObject().getName() == eventStr.getString())
		{
			mUI.setFillColor(mSkin);
		}
	}
	else if (mResidentID == Resident4 && mHasMalaria4 == gameEvent.getType())
	{
		// ALW - Incidentally confirmed event is an EventStr. Downcasting is safe.
		const trmb::EventStr &eventStr = static_cast<const trmb::EventStr &>(gameEvent);

		if (mHouseNode->getInteractiveObject().getName() == eventStr.getString())
		{
			mUI.setFillColor(mRed);
		}
	}
	else if (mResidentID == Resident5 && mCured5 == gameEvent.getType())
	{
		// ALW - Incidentally confirmed event is an EventStr. Downcasting is safe.
		const trmb::EventStr &eventStr = static_cast<const trmb::EventStr &>(gameEvent);

		if (mHouseNode->getInteractiveObject().getName() == eventStr.getString())
		{
			mUI.setFillColor(mSkin);
		}
	}
	else if (mResidentID == Resident5 && mHasMalaria5 == gameEvent.getType())
	{
		// ALW - Incidentally confirmed event is an EventStr. Downcasting is safe.
		const trmb::EventStr &eventStr = static_cast<const trmb::EventStr &>(gameEvent);

		if (mHouseNode->getInteractiveObject().getName() == eventStr.getString())
		{
			mUI.setFillColor(mRed);
		}
	}
	else if (mResidentID == Resident6 && mCured6 == gameEvent.getType())
	{
		// ALW - Incidentally confirmed event is an EventStr. Downcasting is safe.
		const trmb::EventStr &eventStr = static_cast<const trmb::EventStr &>(gameEvent);

		if (mHouseNode->getInteractiveObject().getName() == eventStr.getString())
		{
			mUI.setFillColor(mSkin);
		}
	}
	else if (mResidentID == Resident6 && mHasMalaria6 == gameEvent.getType())
	{
		// ALW - Incidentally confirmed event is an EventStr. Downcasting is safe.
		const trmb::EventStr &eventStr = static_cast<const trmb::EventStr &>(gameEvent);

		if (mHouseNode->getInteractiveObject().getName() == eventStr.getString())
		{
			mUI.setFillColor(mRed);
		}
	}
}

void ResidentUpdateNode::setUIPosition()
{
	const float yResidentOffset = 15.0f;
	const float xUI = mHouseNode->getInteractiveObject().getAlternateXCoord1();
	float yUI = mHouseNode->getInteractiveObject().getAlternateYCoord1();

	switch (mResidentID)
	{
	case Resident0:
		// ALW - No offset
		break;
	case Resident1:
		yUI += yResidentOffset;
		break;
	case Resident2:
		yUI += yResidentOffset * 2.0f;
		break;
	case Resident3:
		yUI += yResidentOffset * 3.0f;
		break;
	case Resident4:
		yUI += yResidentOffset * 4.0f;
		break;
	case Resident5:
		yUI += yResidentOffset * 5.0f;
		break;
	case Resident6:
		yUI += yResidentOffset * 6.0f;
		break;
	default:
		assert(("The resident ID is out of range!", false));
	}

	mUI.setPosition(sf::Vector2f(xUI, yUI));
}

void ResidentUpdateNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(mUI, states);
}
