#include "gameState.h"
#include "stateIdentifiers.h"
#include "../Player/player.h"
#include "../Resources/resourceIdentifiers.h"

#include "Trambo/Sounds/musicPlayer.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>


GameState::GameState(trmb::StateStack &stack, trmb::State::Context context)
: trmb::State(stack, context)
, mWorld(*context.window, *context.fonts, *context.sounds)
, mPlayer(*context.player)
{
	mPlayer.setMissionStatus(Player::MissionStatus::MissionRunning);

	// Play game theme
	context.music->play(Music::ID::MissionTheme);
}

void GameState::draw()
{
	mWorld.draw();
}

bool GameState::update(sf::Time dt)
{
	mWorld.update(dt);
	mPlayer.update();

	if (mWorld.isScoreboardFinished())
	{
		requestStateClear();
		requestStackPush(States::ID::Menu);
	}

	return true;
}

bool GameState::handleEvent(const sf::Event &event)
{
	mPlayer.handleEvent(event);

	// Escape pressed, trigger the pause screen
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		requestStackPush(States::ID::Pause);

	return true;
}
