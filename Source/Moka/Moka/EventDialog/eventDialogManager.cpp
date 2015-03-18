#include "eventDialogManager.h"
#include "didYouKnow.h"
#include "../HUD/chatBoxUI.h"
#include "../Resources/resourceIdentifiers.h"

#include "Trambo/Localize/localize.h"
#include "Trambo/Sounds/soundPlayer.h"


EventDialogManager::EventDialogManager(ChatBoxUI &chatBoxUI, DidYouKnow &didYouKnow, trmb::SoundPlayer &soundPlayer)
: mSimulationDuration(sf::seconds(120))
, mMinimumEventDialogs(3)
, mChatBoxUI(chatBoxUI)
, mDidYouKnow(didYouKnow)
, mSoundPlayer(soundPlayer)
, mTimer()
, mPause(true)
, mIntervalDuration()
, mTotalIntervals(0)
, mIntervalCount(0)
, mReadyToDisplay(false)
, mFinished(false)
{
}

void EventDialogManager::initialize(int totalScheduledEventDialogs)
{
	if (totalScheduledEventDialogs > mMinimumEventDialogs)
		mTotalIntervals = totalScheduledEventDialogs + 1;
	else
		mTotalIntervals = mMinimumEventDialogs + 1;

	mIntervalDuration = mSimulationDuration / static_cast<float>(mTotalIntervals);
}

bool EventDialogManager::isFinished() const
{
	return mFinished;
}

bool EventDialogManager::isReadyToDisplay()
{
	bool ret = mReadyToDisplay;

	// ALW - Only return true once per interval.
	if (mReadyToDisplay)
		mReadyToDisplay = false;

	return ret;
}

void EventDialogManager::displayText(std::string string)
{
	mChatBoxUI.updateText(string, true);
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
	// ALW - Event dialogs are not displayed at the beginning of the first interval or then end of the last interval.
	// ALW - If an asterisk represents the displaying of an event dialog and there 3 synchronous event dialogs then it 
	// ALW - look like this. | interval 1 |* interval 2 |* interval 3 |* interval 4 |
	if (!mPause && !mFinished)
	{
		mTimer += dt;

		if (mIntervalCount < mTotalIntervals)
		{
			sf::Time startOfNextInterval = mIntervalDuration * static_cast<float>(mIntervalCount + 1);

			if (startOfNextInterval <= mTimer)
			{
				++mIntervalCount;

				if (mIntervalCount >= mTotalIntervals)
				{
					mReadyToDisplay = false;
					mFinished = true;
				}
				else
					mReadyToDisplay = true;
			}
		}
	}
}
