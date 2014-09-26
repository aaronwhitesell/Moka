#include "HouseNode.h"
#include "../GameObjects/preventionObject.h"

#include "Trambo/Events/event.h"

#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


HouseNode::HouseNode(sf::RenderWindow &window, const sf::View &view
	, const PreventionObject &preventionObject, std::vector<sf::IntRect> attachedRects)
: mWindow(window)
, mView(view)
, mPreventionObject(preventionObject)
, mAttachedRects(attachedRects)
{
	mHightlight.setSize(static_cast<sf::Vector2f>(sf::Vector2i(mPreventionObject.getWidth(), mPreventionObject.getHeight())));
	mHightlight.setPosition(static_cast<sf::Vector2f>(sf::Vector2i(mPreventionObject.getX(), mPreventionObject.getY())));
}

void HouseNode::updateSelection()
{
	const sf::Vector2i relativeToWindow = sf::Mouse::getPosition(mWindow);
	const sf::Vector2f relativeToWorld = mWindow.mapPixelToCoords(relativeToWindow, mView);
	const sf::Vector2f MousePosition(relativeToWorld);
	const sf::FloatRect PrevObjRect(static_cast<sf::FloatRect>(sf::IntRect(mPreventionObject.getX(),
		mPreventionObject.getY(), mPreventionObject.getWidth(), mPreventionObject.getHeight())));

	// ALW - If the cursor click is on a house select it
	if (PrevObjRect.contains(MousePosition))
		mIsSelected = true;
	else
		mIsSelected = false;

	std::vector<sf::IntRect>::const_iterator iter    = begin(mAttachedRects);
	std::vector<sf::IntRect>::const_iterator iterEnd = end(mAttachedRects);

	// ALW - If the cursor click is on an object attached to the house then the house is unselected
	for (; iter != iterEnd; ++iter)
	{
		if (static_cast<sf::FloatRect>(*iter).contains(MousePosition))
			mIsSelected = false;
	}
}
