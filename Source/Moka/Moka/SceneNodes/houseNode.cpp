#include "houseNode.h"
#include "../GameObjects/interactiveObject.h"
#include "../HUD/chatBox.h"
#include "../HUD/optionsUI.h"
#include "../HUD/undoUI.h"
#include "../Levels/uiBundle.h"
#include "../Resources/resourceIdentifiers.h"

#include "Trambo/Localize/localize.h"
#include "Trambo/Sounds/soundPlayer.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


HouseNode::HouseNode(const InteractiveObject &interactiveObject, const sf::RenderWindow &window, const sf::View &view, UIBundle &uiBundle
	, std::vector<sf::FloatRect> attachedRects, trmb::SoundPlayer &soundPlayer, ChatBox &chatBox)
: BuildingNode(interactiveObject, window, view, uiBundle, attachedRects)
, mHouseUIActivated(0xb5ba9eaf)
, mDrawHouseUI(0xc7353048)
, mDoNotDrawHouseUI(0x8e6093bf)
, mLeftClickPress(0x6955d309)
, mSoundPlayer(soundPlayer)
, mChatBox(chatBox)
, mHouseUIActive(false)
{
}

void HouseNode::handleEvent(const trmb::Event &gameEvent)
{
	InteractiveNode::handleEvent(gameEvent);

	if (!mDisableInput)
	{
		if (mHouseUIActivated.getType() == gameEvent.getType())
		{
			// ALW - This is a workaround to fix an issue where an object had selection and then another object of the same
			// ALW - type (barrel, door, window, clinic, house) was selected earlier in the SceneNode. What happened was
			// ALW - the newly selected object would enable the UI, but then the previously selected object would disable the
			// ALW - UI. This was not an issue in the reverse order. Now when an object is selected it sends a UI activated
			// ALW - message, so an object later in the SceneNode knows not to disable the UI.
			mHouseUIActive = true;
		}
		else if (mLeftClickPress == gameEvent.getType())
		{
			mPreviousSelectedState = mSelected;
			if (mSelected && !isMouseOverUI(mUIBundle.getHouseUI().getRect()))
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

			if (mPreviousSelectedState && !mSelected && mHouseUIActive)
			{
				// ALW - The object was unselected, because another object of the same type was selected.
				// ALW - Therefore, the object's UI is still in use. Only reset the UI.
				mUIBundle.getHouseUI().reset();
			}
			else if (mPreviousSelectedState && !mSelected && !mHouseUIActive)
			{
				// ALW - Two scenarios are possible. First, an object earlier in the SceneNode is unselected. The UI
				// ALW - will be reset and hidden. This behavior is ok, because if an object later in the SceneNode
				// ALW - is then selected it will unhide itself. Next, the object may be unselected, because the click
				// ALW - did not occur on an object of the same type. Here the UI is not in use, so resetting and hiding
				// ALW - the UI is the desired behavior.
				mUIBundle.getHouseUI().reset();
				mUIBundle.getHouseUI().hide();
			}

			mHouseUIActive = false;
		}
	}
}

void HouseNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (mSelected)
	{
		target.draw(mHightlight, states);
	}
}

void HouseNode::updateCurrent(sf::Time)
{
	// ALW - Do not apply the InteractiveNode's transform, because there are multiple instances that share a single OptionsUI. If the 
	// ALW - transform is applied then the OptionsUI would be at the location specified by the translation of multiple InteractiveNodes
	// ALW - which is not correct. The workaround is to let the InteractiveNode's position default to 0.0f, 0.0f and then use the
	// ALW - InteractiveObjects coordinates to position the OptionsUI in the world. This way the InteractiveNode's transform does not
	// ALW - need to be applied to the OptionsUI. To keep the handler interface consistent the Identity transform is passed in and applied.
	sf::Transform transform = sf::Transform::Identity;

	mUIBundle.getHouseUI().handler(mWindow, mView, transform);
}

void HouseNode::activate()
{
	updateOptionsUI();
	mSoundPlayer.play(SoundEffects::ID::Button);
	InteractiveNode::sendEvent(mHouseUIActivated);
	// ALW - ChatBox::UpdateText() can generate a mCreatePrompt event when an interactive object
	// ALW - is selected. This asynchronous event will force InteractiveNode classes to ignore
	// ALW - left and right click events. Then if <enter> is pressed an mEnter event will be
	// ALW - generated allowing InteractiveNode classes to handle left and right click events.
	// ALW - However, if another interactive object is selected that occurs before the currently
	// ALW - selected interactive object in the SceneNode then this newly selected interactive
	// ALW - object will cause ChatBox::UpdateText() to be called which generates a mCreatePrompt.
	// ALW - InteractiveNodes are then forced to ignore left and right click events, so the original
	// ALW - interactive object will be left selected. To remedy this all InteractiveNodes deselect
	// ALW - themselves when a mCreatePrompt is generated. Immediately afterwards the InteractiveNode
	// ALW - that generated the mCreatePrompt is reselected.
	mChatBox.updateText(trmb::Localize::getInstance().getString("inspectHouse"));
	mSelected = true;
}

void HouseNode::updateOptionsUI()
{
	// ALW - Resizes UI to zero, so click detection does not occur.
	mUIBundle.getHouseUI().unhide();

	const float verticalBuffer = 10.0f;
	mUIBundle.getHouseUI().setPosition(sf::Vector2f(mInteractiveObject.getX() + mInteractiveObject.getWidth() / 2.0f
		, mInteractiveObject.getY() + mInteractiveObject.getHeight() + verticalBuffer));

	mUIBundle.getHouseUI().updateIncDecCallbacks(std::bind(&HouseNode::incrementPurchaseBedNet, this)
		, std::bind(&HouseNode::decrementPurchaseBedNet, this)
		, std::bind(&HouseNode::incrementRepair, this)
		, std::bind(&HouseNode::decrementRepair, this));

	// ALW - Tells the HouseUINode whether to draw the UI or not.
	if (mSelected)
		InteractiveNode::sendEvent(mDrawHouseUI);
	else
		InteractiveNode::sendEvent(mDoNotDrawHouseUI);
}

void HouseNode::incrementPurchaseBedNet()
{
	// ALW - TODO - Increment resource
}

void HouseNode::decrementPurchaseBedNet()
{
	// ALW - TODO - Decrement resource
}

void HouseNode::incrementRepair()
{
	// ALW - TODO - Increment resource
}

void HouseNode::decrementRepair()
{
	// ALW - TODO - Decrement resource
}
