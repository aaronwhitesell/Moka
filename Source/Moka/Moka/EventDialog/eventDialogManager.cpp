#include "eventDialogManager.h"
#include "didYouKnow.h"
#include "../HUD/chatBoxUI.h"
#include "../Resources/resourceIdentifiers.h"

#include "Trambo/Localize/localize.h"
#include "Trambo/Sounds/soundPlayer.h"


EventDialogManager::EventDialogManager(ChatBoxUI &chatBoxUI, DidYouKnow &didYouKnow, trmb::SoundPlayer &soundPlayer)
: mEventDialog1(sf::seconds(20))
, mEventDialog2(sf::seconds(40))
, mEventDialog3(sf::seconds(60))
, mEventDialog4(sf::seconds(80))
, mEventDialog5(sf::seconds(100))
, mSimulationDuration(sf::seconds(120))
, mChatBoxUI(chatBoxUI)
, mDidYouKnow(didYouKnow)
, mSoundPlayer(soundPlayer)
, mTimer()
, mPause(true)
, mFinished(false)
, mDisableEventDialog1(false)
, mDisableEventDialog2(false)
, mDisableEventDialog3(false)
, mDisableEventDialog4(false)
, mDisableEventDialog5(false)
{
}

bool EventDialogManager::isFinished() const
{
	return mFinished;
}

void EventDialogManager::updateText(std::string didYouKnow)
{
	mChatBoxUI.updateText(didYouKnow, true);
	mSoundPlayer.play(SoundEffects::ID::Button);
}

void EventDialogManager::updateText(std::string eventDialog, std::string didYouKnow)
{
	mChatBoxUI.updateText(eventDialog + " " + didYouKnow, true);
	mSoundPlayer.play(SoundEffects::ID::Button);
}

void EventDialogManager::start()
{
	mPause = false;
}

void EventDialogManager::stop()
{
	mPause = true;
}

void EventDialogManager::reset()
{
	mPause = true;
	mTimer = sf::Time::Zero;
}

void EventDialogManager::update(sf::Time dt)
{
	if (!mPause)
	{
		mTimer += dt;

		if (mSimulationDuration <= mTimer)
		{
			mFinished = true;
		}
		else if (mEventDialog5 <= mTimer && !mDisableEventDialog5)
		{
			updateText(trmb::Localize::getInstance().getString(mDidYouKnow.getDidYouKnow()));
			mDisableEventDialog5 = true;
		}
		if (mEventDialog4 <= mTimer && !mDisableEventDialog4)
		{
			updateText(trmb::Localize::getInstance().getString(mDidYouKnow.getDidYouKnow()));
			mDisableEventDialog4 = true;
		}
		if (mEventDialog3 <= mTimer && !mDisableEventDialog3)
		{
			updateText(trmb::Localize::getInstance().getString(mDidYouKnow.getDidYouKnow()));
			mDisableEventDialog3 = true;
		}
		if (mEventDialog2 <= mTimer && !mDisableEventDialog2)
		{
			updateText(trmb::Localize::getInstance().getString(mDidYouKnow.getDidYouKnow()));
			mDisableEventDialog2 = true;
		}
		if (mEventDialog1 <= mTimer && !mDisableEventDialog1)
		{
			updateText(trmb::Localize::getInstance().getString(mDidYouKnow.getDidYouKnow()));
			mDisableEventDialog1 = true;
		}
	}
}
