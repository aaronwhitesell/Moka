#include "clinicSpriteNode.h"
#include "../GameObjects/interactiveObject.h"

#include "Trambo/Events/event.h"
#include "Trambo/Events/eventStr.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>


ClinicSpriteNode::ClinicSpriteNode(const InteractiveObject &interactiveObject, const sf::Texture &texture)
: mDrawSmallRDTCrateSprite(0x3377fe93)
, mDrawLargeRDTCrateSprite(0x358963ee)
, mDrawRDTBarrelSprite(0xafbc42ce)
, mDoNotRDTDrawSprite(0xf08738aa)
, mDrawSmallACTCrateSprite(0x7f31c614)
, mDrawLargeACTCrateSprite(0x4a640fb6)
, mDrawACTBarrelSprite(0xad344f84)
, mDoNotACTDrawSprite(0xd6fe68f2)
, mInteractiveObject(interactiveObject)
, mRDTSprite(texture)
, mACTSprite(texture)
, mDrawRDT(false)
, mDrawACT(false)
{
	mRDTSprite.setPosition(sf::Vector2f(mInteractiveObject.getPosX0(), mInteractiveObject.getPosY0()));
	mACTSprite.setPosition(sf::Vector2f(mInteractiveObject.getPosX1(), mInteractiveObject.getPosY1()));
}

void ClinicSpriteNode::handleEvent(const trmb::Event &gameEvent)
{
	if (mDrawSmallRDTCrateSprite == gameEvent.getType())
	{
		// ALW - Incidentally confirmed event is an EventStr. Downcasting is safe.
		const trmb::EventStr &eventStr = static_cast<const trmb::EventStr &>(gameEvent);

		if (mInteractiveObject.getName() == eventStr.getString())
		{
			mRDTSprite.setTextureRect(sf::IntRect(576, 576, 64, 64)); // ALW - Coordinates for a small crate from Tiles.png
			mDrawRDT = true;
		}
	}
	else if (mDrawLargeRDTCrateSprite == gameEvent.getType())
	{
		// ALW - Incidentally confirmed event is an EventStr. Downcasting is safe.
		const trmb::EventStr &eventStr = static_cast<const trmb::EventStr &>(gameEvent);

		if (mInteractiveObject.getName() == eventStr.getString())
		{
			mRDTSprite.setTextureRect(sf::IntRect(512, 576, 64, 64)); // ALW - Coordinates for a large crate from Tiles.png
			mDrawRDT = true;
		}
	}
	else if (mDrawRDTBarrelSprite == gameEvent.getType())
	{
		// ALW - Incidentally confirmed event is an EventStr. Downcasting is safe.
		const trmb::EventStr &eventStr = static_cast<const trmb::EventStr &>(gameEvent);

		if (mInteractiveObject.getName() == eventStr.getString())
		{
			mRDTSprite.setTextureRect(sf::IntRect(512, 704, 64, 64)); // ALW - Coordinates for a barrel from Tiles.png
			mDrawRDT = true;
		}
	}
	else if (mDoNotRDTDrawSprite == gameEvent.getType())
	{
		// ALW - Incidentally confirmed event is an EventStr. Downcasting is safe.
		const trmb::EventStr &eventStr = static_cast<const trmb::EventStr &>(gameEvent);

		if (mInteractiveObject.getName() == eventStr.getString())
		{
			mDrawRDT = false;
		}
	}
	else if (mDrawSmallACTCrateSprite == gameEvent.getType())
	{
		// ALW - Incidentally confirmed event is an EventStr. Downcasting is safe.
		const trmb::EventStr &eventStr = static_cast<const trmb::EventStr &>(gameEvent);

		if (mInteractiveObject.getName() == eventStr.getString())
		{
			mACTSprite.setTextureRect(sf::IntRect(576, 576, 64, 64)); // ALW - Coordinates for a small crate from Tiles.png
			mDrawACT = true;
		}
	}
	else if (mDrawLargeACTCrateSprite == gameEvent.getType())
	{
		// ALW - Incidentally confirmed event is an EventStr. Downcasting is safe.
		const trmb::EventStr &eventStr = static_cast<const trmb::EventStr &>(gameEvent);

		if (mInteractiveObject.getName() == eventStr.getString())
		{
			mACTSprite.setTextureRect(sf::IntRect(512, 576, 64, 64)); // ALW - Coordinates for a large crate from Tiles.png
			mDrawACT = true;
		}
	}
	else if (mDrawACTBarrelSprite == gameEvent.getType())
	{
		// ALW - Incidentally confirmed event is an EventStr. Downcasting is safe.
		const trmb::EventStr &eventStr = static_cast<const trmb::EventStr &>(gameEvent);

		if (mInteractiveObject.getName() == eventStr.getString())
		{
			mACTSprite.setTextureRect(sf::IntRect(512, 704, 64, 64)); // ALW - Coordinates for a barrel from Tiles.png
			mDrawACT = true;
		}
	}
	else if (mDoNotACTDrawSprite == gameEvent.getType())
	{
		// ALW - Incidentally confirmed event is an EventStr. Downcasting is safe.
		const trmb::EventStr &eventStr = static_cast<const trmb::EventStr &>(gameEvent);

		if (mInteractiveObject.getName() == eventStr.getString())
		{
			mDrawACT = false;
		}
	}
}

void ClinicSpriteNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (mDrawRDT)
	{
		target.draw(mRDTSprite, states);
	}

	if (mDrawACT)
	{
		target.draw(mACTSprite, states);
	}
}
