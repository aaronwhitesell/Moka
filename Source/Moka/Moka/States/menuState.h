#ifndef MENU_STATE_H
#define MENU_STATE_H

#include "Trambo/GUI/container.h"
#include "Trambo/States/state.h"

#include <SFML/Graphics/Sprite.hpp>


namespace sf
{
	class Event;
	class Time;
}

namespace trmb
{
	class StateStack;
}

class MenuState : public trmb::State
{
public:
							MenuState(trmb::StateStack& stack, trmb::State::Context context);
							MenuState(const MenuState &) = delete;
	MenuState &				operator=(const MenuState &) = delete;

	virtual void			draw();
	virtual bool			update(sf::Time dt);
	virtual bool			handleEvent(const sf::Event& event);

private:
	sf::Sprite				mBackgroundSprite;
	trmb::Container			mGUIContainer;
};

#endif
