#include "hero.h"

#include "Trambo/Events/event.h"

//#include <SFML/Graphics/Color.hpp> // ALW - mDebugShape
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/View.hpp>

#include <algorithm>
#include <cmath>


Hero::Hero(sf::FloatRect worldBounds, const sf::View &view)
: Entity()
, mView(view)
, mWorldBounds(worldBounds)
{
	// ALW - Uncomment to draw a centered rectangle
//	sf::FloatRect bounds = mDebugShape.getLocalBounds();
//	mDebugShape.setOrigin(std::floor(bounds.width / 2.f), std::floor(bounds.height / 2.f));
//	mDebugShape.setFillColor(sf::Color(0u, 0u, 0u, 150u));
//	mDebugShape.setSize(sf::Vector2f(16.0f, 16.0f));
}

void Hero::handleEvent(const trmb::Event &gameEvent)
{
	// ALW - These GUIDs match the GUIDs in the Controller class.
	const EventGuid up    = 0x84b05719;
	const EventGuid down  = 0xa95ea771;
	const EventGuid left  = 0x84e6c13c;
	const EventGuid right = 0x19e343e8;
	const EventGuid jump  = 0x6955d309;
	const EventGuid shoot = 0x3e6524cd;

	switch (gameEvent.getType())
	{
	case up:
		accelerate(0.0f, -1.0f);
		break;
	case down:
		accelerate(0.0f, 1.0f);
		break;
	case left:
		accelerate(-1.0f, 0.0f);
		break;
	case right:
		accelerate(1.0f, 0.0f);
		break;
	}
}

void Hero::updateCurrent(sf::Time dt)
{
	correctDiagonalVelocity();
	accelerate(trmb::Entity::getVelocity() * getMaxSpeed());
	trmb::Entity::updateCurrent(dt);
	correctPosition();
}

float Hero::getMaxSpeed() const
{
	return 350.0f;
}

//void Hero::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
//{
//	target.draw(mDebugShape, states);
//}

void Hero::correctPosition()
{
	// ALW - If the camera moves outside the boundaries of the world then
	// ALW - move it back to the world boundary.
	sf::Vector2f position = getPosition();

	// ALW - Accounts for origin being set to the center of the view
	sf::Vector2f viewHalfDimensions = mView.getSize() / 2.0f;

	position.x = std::max(position.x, mWorldBounds.left + viewHalfDimensions.x);
	position.x = std::min(position.x, mWorldBounds.left + mWorldBounds.width - viewHalfDimensions.x);
	position.y = std::max(position.y, mWorldBounds.top + viewHalfDimensions.y);
	position.y = std::min(position.y, mWorldBounds.top + mWorldBounds.height - viewHalfDimensions.y);

	// ALW - Round down to the the nearest whole number to assure there is a 1:1
	// ALW - mapping of pixels in the window to texels (pixels in the texture).
	// ALW - This is done to avoid graphical tearing.
	position.x = std::floor(position.x);
	position.y = std::floor(position.y);

	setPosition(position);
}

void Hero::correctDiagonalVelocity()
{
	sf::Vector2f velocity = getVelocity();

	// ALW - If moving diagonally, correct velocity
	if (velocity.x != 0.0f && velocity.y != 0.0f)
		setVelocity(velocity / std::sqrt(2.0f));
}
