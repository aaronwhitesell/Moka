#include "windowUINode.h"
#include "../HUD/UndoUI.h"

#include "Trambo/Events/event.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>


WindowUINode::WindowUINode(const UndoUI &windowUI)
: mDrawUI(0x30459275)
, mDoNotDrawUI(0xf83a20bd)
, mUI(windowUI)
, mIsDrawn(false)
{
}

void WindowUINode::handleEvent(const trmb::Event &gameEvent)
{
	if (mDrawUI == gameEvent.getType())
	{
		mIsDrawn = true;
	}
	else if (mDoNotDrawUI == gameEvent.getType())
	{
		mIsDrawn = false;
	}
}

void WindowUINode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (mIsDrawn)
	{
		target.draw(mUI, states);
	}
}
