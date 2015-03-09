#include "residentNode.h"
#include "houseNode.h"
#include "../GameObjects/interactiveObject.h"

#include "Trambo/Events/event.h"
#include "Trambo/Utilities/utility.h"

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
{
	generateSpawnPosition(houseNode->getCollisionBox());
}

bool ResidentNode::hasMalaria() const
{
	return mHasMalaria;
}

bool ResidentNode::isBitten(int totalMintNets, int totalDamagedNets) const
{
	const int bitten = 0;
	bool ret = false;

	if (isNetDamaged(totalDamagedNets))
	{
		// ALW - Mosquito has a 20% chance to pass through damaged net and bite resident.
		const int diceRoll = trmb::randomInt(5);

		if (diceRoll == bitten)
			ret = true;
	}
	else if (isNetMint(totalMintNets, totalDamagedNets))
	{
		// ALW - Mosquito has a 10% chance to pass through mint net and bite resident.
		const int diceRoll = trmb::randomInt(10);

		if (diceRoll == bitten)
			ret = true;
	}
	else
	{
		// ALW - Mosquito has a 100% chance to pass through no net and bite resident.
		ret = true;
	}

	return ret;
}

bool ResidentNode::isCured(int RDTs, int ACTs) const
{
	bool ret = false;

	if (isDiagnosed(RDTs))
	{
		ret = isTreated(ACTs);
	}

	return ret;
}

sf::FloatRect ResidentNode::getBoundingRect() const
{
	const float tileWidth = 64.0f;
	const float tileHeight = 64.0f;

	return sf::FloatRect(getPosition().x, getPosition().y, tileWidth, tileHeight);
}

void ResidentNode::contractMalaria()
{
	mHasMalaria = true;
	sendMalariaMsg();
}

void ResidentNode::handleEvent(const trmb::Event &gameEvent)
{
	if (mBeginSimulationEvent == gameEvent.getType())
	{
		if (mHasMalaria)
			sendMalariaMsg(); // ALW - Display patient zero
	}
}

bool ResidentNode::isNetDamaged(int totalDamagedNets) const
{
	bool ret = false;
	// ALW - ResidentIDs has zero-based numbering, so it is necessary to add 1 before working with it.
	// ALW - Divide by 2, because there are two residents per bed max.
	const float sleepingPairs = (mResidentID + 1.0f) / 2.0f;

	// ALW - We assume damaged nets come first. Continue reading for explanation.
	// ALW - Beds are displayed from top to bottom in this order; damaged nets, mint nets, and no nets.
	// ALW - The ordering is important. If mint nets came before damaged nets and the player does not
	// ALW - repair the net then the first bed would not have a net. This could leave a bed with a
	// ALW - damaged net not at full capcity while filling a bed with no net, since the total residents
	// ALW - per house is not known here the "order" is filled on a first come first served basis.
	if (sleepingPairs <= totalDamagedNets)
	{
		ret = true;
	}

	return ret;
}

bool ResidentNode::isNetMint(int totalMintNets, int totalDamagedNets) const
{
	bool ret = false;
	const int totalNets = totalDamagedNets + totalMintNets;
	// ALW - ResidentIDs has zero-based numbering, so it is necessary to add 1 before working with it.
	// ALW - Divide by 2, because there are two residents per bed max.
	const float sleepingPairs = (mResidentID + 1.0f) / 2.0f;

	// ALW - Assume mint nets come after damaged nets. Continue reading for explanation.
	// ALW - Beds are displayed from top to bottom in this order; damaged nets, mint nets, and no nets.
	// ALW - The ordering is important. If mint nets came before damaged nets and the player does not
	// ALW - repair the net then the first bed would not have a net. This could leave a bed with a
	// ALW - damaged net not at full capcity while filling a bed with no net, since the total residents
	// ALW - per house is not known here the "order" is filled on a first come first served basis.
	if (totalDamagedNets < sleepingPairs && sleepingPairs <= totalNets)
	{
		ret = true;
	}

	return ret;
}

bool ResidentNode::isDiagnosed(int RDTs) const
{
	bool ret = false;
	const int diceRoll = trmb::randomInt(100);

	switch (RDTs)
	{
	case Stock::None:
		if (diceRoll == 0)
			ret = true;			// ALW - 1% chance to seek diagnosis
		break;
	case Stock::Small:
		if (diceRoll < 30)
			ret = true;			// ALW - 30% chance to seek diagnosis
		break;
	case Stock::Medium:
		if (diceRoll < 60)
			ret = true;			// ALW - 60% chance to seek diagnosis
		break;
	case Stock::Large:
		if (diceRoll < 90)
			ret = true;			// ALW - 90% chance to seek diagnosis
		break;
	default:
		assert(("The RDTs total is out of range!", false));
	}

	return ret;
}

bool ResidentNode::isTreated(int ACTs) const
{
	bool ret = false;
	const int diceRoll = trmb::randomInt(100);

	switch (ACTs)
	{
	case Stock::None:
		if (diceRoll == 0)
			ret = true;			// ALW - 1% chance to receive treatment
		break;
	case Stock::Small:
		if (diceRoll < 30)
			ret = true;			// ALW - 30% chance to receive treatment
		break;
	case Stock::Medium:
		if (diceRoll < 60)
			ret = true;			// ALW - 60% chance to receive treatment
		break;
	case Stock::Large:
		if (diceRoll < 90)
			ret = true;			// ALW - 90% chance to receive treatment
		break;
	default:
		assert(("The ACTs total is out of range!", false));
	}

	return ret;
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
