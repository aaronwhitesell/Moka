#ifndef PLAYER_H
#define PLAYER_H

#include "controller.h"


namespace sf
{
	class Event;
}

class Player
{
public:
	enum class MissionStatus
	{
		MissionRunning,
		MissionSuccess,
		MissionFailure
	};


public:
							Player();
							Player(const Player &) = delete;
	Player &				operator=(const Player &) = delete;

	const Controller &		getController() const;
	Controller &			getController();

	void					update();
	void					handleEvent(const sf::Event &inputEvent);

	void					setMissionStatus(MissionStatus status);
	MissionStatus			getMissionStatus() const;


private:
	Controller				mController;
	MissionStatus			mCurrentMissionStatus;
};

#endif
