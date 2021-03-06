#include "pauseState.h"
#include "stateIdentifiers.h"
#include "../Resources/resourceIdentifiers.h"

#include "Trambo/Events/event.h"
#include "Trambo/Localize/localize.h"
#include "Trambo/Sounds/musicPlayer.h"
#include "Trambo/Utilities/utility.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

#include <cmath>


PauseState::PauseState(trmb::StateStack& stack, trmb::State::Context context)
: trmb::State(stack, context)
, mFullscreen(0x5a0d2314)
, mWindowed(0x11e3c735)
, mBackgroundShape()
, mPausedText()
, mGUIContainer(context.window)
{
	// ALW - Calculate x, y coordinates relative to the size of the window,
	// ALW - so GUI elements are equidistance from the center in any resolution.
	const sf::Vector2f windowSize = sf::Vector2f(context.window->getSize());

	mBackgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
	mBackgroundShape.setSize(windowSize);

	sf::Font& font = context.fonts->get(Fonts::ID::Main);
	mPausedText.setFont(font);
	mPausedText.setString(trmb::Localize::getInstance().getString("gamePausedUI"));
	mPausedText.setCharacterSize(70);
	trmb::centerOrigin(mPausedText);
	mPausedText.setPosition(std::floor(0.5f * windowSize.x), std::floor(0.3f * windowSize.y));

	const float buttonHeight = 50.0f;
	const float buffer = 25.0f;

	mReturnButton = std::make_shared<trmb::Button>(context, Fonts::ID::Main, SoundEffects::ID::Button, Textures::ID::Buttons, 200, 50);
	mReturnButton->setPosition(std::floor(0.5f * windowSize.x - 100), std::floor(0.3f * windowSize.y + 0.5f * buttonHeight + buffer));
	mReturnButton->setText(trmb::Localize::getInstance().getString("returnButton"));
	mReturnButton->setCallback([this]()
	{
		getContext().music->setPaused(false);
		requestStackPop();
	});

	mBackToMenuButton = std::make_shared<trmb::Button>(context, Fonts::ID::Main, SoundEffects::ID::Button, Textures::ID::Buttons, 200, 50);
	mBackToMenuButton->setPosition(std::floor(0.5f * windowSize.x - 100), std::floor(0.3f * windowSize.y + 1.5f * buttonHeight + buffer));
	mBackToMenuButton->setText(trmb::Localize::getInstance().getString("backToMenuButton"));
	mBackToMenuButton->setCallback([this]()
	{
		getContext().music->stop();
		requestStateClear();
		requestStackPush(States::ID::Menu);
	});

	mGUIContainer.pack(mReturnButton);
	mGUIContainer.pack(mBackToMenuButton);

	getContext().music->setPaused(true);
}

void PauseState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	window.draw(mBackgroundShape);
	window.draw(mPausedText);
	window.draw(mGUIContainer);
}

bool PauseState::update(sf::Time)
{
	return false;
}

bool PauseState::handleEvent(const sf::Event& event)
{
	mGUIContainer.handleEvent(event);

	// ALW - Escape pressed, return to the game (same as pressing the Return button)
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
	{
		getContext().music->setPaused(false);
		requestStackPop();
	}

	return false;
}

void PauseState::handleEvent(const trmb::Event &gameEvent)
{
	// ALW - Currently, fullscreen and windowed mode are the same.
	if (mFullscreen == gameEvent.getType() || mWindowed == gameEvent.getType())
	{
		repositionGUI();
	}
}

void PauseState::repositionGUI()
{
	// ALW - Calculate x, y coordinates relative to the size of the window,
	// ALW - so GUI elements are equidistance from the center in any resolution.
	const sf::Vector2f windowSize = sf::Vector2f(getContext().window->getSize());

	mBackgroundShape.setSize(windowSize);
	mPausedText.setPosition(std::floor(0.5f * windowSize.x), std::floor(0.3f * windowSize.y));

	const float buttonHeight = 50.0f;
	const float buffer = 25.0f;

	mReturnButton->setPosition(std::floor(0.5f * windowSize.x - 100), std::floor(0.3f * windowSize.y + 0.5f * buttonHeight + buffer));
	mBackToMenuButton->setPosition(std::floor(0.5f * windowSize.x - 100), std::floor(0.3f * windowSize.y + 1.5f * buttonHeight + buffer));
}
