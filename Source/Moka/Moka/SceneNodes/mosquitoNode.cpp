#include "mosquitoNode.h"
#include "../Resources/resourceIdentifiers.h"

#include "Trambo/Events/event.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>

#include <vector>


MosquitoNode::MosquitoNode(const trmb::TextureHolder& textures, sf::Vector2f position)
: mBeginSimulationMode(0x5000e550)
, mMosquito(textures.get(Textures::ID::MosquitoAnimation))
, mDelay()
, mUpdateTime()
, mActivate(false)
, mDelaySet(false)
, mActive(false)
, mRandomDevice()				// ALW - obtain random number from hardware
, mGenerator(mRandomDevice())	// ALW - Seed the generator
, mDistribution()				// ALW - Define the range
{
	mMosquito.setPosition(position);
	mMosquito.setFrameSize(sf::Vector2i(64, 64));
	mMosquito.setNumFrames(8);
	mMosquito.setDuration(sf::seconds(1));
	mMosquito.setRepeating(true);

	// ALW - Change the range and weights 
	std::vector<float> intervals = { 0, 1, 2, 3 };
	std::vector<float> weights   = { 0.05f, 0.1f, 0.20f };
	decltype(mDistribution.param()) new_range(intervals.begin(), intervals.end(), weights.begin());
	mDistribution.param(new_range);
}

void MosquitoNode::updateCurrent(sf::Time dt)
{
	if (mActive)
		mMosquito.update(dt);

	if (mActivate && !mDelaySet)
	{
		mDelay = sf::seconds(getDelay());
		mDelaySet = true;
	}

	if (mActivate)
	{
		mUpdateTime += dt;

		if (mUpdateTime >= mDelay)
		{
			mActive = true;
		}
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

float MosquitoNode::getDelay()
{
	return mDistribution(mGenerator);
}
