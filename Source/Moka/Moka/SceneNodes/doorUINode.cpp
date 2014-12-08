#include "doorUINode.h"
#include "../HUD/UndoUI.h"

#include "Trambo/Events/event.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>


DoorUINode::DoorUINode(const UndoUI &doorUI)
: mDrawUI(0x7cf851c6)
, mDoNotDrawUI(0xc0a53a4d)
, mUI(doorUI)
, mIsDrawn(false)
{
}

void DoorUINode::handleEvent(const trmb::Event &gameEvent)
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

void DoorUINode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (mIsDrawn)
	{
		target.draw(mUI, states);
	}
}
