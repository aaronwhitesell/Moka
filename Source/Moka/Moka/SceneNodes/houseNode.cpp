#include "houseNode.h"
#include "../GameObjects/interactiveObject.h"
#include "../HUD/chatBoxUI.h"
#include "../HUD/daylightUI.h"
#include "../HUD/optionsUI.h"
#include "../HUD/undoUI.h"
#include "../HUD/uiBundle.h"
#include "../Resources/resourceIdentifiers.h"

#include "Trambo/Localize/localize.h"
#include "Trambo/Sounds/soundPlayer.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

#include <cassert>


HouseNode::HouseNode(const InteractiveObject &interactiveObject, const sf::RenderWindow &window, const sf::View &view, UIBundle &uiBundle
	, std::vector<sf::FloatRect> attachedRects, trmb::SoundPlayer &soundPlayer, DaylightUI &daylightUI, ChatBoxUI &chatBoxUI)
: BuildingNode(interactiveObject, window, view, uiBundle, attachedRects)
, mHouseUIActivated(0xb5ba9eaf)
, mAddNet1(0x43702f1a, interactiveObject.getName())
, mAddNet2(0xc84d3fea, interactiveObject.getName())
, mAddNet3(0x68edab82, interactiveObject.getName())
, mRemoveNet1(0xe1636e98, interactiveObject.getName())
, mRemoveNet2(0xadd9292e, interactiveObject.getName())
, mRemoveNet3(0x15f4a6d9, interactiveObject.getName())
, mRepairNet1(0xbd7bb606, interactiveObject.getName())
, mRepairNet2(0x1b5cf244, interactiveObject.getName())
, mRepairNet3(0x2bf40c2c, interactiveObject.getName())
, mUnrepairNet1(0x529e6299, interactiveObject.getName())
, mUnrepairNet2(0xfeb3945, interactiveObject.getName())
, mUnrepairNet3(0x2aac4ae8, interactiveObject.getName())
, mDrawHouseUI(0xc7353048)
, mDoNotDrawHouseUI(0x8e6093bf)
, mLeftClickPress(0x6955d309)
, mNetCost(1.0f)
, mRepairCost(0.5f)
, mSoundPlayer(soundPlayer)
, mDaylightUI(daylightUI)
, mChatBoxUI(chatBoxUI)
, mHouseUIActive(false)
, mTotalBeds(interactiveObject.getBeds())
, mTotalOldNets(interactiveObject.getNets())
, mNewNetCount(0)
, mRepairCount(0)
{
	updateNetDisableState();
	updateRepairDisableState();
}

void HouseNode::handleEvent(const trmb::Event &gameEvent)
{
	InteractiveNode::handleEvent(gameEvent);

	if (!mDisableInput
		&& !isMouseOverUI(mUIBundle.getChatBoxUI().getRect())
		&& !isMouseOverUI(mUIBundle.getDaylightUI().getRect()))
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
	mChatBoxUI.updateText(trmb::Localize::getInstance().getString("inspectHouse"));
	mSelected = true;
}

void HouseNode::updateOptionsUI()
{
	// ALW - Resizes UI to zero, so click detection does not occur.
	mUIBundle.getHouseUI().unhide();

	updateNetDisableState();
	updateRepairDisableState();

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
	if (mDaylightUI.subtract(mNetCost))
	{
		++mNewNetCount;
		calculateNetPurchaseEvent();
		updateNetDisableState();
		mChatBoxUI.updateText(trmb::Localize::getInstance().getString("purchaseNet"));
	}
	else
	{
		mChatBoxUI.updateText(trmb::Localize::getInstance().getString("daylightHours"));
	}
}

void HouseNode::decrementPurchaseBedNet()
{
	--mNewNetCount;
	calculateNetRefundEvent();
	updateNetDisableState();
	mChatBoxUI.updateText(trmb::Localize::getInstance().getString("refundNet"));
	mDaylightUI.add(mNetCost);
}

void HouseNode::incrementRepair()
{
	if (mDaylightUI.subtract(mRepairCost))
	{
		++mRepairCount;
		calculateRepairPurchaseEvent();
		updateRepairDisableState();
		mChatBoxUI.updateText(trmb::Localize::getInstance().getString("purchaseRepair"));
	}
	else
	{
		mChatBoxUI.updateText(trmb::Localize::getInstance().getString("daylightHours"));
	}
}

