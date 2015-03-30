#include "titleState.h"
#include "stateIdentifiers.h"
#include "../Resources/resourceIdentifiers.h"

#include "Trambo/Events/event.h"
#include "Trambo/Localize/localize.h"
#include "Trambo/Sounds/musicPlayer.h"
#include "Trambo/Utilities/utility.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>


TitleState::TitleState(trmb::StateStack& stack, trmb::State::Context context)
: trmb::State(stack, context)
, mFullscreen(0x5a0d2314)
, mWindowed(0x11e3c735)
, mGUIContainer(context.window)
{
	trmb::Localize::getInstance().setFilePath("Data/Text/Text.xml");
	trmb::Localize::getInstance().setLanguage("en");

	// ALW - Calculate x, y coordinates relative to the center of the window,
	// ALW - so GUI elements are equidistance from the center in any resolution.
	sf::Vector2f center = sf::Vector2f(context.window->getSize() / 2u);

	sf::Texture& texture = context.textures->get(Textures::ID::TitleScreen);
	mBackgroundSprite.setTexture(texture);
	trmb::centerOrigin(mBackgroundSprite);
	mBackgroundSprite.setPosition(center);

	mText.setFont(context.fonts->get(Fonts::ID::Title));
	mText.setString(trmb::Localize::getInstance().getString("gameTitleUI"));
	mText.setColor(sf::Color(187, 10, 30, 255));
	mText.setCharacterSize(125);
	trmb::centerOrigin(mText);
	mText.setPosition(center - sf::Vector2f(0, 225));

	const float x = center.x - 355.0f;
	const float y = center.y - 110.0f;
	const float buttonHeight = 50.0f;

	mEnglishButton = std::make_shared<trmb::Button>(context, Fonts::ID::Main, SoundEffects::ID::Button, Textures::ID::Buttons, 200, 50);
	mEnglishButton->setPosition(x, y);
	mEnglishButton->setText(trmb::Localize::getInstance().getString("englishButton"));
	mEnglishButton->setCallback([this]()
	{
		trmb::Localize::getInstance().setLanguage("en");
		requestStackPop();
		requestStackPush(States::ID::Menu);
	});

	mMalagasyButton = std::make_shared<trmb::Button>(context, Fonts::ID::Main, SoundEffects::ID::Button, Textures::ID::Buttons, 200, 50);
	mMalagasyButton->setPosition(x, y + buttonHeight);
	mMalagasyButton->setText(trmb::Localize::getInstance().getString("malagasyButton"));
	mMalagasyButton->setCallback([this]()
	{
		trmb::Localize::getInstance().setLanguage("mg");
		requestStackPop();
		requestStackPush(States::ID::Menu);
	});

	mGUIContainer.pack(mEnglishButton);
	mGUIContainer.pack(mMalagasyButton);

	// Play menu theme
	context.music->play(Music::ID::MenuTheme);
}

void TitleState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.draw(mBackgroundSprite);
	window.draw(mText);
	window.draw(mGUIContainer);
}

bool TitleState::update(sf::Time dt)
{
	return true;
}

bool TitleState::handleEvent(const sf::Event& event)
{
	mGUIContainer.handleEvent(event);

	return true;
}

void TitleState::handleEvent(const trmb::Event &gameEvent)
{
	// ALW - Currently, fullscreen and windowed mode are the same.
	if (mFullscreen == gameEvent.getType() || mWindowed == gameEvent.getType())
	{
		repositionGUI();
	}
}

void TitleState::repositionGUI()
{
	// ALW - Calculate x, y coordinates relative to the center of the window,
	// ALW - so GUI elements are equidistance from the center in any resolution.
	const sf::Vector2f center = sf::Vector2f(getContext().window->getSize() / 2u);

	mBackgroundSprite.setPosition(center);
	mText.setPosition(center - sf::Vector2f(0, 225));

	const float x = center.x - 355.0f;
	const float y = center.y - 110.0f;
	const float buttonHeight = 50.0f;

	mEnglishButton->setPosition(sf::Vector2f(x, y));
	mMalagasyButton->setPosition(sf::Vector2f(x, y + buttonHeight));
}
