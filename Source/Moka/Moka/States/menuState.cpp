#include "menuState.h"
#include "stateIdentifiers.h"
#include "../Resources/resourceIdentifiers.h"

#include "Trambo/Events/event.h"
#include "Trambo/Sounds/musicPlayer.h"
#include "Trambo/Utilities/utility.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include <cmath>


MenuState::MenuState(trmb::StateStack& stack, trmb::State::Context context)
: trmb::State(stack, context)
, mFullscreen(0x5a0d2314)
, mWindowed(0x11e3c735)
, mGUIContainer(context.window)
{
	// ALW - Calculate x, y coordinates relative to the center of the window,
	// ALW - so GUI elements are equidistance from the center in any resolution.
	const sf::Vector2f center = sf::Vector2f(context.window->getSize() / 2u);

	sf::Texture& texture = context.textures->get(Textures::ID::TitleScreen);
	mBackgroundSprite.setTexture(texture);
	trmb::centerOrigin(mBackgroundSprite);
	mBackgroundSprite.setPosition(center);

	mText.setFont(context.fonts->get(Fonts::ID::Title));
	mText.setString("Moka Attack");
	mText.setColor(sf::Color(187, 10, 30, 255));
	mText.setCharacterSize(125);
	trmb::centerOrigin(mText);
	mText.setPosition(center - sf::Vector2f(0, 225));

	const float x = center.x - 350.0f;
	const float y = center.y - 110.0f;
	const float buttonHeight = 50.0f;

	mPlayButton = std::make_shared<trmb::Button>(context, Fonts::ID::Main, SoundEffects::ID::Button, Textures::ID::Buttons, 200, 50);
	mPlayButton->setPosition(x, y);
	mPlayButton->setText("Play");
	mPlayButton->setCallback([this] ()
	{
		requestStackPop();
		requestStackPush(States::ID::Game);
	});

	mCreditButton = std::make_shared<trmb::Button>(context, Fonts::ID::Main, SoundEffects::ID::Button, Textures::ID::Buttons, 200, 50);
	mCreditButton->setPosition(x, y + buttonHeight);
	mCreditButton->setText("Credits");
	mCreditButton->setCallback([this]()
	{
		requestStackPush(States::ID::Credits);
	});

	mExitButton = std::make_shared<trmb::Button>(context, Fonts::ID::Main, SoundEffects::ID::Button, Textures::ID::Buttons, 200, 50);
	mExitButton->setPosition(x, y + 2.0f * buttonHeight);
	mExitButton->setText("Exit");
	mExitButton->setCallback([this]()
	{
		requestStackPop();
	});

	mGUIContainer.pack(mPlayButton);
	mGUIContainer.pack(mCreditButton);
	mGUIContainer.pack(mExitButton);

	// Play menu theme
	context.music->play(Music::ID::MenuTheme);
}

void MenuState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.setView(window.getDefaultView());

	window.draw(mBackgroundSprite);
	window.draw(mText);
	window.draw(mGUIContainer);
}

bool MenuState::update(sf::Time)
{
	return true;
}

bool MenuState::handleEvent(const sf::Event& event)
{
	mGUIContainer.handleEvent(event);
	return false;
}

void MenuState::handleEvent(const trmb::Event &gameEvent)
{
	// ALW - Currently, fullscreen and windowed mode are the same.
	if (mFullscreen == gameEvent.getType() || mWindowed == gameEvent.getType())
	{
		repositionGUI();
	}
}

void MenuState::repositionGUI()
{
	// ALW - Calculate x, y coordinates relative to the center of the window,
	// ALW - so GUI elements are equidistance from the center in any resolution.
	const sf::Vector2f center = sf::Vector2f(getContext().window->getSize() / 2u);

	mBackgroundSprite.setPosition(center);
	mText.setPosition(center - sf::Vector2f(0, 225));

	const float x = center.x - 350.0f;
	const float y = center.y - 110.0f;
	const float buttonHeight = 50.0f;

	mPlayButton->setPosition(sf::Vector2f(x, y));
	mCreditButton->setPosition(sf::Vector2f(x, y + buttonHeight));
	mExitButton->setPosition(sf::Vector2f(x, y + 2.0f * buttonHeight));
}
