#include "houseNode.h"
#include "../GameObjects/interactiveObject.h"
#include "../HUD/chatBox.h"
#include "../HUD/optionsUI.h"
#include "../HUD/undoUI.h"
#include "../Levels/uiBundle.h"
#include "../Resources/resourceIdentifiers.h"

#include "Trambo/Events/event.h"
#include "Trambo/Localize/localize.h"
#include "Trambo/Sounds/soundPlayer.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


HouseNode::HouseNode(const InteractiveObject &interactiveObject, const sf::RenderWindow &window, const sf::View &view, UIBundle &uiBundle
	, std::vector<sf::FloatRect> attachedRects, trmb::SoundPlayer &soundPlayer, ChatBox &chatBox)
: BuildingNode(interactiveObject, window, view, uiBundle, attachedRects)
, mLeftClickPress(0x6955d309)
, mSoundPlayer(soundPlayer)
, mChatBox(chatBox)
{
}

void HouseNode::handleEvent(const trmb::Event &gameEvent)
{
	InteractiveNode::handleEvent(gameEvent);

	if (!mDisableInput)
	{
		if (mLeftClickPress == gameEvent.getType())
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

			if (mPreviousSelectedState && !mSelected)
			{
				// ALW - The house was unselected.
				mUIBundle.getHouseUI().deactivate();
			}
		}
	}
}

void HouseNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (mSelected)
	{
		target.draw(mHightlight, states);
		target.draw(mUIBundle.getHouseUI(), states);
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
	const float verticalBuffer = 10.0f;
	mUIBundle.getHouseUI().setPosition(sf::Vector2f(mInteractiveObject.getX() + mInteractiveObject.getWidth() / 2.0f
		, mInteractiveObject.getY() + mInteractiveObject.getHeight() + verticalBuffer));

	mUIBundle.getHouseUI().updateIncDecCallbacks(std::bind(&HouseNode::incrementPurchaseBedNet, this)
		, std::bind(&HouseNode::decrementPurchaseBedNet, this)
		, std::bind(&HouseNode::incrementRepair, this)
		, std::bind(&HouseNode::decrementRepair, this));
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
