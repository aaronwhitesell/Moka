#include "barrelUINode.h"
#include "../HUD/UndoUI.h"

#include "Trambo/Events/event.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>


BarrelUINode::BarrelUINode(const UndoUI &barrelUI)
: mDrawUI(0xcfdb933d)
, mDoNotDrawUI(0x210832f5)
, mUI(barrelUI)
, mIsDrawn(false)
{
}

void BarrelUINode::handleEvent(const trmb::Event &gameEvent)
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

void BarrelUINode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (mIsDrawn)
	{
		target.draw(mUI, states);
	}
}
