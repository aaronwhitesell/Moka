#include "doorNode.h"
#include "../GameObjects/interactiveObject.h"
#include "../HUD/chatBoxUI.h"
#include "../HUD/daylightUI.h"
#include "../HUD/optionsUI.h"
#include "../HUD/uiBundle.h"
#include "../Resources/resourceIdentifiers.h"

#include "Trambo/Localize/localize.h"
#include "Trambo/Sounds/soundPlayer.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


DoorNode::DoorNode(const InteractiveObject &interactiveObject, const sf::RenderWindow &window, const sf::View &view
	, UIBundle &uiBundle, const trmb::TextureHolder &textures, trmb::SoundPlayer &soundPlayer, DaylightUI &daylightUI
	, ChatBoxUI &chatBoxUI)
: PreventionNode(interactiveObject, window, view, uiBundle)
, mDoorUIActivated(0xa704ae55)
, mDrawDoorUI(0x7cf851c6)
, mDoNotDrawDoorUI(0xc0a53a4d)
, mDrawDoorSprite(0x5ea6cda9, interactiveObject.getName())
, mDoNotDrawDoorSprite(0x918c6b78, interactiveObject.getName())
, mLeftClickPress(0x6955d309)
, mCloseCost(0.5f)
, mTextures(textures)
, mSoundPlayer(soundPlayer)
, mDaylightUI(daylightUI)
, mChatBoxUI(chatBoxUI)
, mDoorUIActive(false)
, mIsDoorClosed(false)
{
	mCallbackPairs.emplace_back(CallbackPair(std::bind(&DoorNode::closeDoor, this), std::bind(&DoorNode::openDoor, this)));
	mUIElemStates.emplace_back(true);
}

void DoorNode::handleEvent(const trmb::Event &gameEvent)
{
	InteractiveNode::handleEvent(gameEvent);

	if (mDisableBuildMode)
	{
		mUIBundle.getDoorUI().hide();
	}
	else if (!mDisableInput
		  && !isMouseOverUI(mUIBundle.getChatBoxUI().getRect())
		  && !isMouseOverUI(mUIBundle.getDaylightUI().getRect()))
	{
		if (mDoorUIActivated.getType() == gameEvent.getType())
		{
			// ALW - This is a workaround to fix an issue where an object had selection and then another object of the same
			// ALW - type (barrel, door, window, clinic, house) was selected earlier in the SceneNode. What happened was
			// ALW - the newly selected object would enable the UI, but then the previously selected object would disable the
			// ALW - UI. This was not an issue in the reverse order. Now when an object is selected it sends a UI activated
			// ALW - message, so an object later in the SceneNode knows not to disable the UI.
			mDoorUIActive = true;
		}
		else if (mLeftClickPress == gameEvent.getType())
		{
			mPreviousSelectedState = mSelected;
			if (mSelected && !isMouseOverUI(mUIBundle.getDoorUI().getRect()))
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

			if (mPreviousSelectedState && !mSelected && !mDoorUIActive)
			{
				// ALW - Two scenarios are possible. First, an object earlier in the SceneNode is unselected. The UI
				// ALW - will be hidden. This behavior is ok, because if an object later in the SceneNode
				// ALW - is then selected it will unhide itself. Next, the object may be unselected, because the click
				// ALW - did not occur on an object of the same type. Here the UI is not in use, so resetting and hiding
				// ALW - the UI is the desired behavior.
				mUIBundle.getDoorUI().hide();
			}

			mDoorUIActive = false;
		}
	}
}

void DoorNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (mSelected)
	{
		target.draw(mHightlight, states);
	}
}

void DoorNode::updateCurrent(sf::Time)
{
	// ALW - Do not apply the InteractiveNode's transform, because there are multiple instances that share a single UndoUI. If the 
	// ALW - transform is applied then the UndoUI would be at the location specified by the translation of multiple InteractiveNodes
	// ALW - which is not correct. The workaround is to let the InteractiveNode's position default to 0.0f, 0.0f and then use the
	// ALW - InteractiveObjects coordinates to position the UndoUI in the world. This way the InteractiveNode's transform does not
	// ALW - need to be applied to the UndoUI. To keep the handler interface consistent the Identity transform is passed in and applied.
	sf::Transform transform = sf::Transform::Identity;

	mUIBundle.getDoorUI().handler(mWindow, mView, transform);
}

void DoorNode::activate()
{
	updateUndoUI();	
	mSoundPlayer.play(SoundEffects::ID::Button);
	InteractiveNode::sendEvent(mDoorUIActivated);
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
	if (mIsDoorClosed)
		mChatBoxUI.updateText(trmb::Localize::getInstance().getString("inspectClosedDoor"));
	else
		mChatBoxUI.updateText(trmb::Localize::getInstance().getString("inspectDoor"));
	mSelected = true;
}

void DoorNode::updateUndoUI()
{
	// ALW - Resizes UI to zero, so click detection does not occur.
	mUIBundle.getDoorUI().unhide();

	// ALW - Tells the DoorUINode whether to draw the UI or not.
	if (mSelected)
		InteractiveNode::sendEvent(mDrawDoorUI);
	else
		InteractiveNode::sendEvent(mDoNotDrawDoorUI);

	const float verticalBuffer = 10.0f;
	mUIBundle.getDoorUI().setPosition(sf::Vector2f(mInteractiveObject.getX() + mInteractiveObject.getWidth() / 2.0f
		, mInteractiveObject.getY() + mInteractiveObject.getHeight() + verticalBuffer));

	mUIBundle.getDoorUI().setCallbacks(mCallbackPairs);

	mUIBundle.getDoorUI().setUIElemState(mUIElemStates);
}

void DoorNode::closeDoor()
{
	if (mDaylightUI.subtract(mCloseCost))
	{
		mIsDoorClosed = true;
		InteractiveNode::sendEvent(mDrawDoorSprite);
		mChatBoxUI.updateText(trmb::Localize::getInstance().getString("purchaseClosedDoor"));
		mUIElemStates.front() = false;
	}
	else
	{
		// ALW - There weren't enough hours of daylight left to "purchase" the item.
		// ALW - However, the button state changed. Reset it.
		mUIElemStates.front() = true;
		mUIBundle.getDoorUI().setUIElemState(mUIElemStates);
		mChatBoxUI.updateText(trmb::Localize::getInstance().getString("daylightHours"));
	}
}

void DoorNode::openDoor()
{
	mIsDoorClosed = false;
	InteractiveNode::sendEvent(mDoNotDrawDoorSprite);
	mChatBoxUI.updateText(trmb::Localize::getInstance().getString("refundClosedDoor"));
	mDaylightUI.add(mCloseCost);
	mUIElemStates.front() = true;
}
