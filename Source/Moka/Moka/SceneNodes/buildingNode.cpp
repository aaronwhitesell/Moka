#include "buildingNode.h"
#include "../GameObjects/interactiveObject.h"
#include "../Levels/uiBundle.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>


BuildingNode::BuildingNode(const InteractiveObject &interactiveObject, const sf::RenderWindow &window, const sf::View &view
	, UIBundle &uiBundle, std::vector<sf::FloatRect> attachedRects)
: InteractiveNode(interactiveObject, window, view, uiBundle)
, mAttachedRects(attachedRects)
{
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
