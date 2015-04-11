#include "windowNode.h"
#include "../GameObjects/interactiveObject.h"
#include "../HUD/chatBoxUI.h"
#include "../HUD/daylightUI.h"
#include "../HUD/optionsUI.h"
#include "../HUD/uiBundle.h"
#include "../Resources/resourceIdentifiers.h"

#include "Trambo/Localize/localize.h"
#include "Trambo/Sounds/soundPlayer.h"
#include "Trambo/Utilities/utility.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

#include <stdexcept>


WindowNode::WindowNode(const InteractiveObject &interactiveObject, const sf::RenderWindow &window, const sf::View &view
	, UIBundle &uiBundle, const trmb::TextureHolder &textures, trmb::SoundPlayer &soundPlayer, DaylightUI &daylightUI
	, ChatBoxUI &chatBoxUI)
: PreventionNode(interactiveObject, window, view, uiBundle)
, mWindowUIActivated(0x961e8d0b)
, mDrawWindowUI(0x30459275)
, mDoNotDrawWindowUI(0xf83a20bd)
, mDrawWindowScreenSprite(0x41a9ff51, interactiveObject.getName())
, mDoNotDrawWindowScreenSprite(0xbfe9d3e6, interactiveObject.getName())
, mDrawWindowClosedSprite(0xe0f87a29, interactiveObject.getName())
, mDoNotDrawWindowClosedSprite(0xdeecc064, interactiveObject.getName())
, mLeftClickPress(0x6955d309)
, mScreenCost(1.0f)
, mCloseCost(0.5f)
, mTextures(textures)
, mSoundPlayer(soundPlayer)
, mDaylightUI(daylightUI)
, mChatBoxUI(chatBoxUI)
, mWindowUIActive(false)
, mIsWindowScreen(false)
, mIsWindowClosed(false)
{
	mCallbackPairs.emplace_back(CallbackPair(std::bind(&WindowNode::addScreen, this), std::bind(&WindowNode::undoScreen, this)));
	mCallbackPairs.emplace_back(CallbackPair(std::bind(&WindowNode::closeWindow, this), std::bind(&WindowNode::openWindow, this)));
	mUIElemStates.emplace_back(true);
	mUIElemStates.emplace_back(true);
}

bool WindowNode::isWindowScreen() const
{
	return mIsWindowScreen;
}

bool WindowNode::isWindowClosed() const
{
	return mIsWindowClosed;
}

sf::FloatRect WindowNode::getBoundingRect() const
{
	return sf::FloatRect(mInteractiveObject.getCollisionBoxXCoord(), mInteractiveObject.getCollisionBoxYCoord()
		, mInteractiveObject.getCollisionBoxWidth(), mInteractiveObject.getCollisionBoxHeight());
}

bool WindowNode::passThrough() const
{
	const int enter = 0;
	bool passThrough = false;

	if (mIsWindowClosed)
	{
		const int diceRoll = trmb::randomInt(3); // ALW - 1/3 chance to pass through a closed door

		if (diceRoll == enter)
			passThrough = true;
	}
	else if (mIsWindowScreen)
	{
		const int diceRoll = trmb::randomInt(5); // ALW - 20% chance to pass through a screened door

		if (diceRoll == enter)
			passThrough = true;
	}
	else
		passThrough = true; // ALW - 100% chance to pass through a open door

	return passThrough;
}

