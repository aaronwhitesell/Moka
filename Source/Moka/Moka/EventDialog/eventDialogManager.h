#ifndef EVENT_DIALOG_MANAGER_H
#define EVENT_DIALOG_MANAGER_H

#include <SFML/System/Time.hpp>

#include <string>


namespace trmb
{
	class SoundPlayer;
}

class ChatBoxUI;
class DidYouKnow;

class EventDialogManager
{
public:
								EventDialogManager(ChatBoxUI &chatBoxUI, DidYouKnow &didYouKnow, trmb::SoundPlayer &soundPlayer);
								EventDialogManager(const EventDialogManager &) = delete;
	EventDialogManager &		operator=(const EventDialogManager &) = delete;

	void						initialize(int totalScheduledEventDialogs);

	bool						isFinished() const;
	bool						isReadyToDisplay();

	void						displayText(std::string string);

	void						start();
	void						stop();
	void						reset();

	void						update(sf::Time dt);


private:
	const sf::Time              mSimulationDuration;
	const int					mMinimumEventDialogs;

	ChatBoxUI					&mChatBoxUI;
	DidYouKnow					&mDidYouKnow;
	trmb::SoundPlayer			&mSoundPlayer;

	sf::Time					mTimer;
	bool						mPause;

	sf::Time					mIntervalDuration;
	int							mTotalIntervals;
	int							mIntervalCount;
	bool						mReadyToDisplay;
	bool						mFinished;
};

#endif
