#include "preventionNode.h"
#include "../GameObjects/interactiveObject.h"

#include "Trambo/Events/event.h"

#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


PreventionNode::PreventionNode(sf::RenderWindow &window, const sf::View &view, const InteractiveObject &interactiveObject)
: mWindow(window)
, mView(view)
, mInteractiveObject(interactiveObject)
{
	mHightlight.setSize(static_cast<sf::Vector2f>(sf::Vector2i(mInteractiveObject.getWidth(), mInteractiveObject.getHeight())));
	mHightlight.setPosition(static_cast<sf::Vector2f>(sf::Vector2i(mInteractiveObject.getX(), mInteractiveObject.getY())));
}

void PreventionNode::updateSelection()
{
	const sf::Vector2i relativeToWindow = sf::Mouse::getPosition(mWindow);
	const sf::Vector2f relativeToWorld = mWindow.mapPixelToCoords(relativeToWindow, mView);
	const sf::Vector2f MousePosition(relativeToWorld.x, relativeToWorld.y);
	const sf::FloatRect interactiveObjRect(static_cast<sf::FloatRect>(sf::IntRect(mInteractiveObject.getX(),
		mInteractiveObject.getY(), mInteractiveObject.getWidth(), mInteractiveObject.getHeight())));

	if (interactiveObjRect.contains(MousePosition))
		mIsSelected = true;
	else
		mIsSelected = false;
}



