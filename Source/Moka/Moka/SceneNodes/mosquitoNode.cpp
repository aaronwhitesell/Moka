#include "mosquitoNode.h"
#include "../Resources/resourceIdentifiers.h"

#include "Trambo/Events/event.h"
#include "Trambo/Utilities/utility.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <cassert>
#include <vector>


MosquitoNode::MosquitoNode(const trmb::TextureHolder& textures, sf::Vector2f position, bool active, sf::FloatRect worldBounds
	, SceneNode &houseLayer)
: mBeginSimulationEvent(0x5000e550)
, mCreateTextPrompt(0x25e87fd8)
, mClearTextPrompt(0xc1523265)
, mTextures(textures)
, mWorldBounds(worldBounds)
, mHouseLayer(houseLayer)
, mAnimation(textures.get(Textures::ID::MosquitoAnimation))
, mPreviousPosition()
, mHasMalaria(false)
, mIndoor(false)
, mBeginSimulationMode(false)
, mPause(false)
, mDelaySet(false)
, mActive(active)
, mTotalDelayTime()
, mUpdateDelayTime()
, mTotalMovementTime(sf::seconds(1.0))
, mUpdateMovementTime()
, mRandomDevice()				// ALW - obtain random number from hardware
, mGenerator(mRandomDevice())	// ALW - Seed the generator
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
	if (!mPause)
	{
		if (mDelaySet)
			delaySpawn(dt);

		if (mActive)
		{
			setNextPosition(dt);
			mAnimation.update(dt);
		}
	}
}

void MosquitoNode::handleEvent(const trmb::Event &gameEvent)
{
	if (mBeginSimulationEvent == gameEvent.getType())
	{
		delaySpawn(sf::seconds(0));	// ALW - Each time after this method will be fed the time delta
	}
	else if (mCreateTextPrompt == gameEvent.getType())
	{
		mPause = true;
	}
	else if (mClearTextPrompt == gameEvent.getType())
	{
		mPause = false;
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
		const int direction = trmb::randomInt(Direction::Count);
		assert(("The direction is invalid", 0 <= direction && direction < Direction::Count));

		switch (direction)
		{
		case Direction::Up:
			position = getPosition() - sf::Vector2f(0, tileHeight);
			break;
		case Direction::Down:
			position = getPosition() + sf::Vector2f(0, tileHeight);
			break;
		case Direction::Left:
			position = getPosition() - sf::Vector2f(tileWidth, 0);
			break;
		case Direction::Right:
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

float MosquitoNode::getDelay()
{
	return mWeightedDistribution(mGenerator);
}
