#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "../Levels/world.h"

#include "Trambo/States/state.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


namespace sf
{
	class Event;
	class Time;
}

namespace trmb
{
	class StateStack;
}

class player;

class GameState : public trmb::State
{
public:
						GameState(trmb::StateStack &stack, trmb::State::Context context);
						GameState(const GameState &) = delete;
	GameState &			operator=(const GameState &) = delete;

	virtual void		draw();
	virtual bool		update(sf::Time dt);
	virtual bool		handleEvent(const sf::Event &event);


private:
	World				mWorld;
	Player				&mPlayer;
};

#endif

