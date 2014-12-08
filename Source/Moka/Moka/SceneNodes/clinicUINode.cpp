#include "clinicUINode.h"
#include "../HUD/OptionsUI.h"

#include "Trambo/Events/event.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>


ClinicUINode::ClinicUINode(const OptionsUI &clinicUI)
: mDrawUI(0x1363b002)
, mDoNotDrawUI(0x7ccd235d)
, mUI(clinicUI)
, mIsDrawn(false)
{
}

void ClinicUINode::handleEvent(const trmb::Event &gameEvent)
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

void ClinicUINode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (mIsDrawn)
	{
		target.draw(mUI, states);
	}
}