void WindowNode::handleEvent(const trmb::Event &gameEvent)
{
	InteractiveNode::handleEvent(gameEvent);

	if (mDisableBuildMode)
	{
		mUIBundle.getWindowUI().hide();
	}
	else if (!mDisableInput
		  && !isMouseOverUI(mUIBundle.getChatBoxUI().getRect())
		  && !isMouseOverUI(mUIBundle.getDaylightUI().getRect()))
	{
		if (mWindowUIActivated.getType() == gameEvent.getType())
		{
			// ALW - This is a workaround to fix an issue where an object had selection and then another object of the same
			// ALW - type (barrel, door, window, clinic, house) was selected earlier in the SceneNode. What happened was
			// ALW - the newly selected object would enable the UI, but then the previously selected object would disable the
			// ALW - UI. This was not an issue in the reverse order. Now when an object is selected it sends a UI activated
			// ALW - message, so an object later in the SceneNode knows not to disable the UI.
			mWindowUIActive = true;
		}
		else if (mLeftClickPress == gameEvent.getType())
		{
			mPreviousSelectedState = mSelected;
			if (mSelected && !isMouseOverUI(mUIBundle.getWindowUI().getRect()))
			{
				mSelected = false;
			}

			if (isMouseOverObject()
				&& !isMouseOverUI(mUIBundle.getBarrelUI().getRect())
				&& !isMouseOverUI(mUIBundle.getDoorUI().getRect())
				&& !isMouseOverUI(mUIBundle.getWindowUI().getRect())
				&& !isMouseOverUI(mUIBundle.getClinicUI().getRect())
				&& !isMouseOverUI(mUIBundle.getHouseUI().getRect()))
			{
				mSelected = true;
				if (!mPreviousSelectedState)
					activate();
			}

			if (mPreviousSelectedState && !mSelected && !mWindowUIActive)
			{
				// ALW - Two scenarios are possible. First, an object earlier in the SceneNode is unselected. The UI
				// ALW - will be hidden. This behavior is ok, because if an object later in the SceneNode
				// ALW - is then selected it will unhide itself. Next, the object may be unselected, because the click
				// ALW - did not occur on an object of the same type. Here the UI is not in use, so resetting and hiding
				// ALW - the UI is the desired behavior.
				mUIBundle.getWindowUI().hide();
			}

			mWindowUIActive = false;
		}
	}
}

void WindowNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (mSelected)
	{
		target.draw(mHightlight, states);
	}
}

void WindowNode::updateCurrent(sf::Time)
{
	// ALW - Build Mode
	if (!mDisableBuildMode)
	{
		// ALW - Do not apply the InteractiveNode's transform, because there are multiple instances that share a single UndoUI. If the 
		// ALW - transform is applied then the UndoUI would be at the location specified by the translation of multiple InteractiveNodes
		// ALW - which is not correct. The workaround is to let the InteractiveNode's position default to 0.0f, 0.0f and then use the
		// ALW - InteractiveObjects coordinates to position the UndoUI in the world. This way the InteractiveNode's transform does not
		// ALW - need to be applied to the UndoUI. To keep the handler interface consistent the Identity transform is passed in and applied.
		sf::Transform transform = sf::Transform::Identity;

		mUIBundle.getWindowUI().handler(mWindow, mView, transform);
	}
}

void WindowNode::activate()
{
	updateUndoUI();
	mSoundPlayer.play(SoundEffects::ID::Object);
	InteractiveNode::sendEvent(mWindowUIActivated);
	// ALW - ChatBoxUI::UpdateText() can generate a mCreatePrompt event when an interactive object
	// ALW - is selected. This asynchronous event will force InteractiveNode classes to ignore
	// ALW - left and right click events. Then if <enter> is pressed an mEnter event will be
	// ALW - generated allowing InteractiveNode classes to handle left and right click events.
	// ALW - However, if another interactive object is selected that occurs before the currently
	// ALW - selected interactive object in the SceneNode then this newly selected interactive
	// ALW - object will cause ChatBoxUI::UpdateText() to be called which generates a mCreatePrompt.
	// ALW - InteractiveNodes are then forced to ignore left and right click events, so the original
	// ALW - interactive object will be left selected. To remedy this all InteractiveNodes deselect
	// ALW - themselves when a mCreatePrompt is generated. Immediately afterwards the InteractiveNode
	// ALW - that generated the mCreatePrompt is reselected.
	if (mIsWindowScreen && mIsWindowClosed)
	{
		mChatBoxUI.updateText(trmb::Localize::getInstance().getString("inspectScreenClosedWindow"));
	}
	else if (mIsWindowScreen)
	{
		mChatBoxUI.updateText(trmb::Localize::getInstance().getString("inspectScreenWindow"));
	}
	else if (mIsWindowClosed)
	{
		mChatBoxUI.updateText(trmb::Localize::getInstance().getString("inspectClosedWindow"));
	}
	else
	{
		mChatBoxUI.updateText(trmb::Localize::getInstance().getString("inspectWindow"));
	}

	mSelected = true;
}

