#include "InteractiveNode.h"

#include "Trambo/Events/event.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>


InteractiveNode::InteractiveNode()
: mRightClick(0x6955d309)
, mLeftClick(0x3e6524cd)
, mIsSelected(false)
{
	mHightlight.setFillColor(sf::Color(0u, 0u, 0u, 50u));
	mHightlight.setOutlineColor(sf::Color(0u, 0u, 0u, 125u));
	mHightlight.setOutlineThickness(2.0f);
}

void InteractiveNode::handleEvent(const trmb::Event& gameEvent)
{
	if (isLeftClick(gameEvent) || isRightClick(gameEvent))
		updateSelection();
}

void InteractiveNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (mIsSelected)
		target.draw(mHightlight, states);
}

bool InteractiveNode::isLeftClick(const trmb::Event& gameEvent) const
{
	bool ret = false;

	if (mLeftClick == gameEvent.getType())
		ret = true;

	return ret;
}

bool InteractiveNode::isRightClick(const trmb::Event& gameEvent) const
{
	bool ret = false;

	if (mRightClick == gameEvent.getType())
		ret = true;

	return ret;
}
