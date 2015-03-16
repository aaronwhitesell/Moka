#ifndef EVENT_DIALOG_MANAGER_H
#define EVENT_DIALOG_MANAGER_H

#include <SFML/System/Time.hpp>


class ChatBoxUI;
class DidYouKnow;

class EventDialogManager
{
public:
								EventDialogManager(ChatBoxUI &chatBoxUI, DidYouKnow &didYouKnow);
								EventDialogManager(const EventDialogManager &) = delete;
	EventDialogManager &		operator=(const EventDialogManager &) = delete;

	bool						isFinished() const;

	void						start();
	void						stop();
	void						reset();

	void						update(sf::Time dt);


private:
	const sf::Time				mEventDialog1;
	const sf::Time				mEventDialog2;
	const sf::Time				mEventDialog3;
	const sf::Time				mEventDialog4;
	const sf::Time				mEventDialog5;
	const sf::Time              mSimulationDuration;

	ChatBoxUI					&mChatBoxUI;
	DidYouKnow					&mDidYouKnow;

	sf::Time					mTimer;
	bool						mPause;
	bool						mFinished;

	bool						mDisableEventDialog1;
	bool						mDisableEventDialog2;
	bool						mDisableEventDialog3;
	bool						mDisableEventDialog4;
	bool						mDisableEventDialog5;
};

#endif