void HouseNode::decrementRepair()
{
	--mRepairCount;
	calculateRepairRefundEvent();
	updateRepairDisableState();
	mChatBoxUI.updateText(trmb::Localize::getInstance().getString("refundRepair"));
	mDaylightUI.add(mRepairCost);
}

void HouseNode::calculateNetPurchaseEvent()
{
	int netCount = mTotalOldNets + mNewNetCount;
	assert(("The net count is out of range!", 0 <= mNewNetCount && netCount <= mTotalBeds));

	switch (netCount)
	{
	case OneNet:
		InteractiveNode::sendEvent(mAddNet1);
		break;
	case TwoNets:
		InteractiveNode::sendEvent(mAddNet2);
		break;
	case ThreeNets:
		InteractiveNode::sendEvent(mAddNet3);
		break;
	default:
		assert(("The net count is out of range!", false));
	}
}

void HouseNode::calculateNetRefundEvent()
{
	int netCount = mTotalOldNets + mNewNetCount;
	assert(("The net count is out of range!", 0 <= mNewNetCount && netCount <= mTotalBeds));

	switch (netCount)
	{
	case NoNets:
		InteractiveNode::sendEvent(mRemoveNet1);
		break;
	case OneNet:
		InteractiveNode::sendEvent(mRemoveNet2);
		break;
	case TwoNets:
		InteractiveNode::sendEvent(mRemoveNet3);
		break;
	default:
		assert(("The net count is out of range!", false));
	}
}

void HouseNode::calculateRepairPurchaseEvent()
{
	assert(("The repair count is out of range!", 0 <= mRepairCount && mRepairCount <= mTotalOldNets));

	switch (mRepairCount)
	{
	case OneNet:
		InteractiveNode::sendEvent(mRepairNet1);
		break;
	case TwoNets:
		InteractiveNode::sendEvent(mRepairNet2);
		break;
	case ThreeNets:
		InteractiveNode::sendEvent(mRepairNet3);
		break;
	default:
		assert(("The net count is out of range!", false));
	}
}

void HouseNode::calculateRepairRefundEvent()
{
	assert(("The repair count is out of range!", 0 <= mRepairCount && mRepairCount <= mTotalOldNets));

	switch (mRepairCount)
	{
	case NoNets:
		InteractiveNode::sendEvent(mUnrepairNet1);
		break;
	case OneNet:
		InteractiveNode::sendEvent(mUnrepairNet2);
		break;
	case TwoNets:
		InteractiveNode::sendEvent(mUnrepairNet3);
		break;
	default:
		assert(("The net count is out of range!", false));
	}
}

void HouseNode::updateNetDisableState()
{
	const int minNewNets = 0;
	int netCount = mTotalOldNets + mNewNetCount;
	assert(("The net count is out of range!", minNewNets <= mNewNetCount && netCount <= mTotalBeds));

	if (minNewNets == mNewNetCount)
	{
		mUIBundle.getHouseUI().setDisableDecrementButtonOfLHSTab(true, true);
	}
	else
	{
		mUIBundle.getHouseUI().setDisableDecrementButtonOfLHSTab(false, false);
	}

	if (mTotalBeds == netCount)
	{
		mUIBundle.getHouseUI().setDisableIncrementButtonOfLHSTab(true, true);
	}
	else
	{
		mUIBundle.getHouseUI().setDisableIncrementButtonOfLHSTab(false, false);
	}
}

void HouseNode::updateRepairDisableState()
{
	const int minOldNets = 0;
	assert(("The old net count is out of range!", minOldNets <= mRepairCount && mRepairCount <= mTotalOldNets));

	if (minOldNets == mRepairCount)
	{
		mUIBundle.getHouseUI().setDisableDecrementButtonOfRHSTab(true, true);
	}
	else
	{
		mUIBundle.getHouseUI().setDisableDecrementButtonOfRHSTab(false, false);
	}

	if (mTotalOldNets == mRepairCount)
	{
		mUIBundle.getHouseUI().setDisableIncrementButtonOfRHSTab(true, true);
	}
	else
	{
		mUIBundle.getHouseUI().setDisableIncrementButtonOfRHSTab(false, false);
	}
}
