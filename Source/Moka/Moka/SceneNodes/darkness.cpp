#include "darkness.h"

#include "Trambo/Events/event.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>


Darkness::Darkness(const sf::RenderWindow &window)
: mFullscreen(0x5a0d2314)
, mWindowed(0x11e3c735)
, mBeginSimulationEvent(0x5000e550)
, mWindow(window)
, mBackground()
, mDarken(false)
, mAlpha(0u)
, mCount(0)
, mUpdateTime()
{
	mBackground.setSize(sf::Vector2f(mWindow.getSize()));
	mBackground.setFillColor(sf::Color(0u, 0u, 0u, 0u));
	mBackground.setPosition(0.0f, 0.0f);
}

void Darkness::updateCurrent(sf::Time dt)
{
	// ALW - Simulation Mode
	const int iterations = 30;
	if (mDarken && mCount < iterations)
	{
		mUpdateTime += dt;

		if (mUpdateTime >= sf::seconds(0.1f))
		{
			mAlpha += 3u; // ALW - Will max at 90u
			mBackground.setFillColor(sf::Color(0u, 0u, 0u, mAlpha));
			++mCount;

			mUpdateTime -= sf::seconds(0.1f);
		}
	}
}

void Darkness::handleEvent(const trmb::Event &gameEvent)
{
	// ALW - Currently, fullscreen and windowed mode are the same.
	if (mFullscreen == gameEvent.getType() || mWindowed == gameEvent.getType())
	{
		reposition();
	}
	else if (mBeginSimulationEvent == gameEvent.getType())
	{
		mDarken = true;
	}
}

void Darkness::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
	// ALW - Save then change view
	sf::View previousView = target.getView();
	target.setView(target.getDefaultView());

	if (mDarken)
		target.draw(mBackground, states);

	// ALW - Reset view
	target.setView(previousView);
}

void Darkness::reposition()
{
	// ALW - Calculate x, y coordinates relative to the center of the window,
	// ALW - so GUI elements are equidistance from the center in any resolution.
	mBackground.setSize(sf::Vector2f(mWindow.getSize()));
}
