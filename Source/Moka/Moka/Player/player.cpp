#include "player.h"

#include <SFML/Window/Event.hpp>


Player::Player()
: mController()
, mCurrentMissionStatus(MissionStatus::MissionRunning)
{
}

const Controller & Player::getController() const
{
	return mController;
}

Controller & Player::getController()
{
	return mController;
}

void Player::update()
{
	mController.update();
}

void Player::handleEvent(const sf::Event &inputEvent)
{
	mController.handleEvent(inputEvent);
}

void Player::setMissionStatus(MissionStatus status)
{
	mCurrentMissionStatus = status;
}

Player::MissionStatus Player::getMissionStatus() const
{
	return mCurrentMissionStatus;
}
