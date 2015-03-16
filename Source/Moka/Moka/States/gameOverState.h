#ifndef GAME_OVER_STATE_H
#define GAME_OVER_STATE_H

#include "Trambo/States/state.h"

#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Time.hpp>


namespace sf
{
	class Event;
}

namespace trmb
{
	class StateStack;
}

class GameOverState : public trmb::State
{
public:
						GameOverState(trmb::StateStack& stack, trmb::State::Context context);
						GameOverState(const GameOverState &) = delete;
	GameOverState &		operator=(const GameOverState &) = delete;

	virtual void		draw() override final;
	virtual bool		update(sf::Time dt)  override final;
	virtual bool		handleEvent(const sf::Event& event)  override final;


private:
	sf::Text			mGameOverText;
	sf::Time			mElapsedTime;
};

#endif
