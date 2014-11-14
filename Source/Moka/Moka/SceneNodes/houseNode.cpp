#include "HouseNode.h"
#include "../GameObjects/interactiveObject.h"
#include "../HUD/chatBox.h"
#include "../HUD/optionsUI.h"
#include "../Resources/resourceIdentifiers.h"

#include "Trambo/Events/event.h"
#include "Trambo/Localize/localize.h"
#include "Trambo/Sounds/soundPlayer.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Mouse.hpp>


HouseNode::HouseNode(const sf::RenderWindow &window, const sf::View &view, const InteractiveObject &interactiveObject
	, std::vector<sf::FloatRect> attachedRects, trmb::SoundPlayer &soundPlayer, ChatBox &chatBox, OptionsUI &mHouseUI)
: InteractiveNode(window, view, interactiveObject)
, mAttachedRects(attachedRects)
, mSoundPlayer(soundPlayer)
, mChatBox(chatBox)
, mHouseUI(mHouseUI)
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
			if (mSelected && !isMouseOverHouseUI())
			{
				mSelected = false;
			}

			if (isMouseOverObject())
			{
				mSelected = true;
				if (!mPreviousSelectedState)
					activate();
			}

			if (mPreviousSelectedState && !mSelected)
			{
				// ALW - The house was unselected.
				mHouseUI.deactivate();
			}
		}
	}
}

bool HouseNode::isMouseOverHouseUI() const
{
	bool ret = false;

	// ALW - If the house is selected then the UI element is displayed, so the UI element's position has been set.
	const sf::Vector2i relativeToWindow = sf::Mouse::getPosition(mWindow);
	const sf::Vector2f relativeToWorld = mWindow.mapPixelToCoords(relativeToWindow, mView);
	const sf::Vector2f mousePosition = relativeToWorld;
	const sf::FloatRect rect = mHouseUI.getRect();

	if (rect.contains(mousePosition))
	{
		ret = true;
	}

	return ret;
}

bool HouseNode::isMouseOverObject() const
{
	const sf::Vector2i relativeToWindow = sf::Mouse::getPosition(mWindow);
	const sf::Vector2f relativeToWorld = mWindow.mapPixelToCoords(relativeToWindow, mView);
	const sf::Vector2f mousePosition = relativeToWorld;
	const sf::FloatRect interactiveObjRect = sf::FloatRect(mInteractiveObject.getX(),
		mInteractiveObject.getY(), mInteractiveObject.getWidth(), mInteractiveObject.getHeight());

	bool ret = false;
	if (interactiveObjRect.contains(mousePosition))
	{
		// ALW - Mouse cursor is over the house.
		ret = true;

		std::vector<sf::FloatRect>::const_iterator iter = begin(mAttachedRects);
		for (; iter != end(mAttachedRects); ++iter)
		{
			if (iter->contains(mousePosition))
			{
				// ALW - Mouse cursor is over an attached object.
				ret = false;
			}
		}
	}

	return ret;
}

void HouseNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (mSelected)
	{
		target.draw(mHightlight, states);
		target.draw(mHouseUI, states);
	}
}

void HouseNode::updateCurrent(sf::Time)
{
	// ALW - Do not apply the HouseNode's transform, because there are multiple instances that share a single HouseUI. If the 
	// ALW - transform is applied then the HouseUI would be at the location specified by the translation of multiple HouseNodes
	// ALW - which is not correct. The workaround is to let the HouseNode's position default to 0.0f, 0.0f and then use the
	// ALW - InteractiveObjects coordinates to position the HouseUI in the world. This way the HouseNode's transform does not need
	// ALW - to be applied to the HouseUI. To keep the handler interface consistent the Identity transform is passed in and applied.
	sf::Transform transform = sf::Transform::Identity;

	mHouseUI.handler(mWindow, mView, transform);
}

void HouseNode::activate()
{
	updateHouseUI();
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

void HouseNode::updateHouseUI()
{
	const float verticalBuffer = 10.0f;
	mHouseUI.setPosition(sf::Vector2f(mInteractiveObject.getX() + mInteractiveObject.getWidth() / 2.0f
		, mInteractiveObject.getY() + mInteractiveObject.getHeight() + verticalBuffer));

	mHouseUI.updateIncDecCallbacks(std::bind(&HouseNode::incrementPurchaseClick, this)
		, std::bind(&HouseNode::decrementPurchaseClick, this)
		, std::bind(&HouseNode::incrementRepairClick, this)
		, std::bind(&HouseNode::decrementRepairClick, this));
}

void HouseNode::incrementPurchaseClick()
{
	// ALW - TODO - Increment resource
}

void HouseNode::decrementPurchaseClick()
{
	// ALW - TODO - Decrement resource
}

void HouseNode::incrementRepairClick()
{
	// ALW - TODO - Increment resource
}

void HouseNode::decrementRepairClick()
{
	// ALW - TODO - Decrement resource
}
