#include "clinicNode.h"
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

#include <cassert>


ClinicNode::ClinicNode(const InteractiveObject &interactiveObject, const sf::RenderWindow &window, const sf::View &view
	, UIBundle &mUIBundle, std::vector<sf::FloatRect> attachedRects, trmb::SoundPlayer &soundPlayer, ChatBox &chatBox)
: BuildingNode(interactiveObject, window, view, mUIBundle, attachedRects)
, mClinicUIActivated(0xcb9e3f21)
, mDrawClinicUI(0x1363b002)
, mDoNotDrawClinicUI(0x7ccd235d)
, mDrawSmallRDTCrateSprite(0x3377fe93, interactiveObject.getName())
, mDrawLargeRDTCrateSprite(0x358963ee, interactiveObject.getName())
, mDrawRDTBarrelSprite(0xafbc42ce, interactiveObject.getName())
, mDoNotRDTDrawSprite(0xf08738aa, interactiveObject.getName())
, mDrawSmallACTCrateSprite(0x7f31c614, interactiveObject.getName())
, mDrawLargeACTCrateSprite(0x4a640fb6, interactiveObject.getName())
, mDrawACTBarrelSprite(0xad344f84, interactiveObject.getName())
, mDoNotACTDrawSprite(0xd6fe68f2, interactiveObject.getName())
, mLeftClickPress(0x6955d309)
, mSoundPlayer(soundPlayer)
, mChatBox(chatBox)
, mClinicUIActive(false)
, mRDTCount(0)
, mACTCount(0)
{
}

void ClinicNode::handleEvent(const trmb::Event &gameEvent)
{
	InteractiveNode::handleEvent(gameEvent);

	if (!mDisableInput)
	{
		if (mClinicUIActivated.getType() == gameEvent.getType())
		{
			// ALW - This is a workaround to fix an issue where an object had selection and then another object of the same
			// ALW - type (barrel, door, window, clinic, house) was selected earlier in the SceneNode. What happened was
			// ALW - the newly selected object would enable the UI, but then the previously selected object would disable the
			// ALW - UI. This was not an issue in the reverse order. Now when an object is selected it sends a UI activated
			// ALW - message, so an object later in the SceneNode knows not to disable the UI.
			mClinicUIActive = true;
		}
		else if (mLeftClickPress == gameEvent.getType())
		{
			mPreviousSelectedState = mSelected;
			if (mSelected && !isMouseOverUI(mUIBundle.getClinicUI().getRect()))
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

			if (mPreviousSelectedState && !mSelected && mClinicUIActive)
			{
				// ALW - The object was unselected, because another object of the same type was selected.
				// ALW - Therefore, the object's UI is still in use. Only reset the UI.
				mUIBundle.getClinicUI().reset();
			}
			else if (mPreviousSelectedState && !mSelected && !mClinicUIActive)
			{
				// ALW - Two scenarios are possible. First, an object earlier in the SceneNode is unselected. The UI
				// ALW - will be reset and hidden. This behavior is ok, because if an object later in the SceneNode
				// ALW - is then selected it will unhide itself. Next, the object may be unselected, because the click
				// ALW - did not occur on an object of the same type. Here the UI is not in use, so resetting and hiding
				// ALW - the UI is the desired behavior.
				mUIBundle.getClinicUI().reset();
				mUIBundle.getClinicUI().hide();
			}

			mClinicUIActive = false;
		}
	}
}

void ClinicNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (mSelected)
	{
		target.draw(mHightlight, states);
	}
}

void ClinicNode::updateCurrent(sf::Time)
{
	// ALW - Do not apply the InteractiveNode's transform, because there are multiple instances that share a single OptionsUI. If the 
	// ALW - transform is applied then the OptionsUI would be at the location specified by the translation of multiple InteractiveNodes
	// ALW - which is not correct. The workaround is to let the InteractiveNode's position default to 0.0f, 0.0f and then use the
	// ALW - InteractiveObjects coordinates to position the OptionsUI in the world. This way the InteractiveNode's transform does not
	// ALW - need to be applied to the OptionsUI. To keep the handler interface consistent the Identity transform is passed in and applied.
	sf::Transform transform = sf::Transform::Identity;

	mUIBundle.getClinicUI().handler(mWindow, mView, transform);
}

void ClinicNode::activate()
{
	updateOptionsUI();
	mSoundPlayer.play(SoundEffects::ID::Button);
	InteractiveNode::sendEvent(mClinicUIActivated);
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
	mChatBox.updateText(trmb::Localize::getInstance().getString("inspectClinic"));
	mSelected = true;
}

