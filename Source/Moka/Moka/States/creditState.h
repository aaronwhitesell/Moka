#ifndef CREDIT_STATE_H
#define CREDIT_STATE_H

#include "Trambo/GUI/button.h"
#include "Trambo/GUI/container.h"
#include "Trambo/GUI/label.h"
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

class CreditState : public trmb::State
{
public:
							CreditState(trmb::StateStack &stack, trmb::State::Context context);
							CreditState(const CreditState &) = delete;
	CreditState &			operator=(const CreditState &) = delete;

	virtual void			draw();
	virtual bool			update(sf::Time dt);
	virtual bool			handleEvent(const sf::Event &event);


private:
	sf::Sprite				mBackgroundSprite;
	trmb::Container			mGUIContainer;
};

#endif
