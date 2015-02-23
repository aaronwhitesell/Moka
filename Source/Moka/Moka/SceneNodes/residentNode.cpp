#include "residentNode.h"
#include "houseNode.h"
#include "../GameObjects/interactiveObject.h"

#include "Trambo/Events/event.h"

#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include <cassert>
#include <cmath>
#include <random>


ResidentNode::ResidentNode(int residentID, bool hasMalaria, const HouseNode * const houseNode)
: mBeginSimulationEvent(0x5000e550)
, mCured0(0xfedc3227, houseNode->getInteractiveObject().getName())
, mCured1(0x72b8b78e, houseNode->getInteractiveObject().getName())
, mCured2(0x4f0b0c7a, houseNode->getInteractiveObject().getName())
, mCured3(0x56549eca, houseNode->getInteractiveObject().getName())
, mCured4(0xee2c7729, houseNode->getInteractiveObject().getName())
, mCured5(0x58acae8,  houseNode->getInteractiveObject().getName())
, mCured6(0x3dadc569, houseNode->getInteractiveObject().getName())
, mHasMalaria0(0xfd04c534, houseNode->getInteractiveObject().getName())
, mHasMalaria1(0x9e203fe1, houseNode->getInteractiveObject().getName())
, mHasMalaria2(0xe146faa8, houseNode->getInteractiveObject().getName())
, mHasMalaria3(0xd05a9101, houseNode->getInteractiveObject().getName())
, mHasMalaria4(0x7809a6c,  houseNode->getInteractiveObject().getName())
, mHasMalaria5(0x8f0a4af3, houseNode->getInteractiveObject().getName())
, mHasMalaria6(0xc9bc4d1,  houseNode->getInteractiveObject().getName())
, mResidentID(residentID)
, mHouseNode(houseNode)
, mHasMalaria(hasMalaria)
, mHasBedNet(false)
, mIsRepaired(false)
{
	generateSpawnPosition(houseNode->getCollisionBox());
}

sf::FloatRect ResidentNode::getBoundingRect() const
{
	const float tileWidth = 64.0f;
	const float tileHeight = 64.0f;

	return sf::FloatRect(getPosition().x, getPosition().y, tileWidth, tileHeight);
}

void ResidentNode::updateCurrent(sf::Time dt)
{
}

void ResidentNode::handleEvent(const trmb::Event &gameEvent)
{
	if (mBeginSimulationEvent == gameEvent.getType())
	{
		if (mHasMalaria)
			sendMalariaMsg();
	}
}

void ResidentNode::sendCureMsg()
{
	switch (mResidentID)
	{
	case Resident0:
		sendEvent(mCured0);
		break;
	case Resident1:
		sendEvent(mCured1);
		break;
	case Resident2:
		sendEvent(mCured2);
		break;
	case Resident3:
		sendEvent(mCured3);
		break;
	case Resident4:
		sendEvent(mCured4);
		break;
	case Resident5:
		sendEvent(mCured5);
		break;
	case Resident6:
		sendEvent(mCured6);
		break;
	default:
		assert(("The resident ID is out of range!", false));
	}
}

void ResidentNode::sendMalariaMsg()
{
	switch (mResidentID)
	{
	case Resident0:
		sendEvent(mHasMalaria0);
		break;
	case Resident1:
		sendEvent(mHasMalaria1);
		break;
	case Resident2:
		sendEvent(mHasMalaria2);
		break;
	case Resident3:
		sendEvent(mHasMalaria3);
		break;
	case Resident4:
		sendEvent(mHasMalaria4);
		break;
	case Resident5:
		sendEvent(mHasMalaria5);
		break;
	case Resident6:
		sendEvent(mHasMalaria6);
		break;
	default:
		assert(("The resident ID is out of range!", false));
	}
}

void ResidentNode::generateSpawnPosition(sf::FloatRect houseBoundingRect)
{
	std::random_device					randomDevice;
	std::mt19937						generator(randomDevice());
	std::uniform_int_distribution<>		distribution;

	const float tileWidth = 64.0f;
	assert(("The width of the house should divide evenly with the tileWidth!"
		, std::floor(houseBoundingRect.width / tileWidth) == houseBoundingRect.width / tileWidth));
	const int columns = static_cast<int>(houseBoundingRect.width / tileWidth);

	decltype(distribution.param()) newWidthRange(0, columns - 1);
	distribution.param(newWidthRange);
	const int column = distribution(generator);

	const float tileHeight = 64.0f;
	assert(("The height of the house should divide evenly with the tileHeight!"
		, std::floor(houseBoundingRect.height / tileHeight) == houseBoundingRect.height / tileHeight));
	const int rows = static_cast<int>(houseBoundingRect.height / tileHeight);

	decltype(distribution.param()) newHeightRange(0, rows - 1);
	distribution.param(newHeightRange);
	const int row = distribution(generator);
	
	const float xResident = houseBoundingRect.left + column * tileWidth;
	const float yResident = houseBoundingRect.top + row * tileHeight;
	setPosition(sf::Vector2f(xResident, yResident));
}
