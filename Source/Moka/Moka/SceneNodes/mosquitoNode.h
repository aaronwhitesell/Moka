#ifndef MOSQUITO_NODE_H
#define MOSQUITO_NODE_H

#include "Trambo/Animations/animation.h"
#include "Trambo/Events/eventHandler.h"
#include "Trambo/SceneNodes/sceneNode.h"
#include "Trambo/Resources/resourceHolder.h"

#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include <random>


namespace sf
{
	class RenderStates;
	class RenderTarget;
}

namespace trambo
{
	class Event;
}

class MosquitoNode : public trmb::SceneNode, trmb::EventHandler
{
public:
							MosquitoNode(const trmb::TextureHolder &textures, sf::Vector2f position);
							MosquitoNode(const MosquitoNode &) = delete;
	MosquitoNode &			operator=(const MosquitoNode &) = delete;

	virtual void			updateCurrent(sf::Time dt) override final;
	virtual void			handleEvent(const trmb::Event &gameEvent) override final;


private:
	using					EventGuid = unsigned long;


private:
	virtual void			drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override;
	float					getDelay();


private:
	const EventGuid         mBeginSimulationMode;   // ALW - Matches the GUID in the DaylightUI class.
	trmb::Animation			mMosquito;
	sf::Time				mDelay;
	sf::Time				mUpdateTime;
	bool					mActivate;
	bool                    mDelaySet;
	bool					mActive;
	std::random_device		mRandomDevice;
	std::mt19937			mGenerator;
	std::piecewise_constant_distribution<float>	mDistribution;
};

#endif
