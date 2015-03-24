#include "houseTrackerUI.h"
#include "../Resources/resourceIdentifiers.h"

#include "Trambo/Events/event.h"
#include "Trambo/Localize/localize.h"
#include "Trambo/Sounds/soundPlayer.h"
#include "Trambo/Utilities/utility.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <cassert>
#include <cmath>
#include <string>


HouseTrackerUI::HouseTrackerUI(Fonts::ID font, trmb::FontHolder &fonts, SoundEffects::ID soundEffect, trmb::SoundPlayer &soundPlayer)
: mBeginSimulationEvent(0x5000e550)
, mBeginScoreboardEvent(0xf5e88b6e)
, mSoundPlayer(soundPlayer)
, mBackground()
, mMosquitoTextBackground()
, mMosquitoText(trmb::Localize::getInstance().getString("mosquitoesUI"), fonts.get(font), 13u)
, mInfectedMosquitoCountBackground()
, mMosquitoTotalBackground()
, mInfectedMosquitoCountText("0", fonts.get(font), 13u)
, mMosquitoTotalText("0", fonts.get(font), 13u)
, mInfectedMosquitoCount(0)
, mMosquitoTotal(0)
, mHide(true)
{
	const float buffer = 3.0f;	// ALW - 1 outline, 1 space, 1 outline
	const sf::Vector2f element = sf::Vector2f(36.0f, 14.0f);
	const sf::Vector2f ui = sf::Vector2f(element.x * 2.0f + buffer, element.y * 2.0f + buffer);
	const sf::Color backgroundColor0 = sf::Color(0u, 0u, 0u, 75u);
	const sf::Color backgroundColor1 = sf::Color(0u, 0u, 0u, 150u);
	const sf::Color outlineColor = sf::Color(0u, 0u, 0u, 255u);
	const float outlineThickness = 1.0f;

	mBackground.setSize(ui);
	mBackground.setFillColor(backgroundColor0);
	mBackground.setOutlineColor(outlineColor);
	mBackground.setOutlineThickness(outlineThickness);
	mBackground.setPosition(0.0f, 0.0f);

	mMosquitoTextBackground.setSize(sf::Vector2f(element.x * 2.0f + buffer, element.y));
	mMosquitoTextBackground.setFillColor(backgroundColor1);
	mMosquitoTextBackground.setOutlineColor(outlineColor);
	mMosquitoTextBackground.setOutlineThickness(outlineThickness);
	mMosquitoTextBackground.setPosition(0.0f, 0.0f);

	trmb::centerOrigin(mMosquitoText, true, false);
	const float xMosquitoText = std::floor(mMosquitoTextBackground.getPosition().x + mMosquitoTextBackground.getSize().x / 2.0f);
	const float yMosquitoText = mMosquitoTextBackground.getPosition().y - 1.0f;
	mMosquitoText.setPosition(sf::Vector2f(xMosquitoText, yMosquitoText));

	mInfectedMosquitoCountBackground.setSize(element);
	mInfectedMosquitoCountBackground.setFillColor(backgroundColor1);
	mInfectedMosquitoCountBackground.setOutlineColor(outlineColor);
	mInfectedMosquitoCountBackground.setOutlineThickness(outlineThickness);
	mInfectedMosquitoCountBackground.setPosition(0.0f, element.y + buffer);

	mMosquitoTotalBackground.setSize(element);
	mMosquitoTotalBackground.setFillColor(backgroundColor1);
	mMosquitoTotalBackground.setOutlineColor(outlineColor);
	mMosquitoTotalBackground.setOutlineThickness(outlineThickness);
	mMosquitoTotalBackground.setPosition(element.x + buffer, element.y + buffer);

	mInfectedMosquitoCountText.setColor(sf::Color::Red);
	trmb::centerOrigin(mInfectedMosquitoCountText, true, false);
	const float xInfectedMosquitoCountText = std::floor(mInfectedMosquitoCountBackground.getPosition().x + mInfectedMosquitoCountBackground.getSize().x / 2.0f);
	const float yInfectedMosquitoCountText = mInfectedMosquitoCountBackground.getPosition().y - 1.0f;
	mInfectedMosquitoCountText.setPosition(sf::Vector2f(xInfectedMosquitoCountText, yInfectedMosquitoCountText));

	trmb::centerOrigin(mMosquitoTotalText, true, false);
	const float xMosquitoTotalText = std::floor(mMosquitoTotalBackground.getPosition().x + mMosquitoTotalBackground.getSize().x / 2.0f);
	const float yMosquitoTotalText = mMosquitoTotalBackground.getPosition().y - 1.0f;
	mMosquitoTotalText.setPosition(sf::Vector2f(xMosquitoTotalText, yMosquitoTotalText));

	centerOrigin(*this, true, false);
}

sf::Vector2f HouseTrackerUI::getSize() const
{
	return mBackground.getSize();
}

void HouseTrackerUI::addInfectedMosquito()
{
	++mInfectedMosquitoCount;

	mInfectedMosquitoCountText.setString(std::to_string(mInfectedMosquitoCount));
	trmb::centerOrigin(mInfectedMosquitoCountText, true, false);
}

void HouseTrackerUI::subtractInfectedMosquito()
{
	--mInfectedMosquitoCount;
	assert(("Count cannot be less than 0.", 0 <= mInfectedMosquitoCount));

	mInfectedMosquitoCountText.setString(std::to_string(mInfectedMosquitoCount));
	trmb::centerOrigin(mInfectedMosquitoCountText, true, false);
}

void HouseTrackerUI::addMosquitoTotal()
{
	++mMosquitoTotal;

	mMosquitoTotalText.setString(std::to_string(mMosquitoTotal));
	trmb::centerOrigin(mMosquitoTotalText, true, false);
}

void HouseTrackerUI::subtractMosquitoTotal()
{
	--mMosquitoTotal;
	assert(("Count cannot be less than 0.", 0 <= mMosquitoTotal));

	mMosquitoTotalText.setString(std::to_string(mMosquitoTotal));
	trmb::centerOrigin(mMosquitoTotalText, true, false);
}

void HouseTrackerUI::handleEvent(const trmb::Event &gameEvent)
{
	if (mBeginSimulationEvent == gameEvent.getType())
	{
		unhide();
	}
	else if (mBeginScoreboardEvent == gameEvent.getType())
	{
		hide();
	}
}

void HouseTrackerUI::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	if (!mHide)
	{
		target.draw(mBackground, states);
		target.draw(mMosquitoTextBackground, states);
		target.draw(mMosquitoText, states);
		target.draw(mInfectedMosquitoCountBackground, states);
		target.draw(mMosquitoTotalBackground, states);
		target.draw(mInfectedMosquitoCountText, states);
		target.draw(mMosquitoTotalText, states);
	}
}

void HouseTrackerUI::unhide()
{
	mHide = false;
}

void HouseTrackerUI::hide()
{
	mHide = true;
}

void centerOrigin(HouseTrackerUI &ui, bool centerXAxis, bool centerYAxis)
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
