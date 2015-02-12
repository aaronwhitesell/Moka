#include "countUI.h"
#include "../Resources/resourceIdentifiers.h"

#include "Trambo/Events/event.h"
#include "Trambo/Sounds/soundPlayer.h"
#include "Trambo/Utilities/utility.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <cassert>
#include <string>


CountUI::CountUI(Fonts::ID font, trmb::FontHolder &fonts, SoundEffects::ID soundEffect, trmb::SoundPlayer &soundPlayer)
: mBeginSimulationEvent(0x5000e550)
, mFonts(fonts)
, mSoundPlayer(soundPlayer)
, mBackground()
, mCountBackgroundLeft()
, mCountBackgroundRight()
, mCountTextLeft()
, mCountTextRight()
, mCountLeft(0)
, mCountRight(1)
, mHide(true)
{
	mBackground.setSize(sf::Vector2f(64.0f, 20.0f));
	mBackground.setFillColor(sf::Color(0u, 0u, 0u, 75u));
	mBackground.setOutlineColor(sf::Color(0u, 0u, 0u, 255u));
	mBackground.setOutlineThickness(1.0f);
	mBackground.setPosition(0.0f, 0.0f);

	mCountBackgroundLeft.setSize(sf::Vector2f(30.0f, 20.0f));
	mCountBackgroundLeft.setFillColor(sf::Color(0u, 0u, 0u, 200u));
	mCountBackgroundLeft.setOutlineColor(sf::Color(0u, 0u, 0u, 255u));
	mCountBackgroundLeft.setOutlineThickness(1.0f);
	mCountBackgroundLeft.setPosition(0.0f, 0.0f);

	mCountBackgroundRight.setSize(sf::Vector2f(30.0f, 20.0f));
	mCountBackgroundRight.setFillColor(sf::Color(0u, 0u, 0u, 200u));
	mCountBackgroundRight.setOutlineColor(sf::Color(0u, 0u, 0u, 255u));
	mCountBackgroundRight.setOutlineThickness(1.0f);
	mCountBackgroundRight.setPosition(34.0f, 0.0f);

	mCountTextLeft.setFont(mFonts.get(font));
	mCountTextLeft.setCharacterSize(14u);
	mCountTextLeft.setString("0");
	trmb::centerOrigin(mCountTextLeft, true, false);
	mCountTextLeft.setPosition(sf::Vector2f(mCountBackgroundLeft.getPosition().x + mCountBackgroundLeft.getSize().x / 2.0f, 1.0f));

	mCountTextRight.setFont(mFonts.get(font));
	mCountTextRight.setCharacterSize(14u);
	mCountTextRight.setString("0");
	mCountTextRight.setColor(sf::Color::Red);
	trmb::centerOrigin(mCountTextRight, true, false);
	mCountTextRight.setPosition(sf::Vector2f(mCountBackgroundRight.getPosition().x + mCountBackgroundRight.getSize().x / 2.0f, 1.0f));

	centerOrigin(*this, true, false);
}

sf::Vector2f CountUI::getSize() const
{
	return mBackground.getSize();
}

void CountUI::addLeft()
{
	++mCountLeft;

	mCountTextLeft.setString(std::to_string(mCountLeft));
	trmb::centerOrigin(mCountTextLeft, true, false);
}

void CountUI::subtractLeft()
{
	--mCountLeft;
	assert(("Count cannot be less than 0.", 0 <= mCountLeft));

	mCountTextLeft.setString(std::to_string(mCountLeft));
	trmb::centerOrigin(mCountTextLeft, true, false);
}

void CountUI::addRight()
{
	++mCountRight;

	mCountTextRight.setString(std::to_string(mCountRight));
	trmb::centerOrigin(mCountTextRight, true, false);
}

void CountUI::subtractRight()
{
	--mCountRight;
	assert(("Count cannot be less than 0.", 0 <= mCountRight));

	mCountTextRight.setString(std::to_string(mCountRight));
	trmb::centerOrigin(mCountTextRight, true, false);
}

void CountUI::handleEvent(const trmb::Event &gameEvent)
{
	if (mBeginSimulationEvent == gameEvent.getType())
	{
		unhide();
	}
}

void CountUI::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	if (!mHide)
	{
		target.draw(mBackground, states);
		target.draw(mCountBackgroundLeft, states);
		target.draw(mCountBackgroundRight, states);
		target.draw(mCountTextLeft, states);
		target.draw(mCountTextRight, states);
	}
}

void CountUI::unhide()
{
	mHide = false;
}

void CountUI::hide()
{
	mHide = true;
}

void centerOrigin(CountUI &ui, bool centerXAxis, bool centerYAxis)
{
	sf::Vector2f size = ui.getSize();
	float xAxis = 0.0f;
	float yAxis = 0.0f;

	if (centerXAxis)
		xAxis = std::floor(size.x / 2.0f);

	if (centerYAxis)
		yAxis = std::floor(size.y / 2.0f);

	ui.setOrigin(xAxis, yAxis);
}
