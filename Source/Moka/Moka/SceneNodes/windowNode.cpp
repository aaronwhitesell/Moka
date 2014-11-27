#include "windowNode.h"
#include "../GameObjects/interactiveObject.h"
#include "../HUD/chatBox.h"
#include "../HUD/optionsUI.h"
#include "../Levels/uiBundle.h"
#include "../Resources/resourceIdentifiers.h"

#include "Trambo/Events/event.h"
#include "Trambo/Localize/localize.h"
#include "Trambo/Sounds/soundPlayer.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


WindowNode::WindowNode(const InteractiveObject &interactiveObject, const sf::RenderWindow &window, const sf::View &view
	, UIBundle &uiBundle, trmb::SoundPlayer &soundPlayer, ChatBox &chatBox)
: PreventionNode(interactiveObject, window, view, uiBundle)
, mWindowUIActivated(0x961e8d0b)
, mLeftClickPress(0x6955d309)
, mSoundPlayer(soundPlayer)
, mChatBox(chatBox)
, mWindowUIActive(false)
{
	mCallbackPairs.emplace_back(CallbackPair(std::bind(&WindowNode::addScreen, this), std::bind(&WindowNode::undoScreen, this)));
	mCallbackPairs.emplace_back(CallbackPair(std::bind(&WindowNode::closeWindow, this), std::bind(&WindowNode::openWindow, this)));
	mUIElemStates.emplace_back(true);
	mUIElemStates.emplace_back(true);
}

void WindowNode::handleEvent(const trmb::Event &gameEvent)
{
	InteractiveNode::handleEvent(gameEvent);

	if (!mDisableInput)
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
				// ALW - The object was unselected, because the click did not occur on an object of the same type.
				// ALW - Therefore, the UI is not in use. Disable the UI.
				mUIBundle.getWindowUI().disable();
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
		target.draw(mUIBundle.getWindowUI(), states);
	}
}

void WindowNode::updateCurrent(sf::Time)
{
	// ALW - Do not apply the InteractiveNode's transform, because there are multiple instances that share a single UndoUI. If the 
	// ALW - transform is applied then the UndoUI would be at the location specified by the translation of multiple InteractiveNodes
	// ALW - which is not correct. The workaround is to let the InteractiveNode's position default to 0.0f, 0.0f and then use the
	// ALW - InteractiveObjects coordinates to position the UndoUI in the world. This way the InteractiveNode's transform does not
	// ALW - need to be applied to the UndoUI. To keep the handler interface consistent the Identity transform is passed in and applied.
	sf::Transform transform = sf::Transform::Identity;

	mUIBundle.getWindowUI().handler(mWindow, mView, transform);
}

void WindowNode::activate()
{
	updateUndoUI();	
	mSoundPlayer.play(SoundEffects::ID::Button);
	InteractiveNode::sendEvent(mWindowUIActivated);
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
	mChatBox.updateText(trmb::Localize::getInstance().getString("inspectWindow"));
	mSelected = true;
}

void WindowNode::updateUndoUI()
{
	mUIBundle.getWindowUI().enable();

	const float verticalBuffer = 10.0f;
	mUIBundle.getWindowUI().setPosition(sf::Vector2f(mInteractiveObject.getX() + mInteractiveObject.getWidth() / 2.0f
		, mInteractiveObject.getY() + mInteractiveObject.getHeight() + verticalBuffer));

	mUIBundle.getWindowUI().setCallbacks(mCallbackPairs);

	mUIBundle.getWindowUI().setUIElemState(mUIElemStates);
}

void WindowNode::addScreen()
{
	const std::size_t screenElement = 0;
	mUIElemStates.at(screenElement) = false;
	// ALW - TODO - Add screen
}

void WindowNode::undoScreen()
{
	const std::size_t screenElement = 0;
	mUIElemStates.at(screenElement) = true;
	// ALW - TODO - Remove screen
}

void WindowNode::closeWindow()
{
	const std::size_t windowElement = 1;
	mUIElemStates.at(windowElement) = false;
	// ALW - TODO - Close window
}

void WindowNode::openWindow()
{
	const std::size_t windowElement = 1;
	mUIElemStates.at(windowElement) = true;
	// ALW - TODO - Open window
}
