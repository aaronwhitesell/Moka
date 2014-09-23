#ifndef HERO_H
#define HERO_H

#include "Trambo/Events/eventHandler.h"
#include "Trambo/SceneNodes/entity.h"

#include <SFML/Graphics/Rect.hpp>
//#include <SFML/Graphics/RectangleShape.hpp> // ALW - mDebugShape


namespace sf
{
	class RenderStates;
	class RenderTarget;
	class Time;
	class View;
}

namespace trmb
{
	class Event;
}

class Hero : public trmb::Entity, public trmb::EventHandler // inherits SceneNode indirectly
{
public:
							Hero(sf::FloatRect worldBounds, const sf::View &view);
							Hero(const Hero &) = delete;
	Hero &					operator=(const Hero &) = delete;

	virtual void			handleEvent(const trmb::Event &gameEvent);
	virtual void			updateCurrent(sf::Time dt);

	float					getMaxSpeed() const;


private:
	typedef unsigned long EventGuid;


private:
//	virtual void			drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const; // ALW - mDebugShape

	void					correctPosition();
	void					correctDiagonalVelocity();


private:
	const sf::View			&mView;
	sf::FloatRect			mWorldBounds;
//	sf::RectangleShape		mDebugShape;
};

#endif
