#include "buildingNode.h"
#include "../GameObjects/interactiveObject.h"
#include "../HUD/optionsUI.h"

#include "Trambo/Events/event.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Mouse.hpp>


BuildingNode::BuildingNode(const InteractiveObject &interactiveObject, const sf::RenderWindow &window, const sf::View &view
	, OptionsUI &optionsUI, std::vector<sf::FloatRect> attachedRects)
: InteractiveNode(interactiveObject)
, mOptionsUI(optionsUI)
, mLeftClickPress(0x6955d309)
, mWindow(window)
, mView(view)
, mAttachedRects(attachedRects)
{
}

void BuildingNode::handleEvent(const trmb::Event &gameEvent)
{
	InteractiveNode::handleEvent(gameEvent);

	if (!mDisableInput)
	{
		if (mLeftClickPress == gameEvent.getType())
		{
			mPreviousSelectedState = mSelected;
			if (mSelected && !isMouseOverOptionsUI())
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
				mOptionsUI.deactivate();
			}
		}
	}
}

void BuildingNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (mSelected)
	{
		target.draw(mHightlight, states);
		target.draw(mOptionsUI, states);
	}
}

void BuildingNode::updateCurrent(sf::Time)
{
	// ALW - Do not apply the BuildingNode's transform, because there are multiple instances that share a single HouseUI. If the 
	// ALW - transform is applied then the HouseUI would be at the location specified by the translation of multiple BuildingNodes
	// ALW - which is not correct. The workaround is to let the BuildingNode's position default to 0.0f, 0.0f and then use the
	// ALW - InteractiveObjects coordinates to position the HouseUI in the world. This way the BuildingNode's transform does not need
	// ALW - to be applied to the HouseUI. To keep the handler interface consistent the Identity transform is passed in and applied.
	sf::Transform transform = sf::Transform::Identity;

	mOptionsUI.handler(mWindow, mView, transform);
}

bool BuildingNode::isMouseOverObject() const
{
	const sf::Vector2i relativeToWindow = sf::Mouse::getPosition(mWindow);
	const sf::Vector2f relativeToWorld = mWindow.mapPixelToCoords(relativeToWindow, mView);
	const sf::Vector2f mousePosition = relativeToWorld;
	const sf::FloatRect interactiveObjRect = sf::FloatRect(mInteractiveObject.getX(),
		mInteractiveObject.getY(), mInteractiveObject.getWidth(), mInteractiveObject.getHeight());

	bool ret = false;
	if (interactiveObjRect.contains(mousePosition))
	{
		// ALW - Mouse cursor is over the building.
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

bool BuildingNode::isMouseOverOptionsUI() const
{
	bool ret = false;

	// ALW - If the building is selected then the UI element is displayed, so the UI element's position has been set.
	const sf::Vector2i relativeToWindow = sf::Mouse::getPosition(mWindow);
	const sf::Vector2f relativeToWorld = mWindow.mapPixelToCoords(relativeToWindow, mView);
	const sf::Vector2f mousePosition = relativeToWorld;
	const sf::FloatRect rect = mOptionsUI.getRect();

	if (rect.contains(mousePosition))
	{
		ret = true;
	}

	return ret;
}
