#ifndef TOGGLE_FULLSCREEN_H
#define TOGGLE_FULLSCREEN_H

#include <Trambo/Events/event.h>
#include <Trambo/Events/eventHandler.h>


namespace sf
{
	class Event;
	class RenderWindow;
}

class ToggleFullscreen : public trmb::EventHandler
{
public:
							ToggleFullscreen(sf::RenderWindow &window);
							ToggleFullscreen(const ToggleFullscreen &) = delete;
	ToggleFullscreen &		operator=(const ToggleFullscreen &) = delete;

	void					handleEvent(const sf::Event &inputEvent);
	virtual void			handleEvent(const trmb::Event &) final; // ALW - This class does not handle events.  It only sends them.


private:
	const trmb::Event		mFullscreen;
	const trmb::Event		mWindowed;

	sf::RenderWindow		&mWindow;
	bool					mIsFullscreen;
};

#endif
