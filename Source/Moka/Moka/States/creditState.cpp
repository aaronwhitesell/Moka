#include "creditState.h"
#include "../Resources/resourceIdentifiers.h"

#include "Trambo/Events/event.h"
#include "Trambo/Localize/localize.h"
#include "Trambo/Utilities/utility.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>


CreditState::CreditState(trmb::StateStack &stack, trmb::State::Context context)
: State(stack, context)
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
	mText.setString(trmb::Localize::getInstance().getString("creditTitleUI"));
	mText.setColor(sf::Color(187, 10, 30, 255));
	mText.setCharacterSize(125);
	trmb::centerOrigin(mText);
	mText.setPosition(center - sf::Vector2f(0, 225));

	const float x = center.x - 355.0f;
	const float y = center.y - 110.0f;
	const float buttonHeight = 50.0f;

	mBackButton = std::make_shared<trmb::Button>(context, Fonts::ID::Main, SoundEffects::ID::Button, Textures::ID::Buttons, 200, 50);
	mBackButton->setPosition(x, y + buttonHeight);
	mBackButton->setText(trmb::Localize::getInstance().getString("backButton"));
	mBackButton->setCallback(std::bind(&CreditState::requestStackPop, this));

	mGUIContainer.pack(mBackButton);
}

void CreditState::draw()
{
	sf::RenderWindow &window = *getContext().window;

	window.draw(mBackgroundSprite);
	window.draw(mText);
	window.draw(mGUIContainer);
}

bool CreditState::update(sf::Time)
{
	return true;
}

bool CreditState::handleEvent(const sf::Event &event)
{
	mGUIContainer.handleEvent(event);
	return false;
}

void CreditState::handleEvent(const trmb::Event &gameEvent)
{
	// ALW - Currently, fullscreen and windowed mode are the same.
	if (mFullscreen == gameEvent.getType() || mWindowed == gameEvent.getType())
	{
		repositionGUI();
	}
}

void CreditState::repositionGUI()
{
	// ALW - Calculate x, y coordinates relative to the center of the window,
	// ALW - so GUI elements are equidistance from the center in any resolution.
	const sf::Vector2f center = sf::Vector2f(getContext().window->getSize() / 2u);

	mBackgroundSprite.setPosition(center);
	mText.setPosition(center - sf::Vector2f(0, 225));

	const float x = center.x - 355.0f;
	const float y = center.y - 110.0f;
	const float buttonHeight = 50.0f;

	mBackButton->setPosition(sf::Vector2f(x, y + buttonHeight));
}
