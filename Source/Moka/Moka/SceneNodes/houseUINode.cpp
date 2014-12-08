#include "houseUINode.h"
#include "../HUD/OptionsUI.h"

#include "Trambo/Events/event.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>


HouseUINode::HouseUINode(const OptionsUI &houseUI)
: mDrawUI(0xc7353048)
, mDoNotDrawUI(0x8e6093bf)
, mUI(houseUI)
, mIsDrawn(false)
{
}

void HouseUINode::handleEvent(const trmb::Event &gameEvent)
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

void HouseUINode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (mIsDrawn)
	{
		target.draw(mUI, states);
	}
}
