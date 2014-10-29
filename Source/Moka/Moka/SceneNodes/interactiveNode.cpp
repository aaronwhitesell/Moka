#include "InteractiveNode.h"
#include "../GameObjects/interactiveObject.h"

#include "Trambo/Events/event.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Mouse.hpp>


InteractiveNode::InteractiveNode(sf::RenderWindow &window, const sf::View &view, const InteractiveObject &interactiveObject)
: mWindow(window)
, mView(view)
, mInteractiveObject(interactiveObject)
, mLeftClickPress(0x6955d309)
, mCreateTextPrompt(0x25e87fd8)
, mClearTextPrompt(0xc1523265)
, mSelected(false)
, mPreviousSelectedState(false)
, mDisableInput(false)
{
	mHightlight.setFillColor(sf::Color(0u, 0u, 0u, 50u));
	mHightlight.setOutlineColor(sf::Color(0u, 0u, 0u, 125u));
	mHightlight.setOutlineThickness(2.0f);
}

void InteractiveNode::handleEvent(const trmb::Event& gameEvent)
{
	if (mCreateTextPrompt == gameEvent.getType())
	{
		mDisableInput = true;
		mSelected = false;
	}
	else if (mClearTextPrompt == gameEvent.getType())
		mDisableInput = false;
}

bool InteractiveNode::isMouseOverObject() const
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

void InteractiveNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (mSelected)
		target.draw(mHightlight, states);
}
