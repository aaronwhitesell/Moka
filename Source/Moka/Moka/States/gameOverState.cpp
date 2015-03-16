#include "gameOverState.h"
#include "stateIdentifiers.h"
//#include "../Player/player.h"
#include "../Resources/resourceIdentifiers.h"

#include "Trambo/States/stateStack.h"
#include "Trambo/Utilities/utility.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>


GameOverState::GameOverState(trmb::StateStack& stack, trmb::State::Context context)
: trmb::State(stack, context)
, mGameOverText()
, mElapsedTime(sf::Time::Zero)
{
	sf::Font& font = context.fonts->get(Fonts::ID::Main);
	sf::Vector2f windowSize(context.window->getSize());

	mGameOverText.setFont(font);
//	if (context.player->getMissionStatus() == Player::MissionStatus::MissionFailure)
//		mGameOverText.setString("Mission failed!");
//	else
//		mGameOverText.setString("Mission successful!");
	mGameOverText.setString("Simulation Finished!");

	mGameOverText.setCharacterSize(70);
	trmb::centerOrigin(mGameOverText);
	mGameOverText.setPosition(0.5f * windowSize.x, 0.4f * windowSize.y);
}

void GameOverState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	// Create dark, semitransparent background
	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
	backgroundShape.setSize(window.getView().getSize());

	window.draw(backgroundShape);
	window.draw(mGameOverText);
}

bool GameOverState::update(sf::Time dt)
{
	// Show state for 3 seconds, after return to menu
	mElapsedTime += dt;
	if (mElapsedTime > sf::seconds(3))
	{
		requestStateClear();
		requestStackPush(States::ID::Menu);
	}
	return false;
}

bool GameOverState::handleEvent(const sf::Event&)
{
	return false;
}
