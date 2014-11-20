#include "preventionNode.h"
#include "../GameObjects/interactiveObject.h"
#include "../HUD/chatBox.h"
#include "../HUD/undoUI.h"
#include "../Resources/resourceIdentifiers.h"

#include "Trambo/Events/event.h"
#include "Trambo/Localize/localize.h"
#include "Trambo/Sounds/soundPlayer.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Mouse.hpp>


PreventionNode::PreventionNode(const InteractiveObject &interactiveObject, const sf::RenderWindow &window, const sf::View &view,
	UndoUI &undoUI)
: InteractiveNode(interactiveObject)
, mLeftClickPress(0x6955d309)
, mUndoUI(undoUI)
, mWindow(window)
, mView(view)
{
}

void PreventionNode::handleEvent(const trmb::Event &gameEvent)
{
	InteractiveNode::handleEvent(gameEvent);

	if (!mDisableInput)
	{
		if (mLeftClickPress == gameEvent.getType())
		{
			mPreviousSelectedState = mSelected;
			if (mSelected && !isMouseOverUndoUI())
			{
				mSelected = false;
			}

			if (isMouseOverObject())
			{
				mSelected = true;
				if (!mPreviousSelectedState)
					activate();
			}
		}
	}
}

void PreventionNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (mSelected)
	{
		target.draw(mHightlight, states);
		target.draw(mUndoUI, states);
	}
}

void PreventionNode::updateCurrent(sf::Time)
{
	// ALW - Do not apply the PreventionNode's transform, because there are multiple instances that share a single UndoUI. If the 
	// ALW - transform is applied then the UndoUI would be at the location specified by the translation of multiple PreventionNodes
	// ALW - which is not correct. The workaround is to let the PreventionNode's position default to 0.0f, 0.0f and then use the
	// ALW - InteractiveObjects coordinates to position the UndoUI in the world. This way the PreventionNode's transform does not need
	// ALW - to be applied to the UndoUI. To keep the handler interface consistent the Identity transform is passed in and applied.
	sf::Transform transform = sf::Transform::Identity;

	mUndoUI.handler(mWindow, mView, transform);
}

bool PreventionNode::isMouseOverObject() const
{
	const sf::Vector2i relativeToWindow = sf::Mouse::getPosition(mWindow);
	const sf::Vector2f relativeToWorld = mWindow.mapPixelToCoords(relativeToWindow, mView);
	const sf::Vector2f mousePosition = relativeToWorld;

	const sf::FloatRect interactiveObjRect = sf::FloatRect(mInteractiveObject.getX(),
		mInteractiveObject.getY(), mInteractiveObject.getWidth(), mInteractiveObject.getHeight());

	bool ret = false;
	if (interactiveObjRect.contains(mousePosition))
	{
		// ALW - Mouse cursor is over the object.
		ret = true;
	}

	return ret;
}

bool PreventionNode::isMouseOverUndoUI() const
{
	bool ret = false;

	// ALW - If the building is selected then the UI element is displayed, so the UI element's position has been set.
	const sf::Vector2i relativeToWindow = sf::Mouse::getPosition(mWindow);
	const sf::Vector2f relativeToWorld = mWindow.mapPixelToCoords(relativeToWindow, mView);
	const sf::Vector2f mousePosition = relativeToWorld;
	const sf::FloatRect rect = mUndoUI.getRect();

	if (rect.contains(mousePosition))
	{
		ret = true;
	}

	return ret;
}