void ClinicNode::updateOptionsUI()
{
	// ALW - Resizes UI to zero, so click detection does not occur.
	mUIBundle.getClinicUI().unhide();

	const float verticalBuffer = 10.0f;
	mUIBundle.getClinicUI().setPosition(sf::Vector2f(mInteractiveObject.getX() + mInteractiveObject.getWidth() / 2.0f
		, mInteractiveObject.getY() + mInteractiveObject.getHeight() + verticalBuffer));

	mUIBundle.getClinicUI().updateIncDecCallbacks(std::bind(&ClinicNode::incrementPurchaseRDT, this)
		, std::bind(&ClinicNode::decrementPurchaseRDT, this)
		, std::bind(&ClinicNode::incrementPurchaseACT, this)
		, std::bind(&ClinicNode::decrementPurchaseACT, this));

	// ALW - Tells the ClinicUINode whether to draw the UI or not.
	if (mSelected)
		InteractiveNode::sendEvent(mDrawClinicUI);
	else
		InteractiveNode::sendEvent(mDoNotDrawClinicUI);
}

void ClinicNode::incrementPurchaseRDT()
{
	++mRDTCount;
	calculateRDTEvent();
	updateRDTDisableState();
	mChatBox.updateText(trmb::Localize::getInstance().getString("purchaseRDT"));
}

void ClinicNode::decrementPurchaseRDT()
{
	--mRDTCount;
	calculateRDTEvent();
	updateRDTDisableState();
	mChatBox.updateText(trmb::Localize::getInstance().getString("refundRDT"));
}

void ClinicNode::incrementPurchaseACT()
{
	++mACTCount;
	calculateACTEvent();
	updateACTDisableState();
	mChatBox.updateText(trmb::Localize::getInstance().getString("purchaseACT"));
}

void ClinicNode::decrementPurchaseACT()
{
	--mACTCount;
	calculateACTEvent();
	updateACTDisableState();
	mChatBox.updateText(trmb::Localize::getInstance().getString("refundACT"));
}

void ClinicNode::calculateRDTEvent()
{
	if (NoRDTs == mRDTCount)
	{
		InteractiveNode::sendEvent(mDoNotRDTDrawSprite);
	}
	else if (SmallRDTCrate == mRDTCount)
	{
		InteractiveNode::sendEvent(mDrawSmallRDTCrateSprite);
	}
	else if (LargeRDTCrate == mRDTCount)
	{
		InteractiveNode::sendEvent(mDrawLargeRDTCrateSprite);
	}
	else if (RDTBarrel == mRDTCount)
	{
		InteractiveNode::sendEvent(mDrawRDTBarrelSprite);
	}
	else
	{
		assert(("The RDT count is out of range!", false));
	}
}

void ClinicNode::calculateACTEvent()
{
	if (NoACTs == mACTCount)
	{
		InteractiveNode::sendEvent(mDoNotACTDrawSprite);
	}
	else if (SmallACTCrate == mACTCount)
	{
		InteractiveNode::sendEvent(mDrawSmallACTCrateSprite);
	}
	else if (LargeACTCrate == mACTCount)
	{
		InteractiveNode::sendEvent(mDrawLargeACTCrateSprite);
	}
	else if (ACTBarrel == mACTCount)
	{
		InteractiveNode::sendEvent(mDrawACTBarrelSprite);
	}
	else
	{
		assert(("The ACT count is out of range!", false));
	}
}

void ClinicNode::updateRDTDisableState()
{
	if (MinRDTCount == mRDTCount)
	{
		mUIBundle.getClinicUI().setDisableDecrementButtonOfLHSTab(true);
	}
	else if (MaxRDTCount == mRDTCount)
	{
		mUIBundle.getClinicUI().setDisableIncrementButtonOfLHSTab(true);
	}
	else
	{
		mUIBundle.getClinicUI().setDisableDecrementButtonOfLHSTab(false);
		mUIBundle.getClinicUI().setDisableIncrementButtonOfLHSTab(false);
	}
}

void ClinicNode::updateACTDisableState()
{
	if (MinACTCount == mACTCount)
	{
		mUIBundle.getClinicUI().setDisableDecrementButtonOfRHSTab(true);
	}
	else if (MaxACTCount == mACTCount)
	{
		mUIBundle.getClinicUI().setDisableIncrementButtonOfRHSTab(true);
	}
	else
	{
		mUIBundle.getClinicUI().setDisableDecrementButtonOfRHSTab(false);
		mUIBundle.getClinicUI().setDisableIncrementButtonOfRHSTab(false);
	}
}
