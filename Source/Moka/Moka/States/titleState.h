#ifndef TITLE_STATE_H
#define TITLE_STATE_H

#include "Trambo/States/state.h"

#include <SFML/Graphics/Sprite.hpp>
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

class TitleState : public trmb::State
{
public:
						TitleState(trmb::StateStack& stack, trmb::State::Context context);
						TitleState(const TitleState &) = delete;
	TitleState &		operator=(const TitleState &) = delete;

	virtual void		draw();
	virtual bool		update(sf::Time dt);
	virtual bool		handleEvent(const sf::Event& event);


private:
	sf::Sprite			mBackgroundSprite;
	sf::Text			mText;

	bool				mShowText;
	sf::Time			mTextEffectTime;
};

#endif
