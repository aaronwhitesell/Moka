#include "barrelNode.h"
#include "../GameObjects/interactiveObject.h"
#include "../HUD/chatBox.h"
#include "../HUD/optionsUI.h"
#include "../Levels/uiBundle.h"
#include "../Resources/resourceIdentifiers.h"

#include "Trambo/Localize/localize.h"
#include "Trambo/Sounds/soundPlayer.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>



BarrelNode::BarrelNode(const InteractiveObject &interactiveObject, const sf::RenderWindow &window, const sf::View &view
	, UIBundle &uiBundle, const trmb::TextureHolder &textures, trmb::SoundPlayer &soundPlayer, ChatBox &chatBox)
: PreventionNode(interactiveObject, window, view, uiBundle)
, mBarrelUIActivated(0x10a1b42f)
, mDrawBarrelUI(0xcfdb933d)
, mDoNotDrawBarrelUI(0x210832f5)
, mLeftClickPress(0x6955d309)
, mTextures(textures)
, mSoundPlayer(soundPlayer)
, mChatBox(chatBox)
, mBarrelUIActive(false)
, mBarrelSprite(mTextures.get(Textures::ID::Tiles), sf::IntRect(576, 640, 64, 64))
, mIsBarrelCovered(false)
{
	mBarrelSprite.setPosition(sf::Vector2f(mInteractiveObject.getPosX0(), mInteractiveObject.getPosY0()));
	mCallbackPairs.emplace_back(CallbackPair(std::bind(&BarrelNode::addCover, this), std::bind(&BarrelNode::undoCover, this)));
	mUIElemStates.emplace_back(true);
}

void BarrelNode::handleEvent(const trmb::Event &gameEvent)
{
	InteractiveNode::handleEvent(gameEvent);

	if (!mDisableInput)
	{
		if (mBarrelUIActivated.getType() == gameEvent.getType())
		{
			// ALW - This is a workaround to fix an issue where an object had selection and then another object of the same
			// ALW - type (barrel, door, window, clinic, house) was selected earlier in the SceneNode. What happened was
			// ALW - the newly selected object would enable the UI, but then the previously selected object would disable the
			// ALW - UI. This was not an issue in the reverse order. Now when an object is selected it sends a UI activated
			// ALW - message, so an object later in the SceneNode knows not to disable the UI.
			mBarrelUIActive = true;
		}
		else if (mLeftClickPress == gameEvent.getType())
		{
			mPreviousSelectedState = mSelected;
			if (mSelected && !isMouseOverUI(mUIBundle.getBarrelUI().getRect()))
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

			if (mPreviousSelectedState && !mSelected && !mBarrelUIActive)
			{
				// ALW - Two scenarios are possible. First, an object earlier in the SceneNode is unselected. The UI
				// ALW - will be hidden. This behavior is ok, because if an object later in the SceneNode
				// ALW - is then selected it will unhide itself. Next, the object may be unselected, because the click
				// ALW - did not occur on an object of the same type. Here the UI is not in use, so resetting and hiding
				// ALW - the UI is the desired behavior.
				mUIBundle.getBarrelUI().hide();
			}

			mBarrelUIActive = false;
		}
	}
}

void BarrelNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (mIsBarrelCovered)
		target.draw(mBarrelSprite, states);

	if (mSelected)
	{
		target.draw(mHightlight, states);
	}
}

void BarrelNode::updateCurrent(sf::Time)
{
	// ALW - Do not apply the InteractiveNode's transform, because there are multiple instances that share a single UndoUI. If the 
	// ALW - transform is applied then the UndoUI would be at the location specified by the translation of multiple InteractiveNodes
	// ALW - which is not correct. The workaround is to let the InteractiveNode's position default to 0.0f, 0.0f and then use the
	// ALW - InteractiveObjects coordinates to position the UndoUI in the world. This way the InteractiveNode's transform does not
	// ALW - need to be applied to the UndoUI. To keep the handler interface consistent the Identity transform is passed in and applied.
	sf::Transform transform = sf::Transform::Identity;

	mUIBundle.getBarrelUI().handler(mWindow, mView, transform);
}

void BarrelNode::activate()
{
	updateUndoUI();	
	mSoundPlayer.play(SoundEffects::ID::Button);
	InteractiveNode::sendEvent(mBarrelUIActivated);
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
	if (mIsBarrelCovered)
		mChatBox.updateText(trmb::Localize::getInstance().getString("inspectCoveredBarrel"));
	else
		mChatBox.updateText(trmb::Localize::getInstance().getString("inspectBarrel"));
	mSelected = true;
}

void BarrelNode::updateUndoUI()
{
	// ALW - Resizes UI to zero, so click detection does not occur.
	mUIBundle.getBarrelUI().unhide();

	const float verticalBuffer = 10.0f;
	mUIBundle.getBarrelUI().setPosition(sf::Vector2f(mInteractiveObject.getX() + mInteractiveObject.getWidth() / 2.0f
		, mInteractiveObject.getY() + mInteractiveObject.getHeight() + verticalBuffer));

	mUIBundle.getBarrelUI().setCallbacks(mCallbackPairs);

	mUIBundle.getBarrelUI().setUIElemState(mUIElemStates);

	// ALW - Tells the BarrelUINode whether to draw the UI or not.
	if (mSelected)
		InteractiveNode::sendEvent(mDrawBarrelUI);
	else
		InteractiveNode::sendEvent(mDoNotDrawBarrelUI);
}

void BarrelNode::addCover()
{
	mIsBarrelCovered = true;
	mChatBox.updateText(trmb::Localize::getInstance().getString("purchaseCover"));
	mUIElemStates.front() = false;
}

void BarrelNode::undoCover()
{
	mIsBarrelCovered = false;
	mChatBox.updateText(trmb::Localize::getInstance().getString("refundCover"));
	mUIElemStates.front() = true;
}
