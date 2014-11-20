#include "InteractiveNode.h"
#include "../GameObjects/interactiveObject.h"

#include "Trambo/Events/event.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>


InteractiveNode::InteractiveNode(const InteractiveObject &interactiveObject)
: mInteractiveObject(interactiveObject)
, mSelected(false)
, mPreviousSelectedState(false)
, mDisableInput(false)
, mCreateTextPrompt(0x25e87fd8)
, mClearTextPrompt(0xc1523265)
{
	mHightlight.setSize(sf::Vector2f(mInteractiveObject.getWidth(), mInteractiveObject.getHeight()));
	mHightlight.setPosition(sf::Vector2f(mInteractiveObject.getX(), mInteractiveObject.getY()));
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

void InteractiveNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
	// Do nothing by default
}
