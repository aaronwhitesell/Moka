#include "preventionNode.h"
#include "../GameObjects/interactiveObject.h"
#include "../HUD/uiBundle.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>


PreventionNode::PreventionNode(const InteractiveObject &interactiveObject, const sf::RenderWindow &window, const sf::View &view
	, UIBundle &uiBundle)
: InteractiveNode(interactiveObject, window, view, uiBundle)
{
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
