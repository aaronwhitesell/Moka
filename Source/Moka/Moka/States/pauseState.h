#ifndef PAUSE_STATE_H
#define PAUSE_STATE_H

#include "Trambo/Events/eventHandler.h"
#include "Trambo/GUI/button.h"
#include "Trambo/GUI/container.h"
#include "Trambo/States/state.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <memory>


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

class PauseState : public trmb::State, public trmb::EventHandler
{
public:
						PauseState(trmb::StateStack& stack, trmb::State::Context context);
						~PauseState();

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

	sf::RectangleShape				mBackgroundShape;
	sf::Text						mPausedText;
	std::shared_ptr<trmb::Button>	mReturnButton;
	std::shared_ptr<trmb::Button>	mBackToMenuButton;
	trmb::Container					mGUIContainer;
};

#endif
