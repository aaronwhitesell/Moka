#include "windowUpdateNode.h"
#include "../GameObjects/interactiveObject.h"

#include "Trambo/Events/event.h"
#include "Trambo/Events/eventStr.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>


WindowUpdateNode::WindowUpdateNode(const InteractiveObject &interactiveObject, const sf::Texture &texture)
: mDrawWindowScreenSprite(0x41a9ff51)
, mDoNotDrawWindowScreenSprite(0xbfe9d3e6)
, mDrawWindowClosedSprite(0xe0f87a29)
, mDoNotDrawWindowClosedSprite(0xdeecc064)
, mInteractiveObject(interactiveObject)
, mSprite(texture)
, mWindowScreen(false)
, mWindowClosed(false)
, mDrawn(false)
{
	mSprite.setPosition(sf::Vector2f(mInteractiveObject.getAlternateXCoord1(), mInteractiveObject.getAlternateYCoord1()));
}

void WindowUpdateNode::handleEvent(const trmb::Event &gameEvent)
{
	if (mDrawWindowScreenSprite == gameEvent.getType())
	{
		// ALW - Incidentally confirmed event is an EventStr. Downcasting is safe.
		const trmb::EventStr &eventStr = static_cast<const trmb::EventStr &>(gameEvent);

		if (mInteractiveObject.getName() == eventStr.getString())
		{
			mWindowScreen = true;
		}
	}
	else if (mDoNotDrawWindowScreenSprite == gameEvent.getType())
	{
		// ALW - Incidentally confirmed event is an EventStr. Downcasting is safe.
		const trmb::EventStr &eventStr = static_cast<const trmb::EventStr &>(gameEvent);

		if (mInteractiveObject.getName() == eventStr.getString())
		{
			mWindowScreen = false;
		}
	}
	else if (mDrawWindowClosedSprite == gameEvent.getType())
	{
		// ALW - Incidentally confirmed event is an EventStr. Downcasting is safe.
		const trmb::EventStr &eventStr = static_cast<const trmb::EventStr &>(gameEvent);

		if (mInteractiveObject.getName() == eventStr.getString())
		{
			mWindowClosed = true;
		}
	}
	else if (mDoNotDrawWindowClosedSprite == gameEvent.getType())
	{
		// ALW - Incidentally confirmed event is an EventStr. Downcasting is safe.
		const trmb::EventStr &eventStr = static_cast<const trmb::EventStr &>(gameEvent);

		if (mInteractiveObject.getName() == eventStr.getString())
		{
			mWindowClosed = false;
		}
	}

	setSprite();
}

void WindowUpdateNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (mDrawn)
	{
		target.draw(mSprite, states);
	}
}

void WindowUpdateNode::setSprite()
{
	if (mWindowClosed)
	{
		// ALW - If the window is closed then the screen is not visible.
		mDrawn = true;
		setWindowClosedSprite();
	}
	else if (mWindowScreen)
	{
		// ALW - If the window is not closed then the screen is visible.
		mDrawn = true;
		setWindowScreenSprite();
	}
	else
	{
		mDrawn = false;
	}
}

void WindowUpdateNode::setWindowScreenSprite()
{
	if ("Tan" == mInteractiveObject.getColor())
	{
		if ("Simple" == mInteractiveObject.getStyle())
		{
			mSprite.setTextureRect(sf::IntRect(832, 640, 64, 64));
		}
		else if ("Wooden" == mInteractiveObject.getStyle())
		{
			mSprite.setTextureRect(sf::IntRect(768, 640, 64, 64));
		}
		else
		{
			throw std::runtime_error("ALW - Runtime Error: Interactive object's style property is not valid.");
		}
	}
	else if ("Slate" == mInteractiveObject.getColor())
	{
		if ("Simple" == mInteractiveObject.getStyle())
		{
			mSprite.setTextureRect(sf::IntRect(1152, 640, 64, 64));
		}
		else if ("Wooden" == mInteractiveObject.getStyle())
		{
			mSprite.setTextureRect(sf::IntRect(1088, 640, 64, 64));
		}
		else
		{
			throw std::runtime_error("ALW - Runtime Error: Interactive object's style property is not valid.");
		}
	}
	else
	{
		throw std::runtime_error("ALW - Runtime Error: Interactive object's color property is not valid.");
	}
}

void WindowUpdateNode::setWindowClosedSprite()
{
	if ("Tan" == mInteractiveObject.getColor())
	{
		if ("Simple" == mInteractiveObject.getStyle())
		{
			mSprite.setTextureRect(sf::IntRect(832, 704, 64, 64));
		}
		else if ("Wooden" == mInteractiveObject.getStyle())
		{
			mSprite.setTextureRect(sf::IntRect(768, 704, 64, 64));
		}
		else
		{
			throw std::runtime_error("ALW - Runtime Error: Interactive object's style property is not valid.");
		}
	}
	else if ("Slate" == mInteractiveObject.getColor())
	{
		if ("Simple" == mInteractiveObject.getStyle())
		{
			mSprite.setTextureRect(sf::IntRect(1152, 704, 64, 64));
		}
		else if ("Wooden" == mInteractiveObject.getStyle())
		{
			mSprite.setTextureRect(sf::IntRect(1088, 704, 64, 64));
		}
		else
		{
			throw std::runtime_error("ALW - Runtime Error: Interactive object's style property is not valid.");
		}
	}
	else
	{
		throw std::runtime_error("ALW - Runtime Error: Interactive object's color property is not valid.");
	}
}
