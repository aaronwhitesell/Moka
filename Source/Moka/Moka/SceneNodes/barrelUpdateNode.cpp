#include "barrelUpdateNode.h"
#include "../GameObjects/interactiveObject.h"

#include "Trambo/Events/event.h"
#include "Trambo/Events/eventStr.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>


BarrelUpdateNode::BarrelUpdateNode(const InteractiveObject &interactiveObject, const sf::Texture &texture)
: mDrawSprite(0xe22f85d5)
, mDoNotDrawSprite(0xcd1fd24)
, mInteractiveObject(interactiveObject)
, mSprite(texture)
, mDrawn(false)
{
	mSprite.setTextureRect(sf::IntRect(576, 640, 64, 64)); // ALW - Coordinates for a barrel with lid from Tiles.png
	mSprite.setPosition(sf::Vector2f(mInteractiveObject.getAlternateXCoord1(), mInteractiveObject.getAlternateYCoord1()));
}

void BarrelUpdateNode::handleEvent(const trmb::Event &gameEvent)
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

void BarrelUpdateNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (mDrawn)
	{
		target.draw(mSprite, states);
	}
}
