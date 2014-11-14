#include "clinicNode.h"
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


ClinicNode::ClinicNode(const sf::RenderWindow &window, const sf::View &view, const InteractiveObject &interactiveObject
	, std::vector<sf::FloatRect> attachedRects, trmb::SoundPlayer &soundPlayer, ChatBox &chatBox, OptionsUI &mClinicUI)
: InteractiveNode(window, view, interactiveObject)
, mAttachedRects(attachedRects)
, mSoundPlayer(soundPlayer)
, mChatBox(chatBox)
, mClinicUI(mClinicUI)
{
	int x = 7;
}

void ClinicNode::handleEvent(const trmb::Event &gameEvent)
{
	InteractiveNode::handleEvent(gameEvent);

	if (!mDisableInput)
	{
		if (mLeftClickPress == gameEvent.getType())
		{
			mPreviousSelectedState = mSelected;
			if (mSelected && !isMouseOverClinicUI())
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
				// ALW - The Clinic was unselected.
				mClinicUI.deactivate();
			}
		}
	}
}

bool ClinicNode::isMouseOverClinicUI() const
{
	bool ret = false;

	// ALW - If the Clinic is selected then the UI element is displayed, so the UI element's position has been set.
	const sf::Vector2i relativeToWindow = sf::Mouse::getPosition(mWindow);
	const sf::Vector2f relativeToWorld = mWindow.mapPixelToCoords(relativeToWindow, mView);
	const sf::Vector2f mousePosition = relativeToWorld;
	const sf::FloatRect rect = mClinicUI.getRect();

	if (rect.contains(mousePosition))
	{
		ret = true;
	}

	return ret;
}

bool ClinicNode::isMouseOverObject() const
{
	const sf::Vector2i relativeToWindow = sf::Mouse::getPosition(mWindow);
	const sf::Vector2f relativeToWorld = mWindow.mapPixelToCoords(relativeToWindow, mView);
	const sf::Vector2f mousePosition = relativeToWorld;
	const sf::FloatRect interactiveObjRect = sf::FloatRect(mInteractiveObject.getX(),
		mInteractiveObject.getY(), mInteractiveObject.getWidth(), mInteractiveObject.getHeight());

	bool ret = false;
	if (interactiveObjRect.contains(mousePosition))
	{
		// ALW - Mouse cursor is over the Clinic.
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

void ClinicNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (mSelected)
	{
		target.draw(mHightlight, states);
		target.draw(mClinicUI, states);
	}
}

void ClinicNode::updateCurrent(sf::Time)
{
	// ALW - Do not apply the ClinicNode's transform, because there are multiple instances that share a single ClinicUI. If the 
	// ALW - transform is applied then the ClinicUI would be at the location specified by the translation of multiple ClinicNodes
	// ALW - which is not correct. The workaround is to let the ClinicNode's position default to 0.0f, 0.0f and then use the
	// ALW - InteractiveObjects coordinates to position the ClinicUI in the world. This way the ClinicNode's transform does not need
	// ALW - to be applied to the ClinicUI. To keep the handler interface consistent the Identity transform is passed in and applied.
	sf::Transform transform = sf::Transform::Identity;

	mClinicUI.handler(mWindow, mView, transform);
}

void ClinicNode::activate()
{
	updateClinicUI();
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
	mChatBox.updateText(trmb::Localize::getInstance().getString("inspectClinic"));
	mSelected = true;
}

void ClinicNode::updateClinicUI()
{
	const float verticalBuffer = 10.0f;
	mClinicUI.setPosition(sf::Vector2f(mInteractiveObject.getX() + mInteractiveObject.getWidth() / 2.0f
		, mInteractiveObject.getY() + mInteractiveObject.getHeight() + verticalBuffer));

	mClinicUI.updateIncDecCallbacks(std::bind(&ClinicNode::incrementPurchaseClick, this)
		, std::bind(&ClinicNode::decrementPurchaseClick, this)
		, std::bind(&ClinicNode::incrementRepairClick, this)
		, std::bind(&ClinicNode::decrementRepairClick, this));
}

void ClinicNode::incrementPurchaseClick()
{
	// ALW - TODO - Increment resource
}

void ClinicNode::decrementPurchaseClick()
{
	// ALW - TODO - Decrement resource
}

void ClinicNode::incrementRepairClick()
{
	// ALW - TODO - Increment resource
}

void ClinicNode::decrementRepairClick()
{
	// ALW - TODO - Decrement resource
}
