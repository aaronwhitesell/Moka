#include "mosquitoNode.h"
#include "../Resources/resourceIdentifiers.h"

#include "Trambo/Events/event.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>

#include <cassert>
#include <vector>


MosquitoNode::MosquitoNode(const trmb::TextureHolder& textures, sf::Vector2f position, sf::FloatRect worldBounds)
: mBeginSimulationMode(0x5000e550)
, mMosquito(textures.get(Textures::ID::MosquitoAnimation))
, mWorldBounds(worldBounds)
, mNextPosition(position)
, mTotalDelayTime()
, mUpdateDelayTime()
, mTotalMovementTime(sf::seconds(1.0))
, mUpdateMovementTime()
, mActivate(false)
, mDelaySet(false)
, mActive(false)
, mRandomDevice()				// ALW - obtain random number from hardware
, mGenerator(mRandomDevice())	// ALW - Seed the generator
, mDistribution(0, 3)			// ALW - Define the range
, mWeightedDistribution()
{
	mMosquito.setPosition(position);
	mMosquito.setFrameSize(sf::Vector2i(64, 64));
	mMosquito.setNumFrames(8);
	mMosquito.setDuration(sf::seconds(1));
	mMosquito.setRepeating(true);

	// ALW - Change the range and weights 
	std::vector<float> intervals = { 0, 1, 2, 3 };
	std::vector<float> weights   = { 0.05f, 0.1f, 0.20f };
	decltype(mWeightedDistribution.param()) new_range(intervals.begin(), intervals.end(), weights.begin());
	mWeightedDistribution.param(new_range);
}

void MosquitoNode::updateCurrent(sf::Time dt)
{
	delaySpawn(dt);

	if (mActive)
	{
		setNextPosition(dt);
		mMosquito.update(dt);
	}
}

void MosquitoNode::handleEvent(const trmb::Event &gameEvent)
{
	if (mBeginSimulationMode == gameEvent.getType())
	{
		mActivate = true;
	}
}

void MosquitoNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (mActive)
		target.draw(mMosquito, states);
}

void MosquitoNode::setNextPosition(sf::Time dt)
{
	mUpdateMovementTime += dt;

	if (mUpdateMovementTime >= mTotalMovementTime)
	{
		mUpdateMovementTime -= mTotalMovementTime;

		sf::Vector2f position;
		const float tileWidth = 64;
		const float tileHeight = 64;
		const int direction = mDistribution(mGenerator);
		assert(("The direction is invalid", 0 <= direction && direction <= 3));

		switch (direction)
		{
		case Up:
			position = mMosquito.getPosition() - sf::Vector2f(0, tileHeight);
			break;
		case Down:
			position = mMosquito.getPosition() + sf::Vector2f(0, tileHeight);
			break;
		case Left:
			position = mMosquito.getPosition() - sf::Vector2f(tileWidth, 0);
			break;
		case Right:
			position = mMosquito.getPosition() + sf::Vector2f(tileWidth, 0);
			break;
		}

		if (mWorldBounds.left <= position.x && mWorldBounds.width > position.x
			&& mWorldBounds.top <= position.y && mWorldBounds.height > position.y)
		{
			// ALW - Next position is within the world bounds
			mNextPosition = position;
		}
	}
}

void MosquitoNode::delaySpawn(sf::Time dt)
{
	if (mActivate)
	{
		if (!mDelaySet)
		{
			mTotalDelayTime = sf::seconds(getDelay());
			mDelaySet = true;
		}

		mUpdateDelayTime += dt;

		if (mUpdateDelayTime >= mTotalDelayTime)
		{
			mActive = true;
		}
	}
}

float MosquitoNode::getDelay()
{
	return mWeightedDistribution(mGenerator);
}
