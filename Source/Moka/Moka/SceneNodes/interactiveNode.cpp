#include "InteractiveNode.h"
#include "../GameObjects/interactiveObject.h"
#include "../HUD/optionsUI.h"
#include "../HUD/UndoUI.h"
#include "../Levels/uiBundle.h"

#include "Trambo/Events/event.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>


InteractiveNode::InteractiveNode(const InteractiveObject &interactiveObject, const sf::RenderWindow &window, const sf::View &view, 
	UIBundle &uiBundle)
: mInteractiveObject(interactiveObject)
, mWindow(window)
, mView(view)
, mUIBundle(uiBundle)
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

bool InteractiveNode::isMouseOverUI(sf::FloatRect rect) const
{
	bool ret = false;

	const sf::Vector2i relativeToWindow = sf::Mouse::getPosition(mWindow);
	const sf::Vector2f relativeToWorld = mWindow.mapPixelToCoords(relativeToWindow, mView);
	const sf::Vector2f mousePosition = relativeToWorld;

	if (rect.contains(mousePosition))
	{
		ret = true;
	}

	return ret;
}

void InteractiveNode::sendEvent(const trmb::Event &gameEvent)
{
	EventHandler::sendEvent(gameEvent);
}
