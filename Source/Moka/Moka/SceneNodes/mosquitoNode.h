#ifndef MOSQUITO_NODE_H
#define MOSQUITO_NODE_H

#include "Trambo/Animations/animation.h"
#include "Trambo/Events/eventHandler.h"
#include "Trambo/SceneNodes/sceneNode.h"
#include "Trambo/Resources/resourceHolder.h"

#include <SFML/Graphics/Rect.hpp>
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
							MosquitoNode(const trmb::TextureHolder &textures, sf::Vector2f position, sf::FloatRect worldBounds
								, SceneNode &houseLayer);
							MosquitoNode(const MosquitoNode &) = delete;
	MosquitoNode &			operator=(const MosquitoNode &) = delete;

	bool					isIndoor() const;

	virtual sf::FloatRect	getBoundingRect() const override;

	void					setIndoor(bool indoors);

	virtual void			updateCurrent(sf::Time dt) override final;
	virtual void			handleEvent(const trmb::Event &gameEvent) override final;


private:
	enum Direction
	{
		Up    = 0,
		Down  = 1,
		Left  = 2,
		Right = 3,
	};


private:
	using					EventGuid = unsigned long;


private:
	virtual void			drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override;
	void					setNextPosition(sf::Time dt);
	void					delaySpawn(sf::Time dt);
	float					getDelay();


private:
	const EventGuid         mBeginSimulationEvent;   // ALW - Matches the GUID in the DaylightUI class.
	trmb::Animation			mMosquito;
	sf::FloatRect			mWorldBounds;
	trmb::SceneNode			&mHouseLayer;
	sf::Vector2f			mPreviousPosition;
	bool					mIndoor;

	bool					mBeginSimulationMode;
	bool                    mDelaySet;
	bool					mActive;

	sf::Time				mTotalDelayTime;
	sf::Time				mUpdateDelayTime;
	sf::Time				mTotalMovementTime;
	sf::Time				mUpdateMovementTime;

	std::random_device		mRandomDevice;
	std::mt19937			mGenerator;
	std::uniform_int_distribution<>				mDistribution;
	std::piecewise_constant_distribution<float>	mWeightedDistribution;
};

#endif