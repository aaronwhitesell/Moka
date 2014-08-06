#include "creditState.h"
#include "../Resources/resourceIdentifiers.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>


CreditState::CreditState(trmb::StateStack &stack, trmb::State::Context context)
: State(stack, context)
, mGUIContainer(context.window)
{
	mBackgroundSprite.setTexture(context.textures->get(Textures::ID::TitleScreen));

	auto backButton = std::make_shared<trmb::Button>(context, Fonts::ID::Main, SoundEffects::ID::Button, Textures::ID::Buttons, 200, 50);
	backButton->setPosition(20.0f, 400.0f);
	backButton->setText("Back");
	backButton->setCallback(std::bind(&CreditState::requestStackPop, this));

	mGUIContainer.pack(backButton);
}

void CreditState::draw()
{
	sf::RenderWindow &window = *getContext().window;

	window.draw(mBackgroundSprite);
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
