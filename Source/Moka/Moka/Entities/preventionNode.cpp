#include "preventionNode.h"
#include "../GameObjects/preventionObject.h"

#include "Trambo/Events/event.h"

#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


PreventionNode::PreventionNode(sf::RenderWindow &window, const sf::View &view, const PreventionObject &preventionObject)
: mRightClick(0x6955d309)
, mLeftClick(0x3e6524cd)
, mWindow(window)
, mView(view)
, mPreventionObject(preventionObject)
, mIsSelected(false)
{
	mHightlight.setSize(static_cast<sf::Vector2f>(sf::Vector2i(mPreventionObject.mWidth, mPreventionObject.mHeight)));
	mHightlight.setPosition(static_cast<sf::Vector2f>(sf::Vector2i(mPreventionObject.mX, mPreventionObject.mY)));
	mHightlight.setFillColor(sf::Color(0u, 0u, 0u, 50u));
	mHightlight.setOutlineColor(sf::Color(0u, 0u, 0u, 125u));
	mHightlight.setOutlineThickness(2.0f);
}

void PreventionNode::handleEvent(const trmb::Event& gameEvent)
{
	if (isLeftClick(gameEvent) || isRightClick(gameEvent))
		updateSelection();
}

void PreventionNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (mIsSelected)
		target.draw(mHightlight, states);
}

bool PreventionNode::isLeftClick(const trmb::Event& gameEvent) const
{
	bool ret = false;

	if (mLeftClick == gameEvent.getType())
		ret = true;

	return ret;
}

bool PreventionNode::isRightClick(const trmb::Event& gameEvent) const
{
	bool ret = false;

	if (mRightClick == gameEvent.getType())
		ret = true;

	return ret;
}

void PreventionNode::updateSelection()
{
	const sf::Vector2i relativeToWindow = sf::Mouse::getPosition(mWindow);
	const sf::Vector2f relativeToWorld = mWindow.mapPixelToCoords(relativeToWindow, mView);
	const sf::Vector2f MousePosition(relativeToWorld.x, relativeToWorld.y);
	const sf::FloatRect PrevObjRect(static_cast<sf::FloatRect>(sf::IntRect(mPreventionObject.mX,
		mPreventionObject.mY, mPreventionObject.mWidth, mPreventionObject.mHeight)));

	if (PrevObjRect.contains(MousePosition))
		mIsSelected = true;
	else
		mIsSelected = false;
}



