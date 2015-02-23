#include "mosquitoNode.h"
#include "../Resources/resourceIdentifiers.h"

#include "Trambo/Events/event.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <cassert>
#include <vector>


MosquitoNode::MosquitoNode(const trmb::TextureHolder& textures, sf::Vector2f position, sf::FloatRect worldBounds, SceneNode &houseLayer)
: mBeginSimulationEvent(0x5000e550)
, mTextures(textures)
, mWorldBounds(worldBounds)
, mHouseLayer(houseLayer)
, mAnimation(textures.get(Textures::ID::MosquitoAnimation))
, mPreviousPosition()
, mHasMalaria(false)
, mIndoor(false)
, mBeginSimulationMode(false)
, mDelaySet(false)
, mActive(false)
, mTotalDelayTime()
, mUpdateDelayTime()
, mTotalMovementTime(sf::seconds(1.0))
, mUpdateMovementTime()
, mRandomDevice()				// ALW - obtain random number from hardware
, mGenerator(mRandomDevice())	// ALW - Seed the generator
, mDistribution(0, 3)			// ALW - Define the range
, mWeightedDistribution()
{
	setPosition(position);

	mAnimation.setFrameSize(sf::Vector2i(64, 64));
	mAnimation.setNumFrames(8);
	mAnimation.setDuration(sf::seconds(1));
	mAnimation.setRepeating(true);

	// ALW - Change the range and weights 
	std::vector<float> intervals = { 0, 1, 2, 3 };
	std::vector<float> weights   = { 0.05f, 0.1f, 0.20f };
	decltype(mWeightedDistribution.param()) new_range(intervals.begin(), intervals.end(), weights.begin());
	mWeightedDistribution.param(new_range);
}

sf::FloatRect MosquitoNode::getBoundingRect() const
{
	const float tileWidth = 64;
	const float tileHeight = 64;

	return sf::FloatRect(getPosition().x, getPosition().y, tileWidth, tileHeight);
}

bool MosquitoNode::hasMalaria() const
{
	return mHasMalaria;
}

bool MosquitoNode::isIndoor() const
{
	return mIndoor;
}

void MosquitoNode::setIndoor(bool indoors)
{
	mIndoor = indoors;
}

void MosquitoNode::contractMalaria()
{
	mHasMalaria = true;
	mAnimation.setTexture(mTextures.get(Textures::ID::InfectedMosquitoAnimation));
}

void MosquitoNode::updateCurrent(sf::Time dt)
{
	delaySpawn(dt);

	if (mActive)
	{
		setNextPosition(dt);
		mAnimation.update(dt);
	}
}

void MosquitoNode::handleEvent(const trmb::Event &gameEvent)
{
	if (mBeginSimulationEvent == gameEvent.getType())
	{
		mBeginSimulationMode = true;
	}
}

void MosquitoNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (mActive && !mIndoor)
		target.draw(mAnimation, states);
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
			position = getPosition() - sf::Vector2f(0, tileHeight);
			break;
		case Down:
			position = getPosition() + sf::Vector2f(0, tileHeight);
			break;
		case Left:
			position = getPosition() - sf::Vector2f(tileWidth, 0);
			break;
		case Right:
			position = getPosition() + sf::Vector2f(tileWidth, 0);
			break;
		}

		if (mWorldBounds.left <= position.x && mWorldBounds.width > position.x
			&& mWorldBounds.top <= position.y && mWorldBounds.height > position.y)
		{
			// ALW - The position is within the world bounds
			mPreviousPosition = getPosition();
			setPosition(position);

			if (mIndoor)
			{
				// ALW - Verify indoor mosquito is inside a house's boundaries
				std::set<trmb::SceneNode::Pair> collisionPairs;
				this->checkSceneCollision(mHouseLayer, collisionPairs);

				if (!collisionPairs.size())
				{
					// ALW - There were no collisions between the mosquito and a house. Revert the position.
					setPosition(mPreviousPosition);
				}
			}
		}
	}
}

void MosquitoNode::delaySpawn(sf::Time dt)
{
	if (mBeginSimulationMode)
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