void WindowNode::updateUndoUI()
{
	// ALW - Resizes UI to zero, so click detection does not occur.
	mUIBundle.getWindowUI().unhide();

	const float verticalBuffer = 10.0f;
	mUIBundle.getWindowUI().setPosition(sf::Vector2f(mInteractiveObject.getX() + mInteractiveObject.getWidth() / 2.0f
		, mInteractiveObject.getY() + mInteractiveObject.getHeight() + verticalBuffer));

	mUIBundle.getWindowUI().setCallbacks(mCallbackPairs);

	mUIBundle.getWindowUI().setUIElemState(mUIElemStates);

	// ALW - Tells the WindowUINode whether to draw the UI or not.
	if (mSelected)
		InteractiveNode::sendEvent(mDrawWindowUI);
	else
		InteractiveNode::sendEvent(mDoNotDrawWindowUI);
}

void WindowNode::addScreen()
{
	const std::size_t screenElement = 0;

	if (mDaylightUI.subtract(mScreenCost))
	{
		mIsWindowScreen = true;
		InteractiveNode::sendEvent(mDrawWindowScreenSprite);
		mChatBoxUI.updateText(trmb::Localize::getInstance().getString("purchaseScreenWindow"));
		mUIElemStates.at(screenElement) = false;
	}
	else
	{
		// ALW - There weren't enough hours of daylight left to "purchase" the item.
		// ALW - However, the button state changed. Reset it.
		mUIElemStates.at(screenElement) = true;
		mUIBundle.getWindowUI().setUIElemState(mUIElemStates);
		mChatBoxUI.updateText(trmb::Localize::getInstance().getString("daylightHours"));
	}
}

void WindowNode::undoScreen()
{
	mIsWindowScreen = false;
	InteractiveNode::sendEvent(mDoNotDrawWindowScreenSprite);
	mChatBoxUI.updateText(trmb::Localize::getInstance().getString("refundScreenWindow"));
	mDaylightUI.add(mScreenCost);
	const std::size_t screenElement = 0;
	mUIElemStates.at(screenElement) = true;
}

void WindowNode::closeWindow()
{
	const std::size_t windowElement = 1;

	if (mDaylightUI.subtract(mCloseCost))
	{
		mIsWindowClosed = true;
		InteractiveNode::sendEvent(mDrawWindowClosedSprite);
		mChatBoxUI.updateText(trmb::Localize::getInstance().getString("purchaseClosedWindow"));
		mUIElemStates.at(windowElement) = false;
	}
	else
	{
		// ALW - There weren't enough hours of daylight left to "purchase" the item.
		// ALW - However, the button state changed. Reset it.
		mUIElemStates.at(windowElement) = true;
		mUIBundle.getWindowUI().setUIElemState(mUIElemStates);
		mChatBoxUI.updateText(trmb::Localize::getInstance().getString("daylightHours"));
	}
}

void WindowNode::openWindow()
{
	mIsWindowClosed = false;
	InteractiveNode::sendEvent(mDoNotDrawWindowClosedSprite);
	mChatBoxUI.updateText(trmb::Localize::getInstance().getString("refundClosedWindow"));
	mDaylightUI.add(mCloseCost);
	const std::size_t windowElement = 1;
	mUIElemStates.at(windowElement) = true;
}
