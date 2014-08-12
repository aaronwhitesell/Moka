#include "ToggleFullscreen.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>


ToggleFullscreen::ToggleFullscreen(sf::RenderWindow &window)
: mFullscreen(0x5a0d2314)
, mWindowed(0x11e3c735)
, mWindow(window)
, mIsFullscreen(false)
{
}

void ToggleFullscreen::handleEvent(const sf::Event &inputEvent)
{
	if (inputEvent.type == sf::Event::KeyPressed)
	{
		if (inputEvent.key.control && inputEvent.key.code == sf::Keyboard::F)
		{
			mIsFullscreen = !mIsFullscreen;

			if (mIsFullscreen)
			{
				mWindow.create(sf::VideoMode::getFullscreenModes().front(), "Moka", sf::Style::Fullscreen);
				EventHandler::sendEvent(mFullscreen);
			}
			else
			{
				mWindow.create(sf::VideoMode(800, 600), "Moka", sf::Style::Close);
				EventHandler::sendEvent(mWindowed);
			}
		}
	}
}

void ToggleFullscreen::handleEvent(const trmb::Event &)
{
	// ALW - This class does not handle events.  It only sends them.
}