#ifndef TITLE_STATE_H
#define TITLE_STATE_H

#include "Trambo/GUI/button.h"
#include "Trambo/GUI/container.h"
#include "Trambo/Events/eventHandler.h"
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
	class Event;
	class StateStack;
}

class TitleState : public trmb::State, public trmb::EventHandler
{
public:
						TitleState(trmb::StateStack& stack, trmb::State::Context context);
						TitleState(const TitleState &) = delete;
	TitleState &		operator=(const TitleState &) = delete;

	virtual void		draw();
	virtual bool		update(sf::Time dt);
	virtual bool		handleEvent(const sf::Event& event);
	virtual void		handleEvent(const trmb::Event &gameEvent);


private:
	typedef unsigned long EventGuid;


private:
	void					repositionGUI();


private:
	const EventGuid					mFullscreen; // ALW - Matches the GUID in the ToggleFullscreen class.
	const EventGuid					mWindowed;   // ALW - Matches the GUID in the ToggleFullscreen class.

	sf::Sprite						mBackgroundSprite;
	sf::Text						mText;
	std::shared_ptr<trmb::Button>	mEnglishButton;
	std::shared_ptr<trmb::Button>	mMalagasyButton;
	trmb::Container					mGUIContainer;
};

#endif
