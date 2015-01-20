#include "doorUpdateNode.h"
#include "../GameObjects/interactiveObject.h"

#include "Trambo/Events/event.h"
#include "Trambo/Events/eventStr.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <stdexcept>


DoorUpdateNode::DoorUpdateNode(const InteractiveObject &interactiveObject, const sf::Texture &texture)
: mDrawSprite(0x5ea6cda9)
, mDoNotDrawSprite(0x918c6b78)
, mInteractiveObject(interactiveObject)
, mSprite(texture)
, mDrawn(false)
{
	if ("Tan" == mInteractiveObject.getColor())
	{
		// ALW - Coordinates for a tan colored closed door from Tiles.png 
		mSprite.setTextureRect(sf::IntRect(896, 640, 64, 64)); 
	}
	else if ("Slate" == mInteractiveObject.getColor())
	{
		// ALW - Coordinates for a slate colored closed door from Tiles.png 
		mSprite.setTextureRect(sf::IntRect(1216, 640, 64, 64));
	}
	else
	{
		throw std::runtime_error("ALW - Runtime Error: Interactive object's color property is not valid.");
	}

	mSprite.setPosition(sf::Vector2f(mInteractiveObject.getAlternateXCoord1(), mInteractiveObject.getAlternateYCoord1()));
}

void DoorUpdateNode::handleEvent(const trmb::Event &gameEvent)
{
	if (mDrawSprite == gameEvent.getType())
	{
		// ALW - Incidentally confirmed event is an EventStr. Downcasting is safe.
		const trmb::EventStr &eventStr = static_cast<const trmb::EventStr &>(gameEvent);

		if (mInteractiveObject.getName() == eventStr.getString())
		{
			mDrawn = true;
		}
	}
	else if (mDoNotDrawSprite == gameEvent.getType())
	{
		// ALW - Incidentally confirmed event is an EventStr. Downcasting is safe.
		const trmb::EventStr &eventStr = static_cast<const trmb::EventStr &>(gameEvent);

		if (mInteractiveObject.getName() == eventStr.getString())
		{
			mDrawn = false;
		}
	}
}

void DoorUpdateNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (mDrawn)
	{
		target.draw(mSprite, states);
	}
}
