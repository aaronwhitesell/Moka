#include "mainTrackerUI.h"

#include "Trambo/Events/event.h"
#include "Trambo/Localize/localize.h"
#include "Trambo/Sounds/soundPlayer.h"
#include "Trambo/Utilities/utility.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <cassert>
#include <cmath>
#include <string>


MainTrackerUI::MainTrackerUI(const sf::RenderWindow &window, Fonts::ID font, const trmb::FontHolder &fonts, SoundEffects::ID soundEffect
	, trmb::SoundPlayer &soundPlayer)
: mBeginSimulationEvent(0x5000e550)
, mFullscreen(0x5a0d2314)
, mWindowed(0x11e3c735)
, mWindow(window)
, mSoundPlayer(soundPlayer)
, mBackground()
, mMosquitoTextBackground()
, mMosquitoText(trmb::Localize::getInstance().getString("mosquitoesUI"), fonts.get(font), 13u)
, mInfectedMosquitoTextBackground()
, mTotalMosquitoTextBackground()
, mInfectedMosquitoText(trmb::Localize::getInstance().getString("infectedUI"), fonts.get(font), 13u)
, mTotalMosquitoText(trmb::Localize::getInstance().getString("totalUI"), fonts.get(font), 13u)
, mInfectedMosquitoCountBackground()
, mMosquitoCountBackground()
, mInfectedMosquitoCountText("0", fonts.get(font), 13u)
, mMosquitoCountText("0", fonts.get(font), 13u)
, mInfectedMosquitoCount(0)
, mMosquitoCount(0)
, mResidentTextTextBackground()
, mResidentText(trmb::Localize::getInstance().getString("residentsUI"), fonts.get(font), 13u)
, mInfectedResidentTextBackground()
, mTotalResidentTextBackground()
, mInfectedResidentText(trmb::Localize::getInstance().getString("infectedUI"), fonts.get(font), 13u)
, mTotalResidentText(trmb::Localize::getInstance().getString("totalUI"), fonts.get(font), 13u)
, mInfectedResidentCountBackground()
, mResidentCountBackground()
, mInfectedResidentCountText("0", fonts.get(font), 13u)
, mResidentCountText("0", fonts.get(font), 13u)
, mInfectedResidentCount(0)
, mResidentCount(0)
, mHide(true)
{
	const float elementbuffer = 3.0f;	// ALW - 1 outline, 1 space, 1 outline
	const float uiBuffer = 10.0f;		// ALW - 1 outline, 8 spaces, 1 outline
	const sf::Vector2f element = sf::Vector2f(76.0f, 14.0f);
	const sf::Vector2f mosquitoUI = sf::Vector2f(element.x * 2.0f + elementbuffer, element.y * 3.0f + elementbuffer * 2.0f);
	const sf::Vector2f ResidentUI = sf::Vector2f(element.x * 2.0f + elementbuffer, element.y * 3.0f + elementbuffer * 2.0f);
	const sf::Vector2f ui = sf::Vector2f(mosquitoUI.x + ResidentUI.x + uiBuffer, mosquitoUI.y);
	const sf::Color backgroundColor = sf::Color(0u, 0u, 0u, 75u);
	const sf::Color outlineColor = sf::Color(0u, 0u, 0u, 255u);
	const float outlineThickness = 1.0f;

	// ALW - Used to align the mosquito and resident UI.
	mBackground.setSize(ui);
	mBackground.setFillColor(backgroundColor);
	mBackground.setOutlineColor(outlineColor);
	mBackground.setOutlineThickness(outlineThickness);
	mBackground.setPosition(0.0f, 0.0f);

	buildMosquitoUI();
	buildResidentUI();

	// ALW - Calculate x, y coordinates relative to the center of the window,
	// ALW - so GUI elements are equidistance from the center in any resolution.
	const sf::Vector2f windowCenter = sf::Vector2f(mWindow.getSize() / 2u);
	const float bufferFromTop = 10.0f;
	centerOrigin(*this, true, false);
	setPosition(windowCenter.x, bufferFromTop);
}

void MainTrackerUI::buildMosquitoUI()
{
	const float elementBuffer = 3.0f; // ALW - 1 outline, 1 space, 1 outline
	const sf::Vector2f element = sf::Vector2f(76.0f, 14.0f);
	const sf::Color backgroundColor = sf::Color(0u, 0u, 0u, 150u);
	const sf::Color outlineColor = sf::Color(0u, 0u, 0u, 255u);
	const float outlineThickness = 1.0f;

	mMosquitoTextBackground.setSize(sf::Vector2f(element.x + element.x + elementBuffer, element.y));
	mMosquitoTextBackground.setFillColor(backgroundColor);
	mMosquitoTextBackground.setOutlineColor(outlineColor);
	mMosquitoTextBackground.setOutlineThickness(outlineThickness);
	mMosquitoTextBackground.setPosition(0.0f, 0.0f);

	trmb::centerOrigin(mMosquitoText, true, false);
	const float xMosquitoText = std::floor(mMosquitoTextBackground.getPosition().x + mMosquitoTextBackground.getSize().x / 2.0f);
	const float yMosquitoText = mMosquitoTextBackground.getPosition().y - 1.0f;
	mMosquitoText.setPosition(sf::Vector2f(xMosquitoText, yMosquitoText));

	mInfectedMosquitoTextBackground.setSize(element);
	mInfectedMosquitoTextBackground.setFillColor(backgroundColor);
	mInfectedMosquitoTextBackground.setOutlineColor(outlineColor);
	mInfectedMosquitoTextBackground.setOutlineThickness(outlineThickness);
	mInfectedMosquitoTextBackground.setPosition(0.0f, element.y + elementBuffer);

	mTotalMosquitoTextBackground.setSize(element);
	mTotalMosquitoTextBackground.setFillColor(backgroundColor);
	mTotalMosquitoTextBackground.setOutlineColor(outlineColor);
	mTotalMosquitoTextBackground.setOutlineThickness(outlineThickness);
	mTotalMosquitoTextBackground.setPosition(element.x + elementBuffer, element.y + elementBuffer);

	mInfectedMosquitoText.setColor(sf::Color::Red);
	trmb::centerOrigin(mInfectedMosquitoText, true, false);
	const float xInfectedMosquitoText = std::floor(mInfectedMosquitoTextBackground.getPosition().x + mInfectedMosquitoTextBackground.getSize().x / 2.0f);
	const float yInfectedMosquitoText = mInfectedMosquitoTextBackground.getPosition().y - 1.0f;
	mInfectedMosquitoText.setPosition(sf::Vector2f(xInfectedMosquitoText, yInfectedMosquitoText));

	trmb::centerOrigin(mTotalMosquitoText, true, false);
	const float xTotalMosquitoText = std::floor(mTotalMosquitoTextBackground.getPosition().x + mTotalMosquitoTextBackground.getSize().x / 2.0f);
	const float yTotalMosquitoText = mTotalMosquitoTextBackground.getPosition().y - 1.0f;
	mTotalMosquitoText.setPosition(sf::Vector2f(xTotalMosquitoText, yTotalMosquitoText));

	mInfectedMosquitoCountBackground.setSize(element);
	mInfectedMosquitoCountBackground.setFillColor(backgroundColor);
	mInfectedMosquitoCountBackground.setOutlineColor(outlineColor);
	mInfectedMosquitoCountBackground.setOutlineThickness(outlineThickness);
	mInfectedMosquitoCountBackground.setPosition(0.0f, element.y * 2.0f + elementBuffer * 2.0f);

	mMosquitoCountBackground.setSize(element);
	mMosquitoCountBackground.setFillColor(backgroundColor);
	mMosquitoCountBackground.setOutlineColor(outlineColor);
	mMosquitoCountBackground.setOutlineThickness(outlineThickness);
	mMosquitoCountBackground.setPosition(element.x + elementBuffer, element.y * 2.0f + elementBuffer * 2.0f);

	mInfectedMosquitoCountText.setColor(sf::Color::Red);
	trmb::centerOrigin(mInfectedMosquitoCountText, true, false);
	const float xInfectedMosquitoCountText = std::floor(mInfectedMosquitoCountBackground.getPosition().x + mInfectedMosquitoCountBackground.getSize().x / 2.0f);
	const float yInfectedMosquitoCountText = mInfectedMosquitoCountBackground.getPosition().y - 1.0f;
	mInfectedMosquitoCountText.setPosition(sf::Vector2f(xInfectedMosquitoCountText, yInfectedMosquitoCountText));

	trmb::centerOrigin(mMosquitoCountText, true, false);
	const float xMosquitoCountText = std::floor(mMosquitoCountBackground.getPosition().x + mMosquitoCountBackground.getSize().x / 2.0f);
	const float yMosquitoCountText = mMosquitoCountBackground.getPosition().y - 1.0f;
	mMosquitoCountText.setPosition(sf::Vector2f(xMosquitoCountText, yMosquitoCountText));
}

void MainTrackerUI::buildResidentUI()
{
	const float elementBuffer = 3.0f;   // ALW - 1 outline, 1 space, 1 outline
	const float uiBuffer = 10.0f;		// ALW - 1 outline, 8 space, 1 outline
	const sf::Vector2f element = sf::Vector2f(76.0f, 14.0f);
	const float horizontalOffset = element.x * 2.0f + elementBuffer + uiBuffer;
	const sf::Color backgroundColor = sf::Color(0u, 0u, 0u, 150u);
	const sf::Color outlineColor = sf::Color(0u, 0u, 0u, 255u);
	const float outlineThickness = 1.0f;

	mResidentTextTextBackground.setSize(sf::Vector2f(element.x + element.x + elementBuffer, element.y));
	mResidentTextTextBackground.setFillColor(backgroundColor);
	mResidentTextTextBackground.setOutlineColor(outlineColor);
	mResidentTextTextBackground.setOutlineThickness(outlineThickness);
	mResidentTextTextBackground.setPosition(horizontalOffset, 0.0f);

	trmb::centerOrigin(mResidentText, true, false);
	const float xResidentText = std::floor(mResidentTextTextBackground.getPosition().x + mResidentTextTextBackground.getSize().x / 2.0f);
	const float yResidentText = mResidentTextTextBackground.getPosition().y - 1.0f;
	mResidentText.setPosition(sf::Vector2f(xResidentText, yResidentText));

	mInfectedResidentTextBackground.setSize(element);
	mInfectedResidentTextBackground.setFillColor(backgroundColor);
	mInfectedResidentTextBackground.setOutlineColor(outlineColor);
	mInfectedResidentTextBackground.setOutlineThickness(outlineThickness);
	mInfectedResidentTextBackground.setPosition(horizontalOffset, element.y + elementBuffer);

	mTotalResidentTextBackground.setSize(element);
	mTotalResidentTextBackground.setFillColor(backgroundColor);
	mTotalResidentTextBackground.setOutlineColor(outlineColor);
	mTotalResidentTextBackground.setOutlineThickness(outlineThickness);
	mTotalResidentTextBackground.setPosition(horizontalOffset + element.x + elementBuffer, element.y + elementBuffer);

	mInfectedResidentText.setColor(sf::Color::Red);
	trmb::centerOrigin(mInfectedResidentText, true, false);
	const float xTextInfectedResident = mInfectedResidentTextBackground.getPosition().x + mInfectedResidentTextBackground.getSize().x / 2.0f;
	const float yTextInfectedResident = mInfectedResidentTextBackground.getPosition().y - 1.0f;
	mInfectedResidentText.setPosition(sf::Vector2f(xTextInfectedResident, yTextInfectedResident));

	trmb::centerOrigin(mTotalResidentText, true, false);
	const float xTextTotalResident = mTotalResidentTextBackground.getPosition().x + mTotalResidentTextBackground.getSize().x / 2.0f;
	const float yTextTotalResident = mTotalResidentTextBackground.getPosition().y - 1.0f;
	mTotalResidentText.setPosition(sf::Vector2f(xTextTotalResident, yTextTotalResident));

	mInfectedResidentCountBackground.setSize(element);
	mInfectedResidentCountBackground.setFillColor(backgroundColor);
	mInfectedResidentCountBackground.setOutlineColor(outlineColor);
	mInfectedResidentCountBackground.setOutlineThickness(outlineThickness);
	mInfectedResidentCountBackground.setPosition(horizontalOffset, element.y * 2.0f + elementBuffer * 2.0f);

	mResidentCountBackground.setSize(element);
	mResidentCountBackground.setFillColor(backgroundColor);
	mResidentCountBackground.setOutlineColor(outlineColor);
	mResidentCountBackground.setOutlineThickness(outlineThickness);
	mResidentCountBackground.setPosition(horizontalOffset + element.x + elementBuffer, element.y * 2.0f + elementBuffer * 2.0f);

	mInfectedResidentCountText.setColor(sf::Color::Red);
	trmb::centerOrigin(mInfectedResidentCountText, true, false);
	const float xInfectedResidentCountText = mInfectedResidentCountBackground.getPosition().x + mInfectedResidentCountBackground.getSize().x / 2.0f;
	const float yInfectedResidentCountText = mInfectedResidentCountBackground.getPosition().y - 1.0f;
	mInfectedResidentCountText.setPosition(sf::Vector2f(xInfectedResidentCountText, yInfectedResidentCountText));

	trmb::centerOrigin(mResidentCountText, true, false);
	const float xResidentCountText = mResidentCountBackground.getPosition().x + mResidentCountBackground.getSize().x / 2.0f;
	const float yResidentCountText = mResidentCountBackground.getPosition().y - 1.0f;
	mResidentCountText.setPosition(sf::Vector2f(xResidentCountText, yResidentCountText));
}

sf::Vector2f MainTrackerUI::getSize() const
{
	return mBackground.getSize();
}

int MainTrackerUI::getMosquitoCount() const
{
	return mMosquitoCount;
}

void MainTrackerUI::setMosquitoCount(int count)
{
	mMosquitoCount = count;

	mMosquitoCountText.setString(std::to_string(mMosquitoCount));
	trmb::centerOrigin(mMosquitoCountText, true, false);
}

void MainTrackerUI::setResidentCount(int count)
{
	mResidentCount = count;

	mResidentCountText.setString(std::to_string(mResidentCount));
	trmb::centerOrigin(mResidentCountText, true, false);
}

void MainTrackerUI::addMosquito()
{
	++mMosquitoCount;

	mMosquitoCountText.setString(std::to_string(mMosquitoCount));
	trmb::centerOrigin(mMosquitoCountText, true, false);
}

void MainTrackerUI::addInfectedMosquito()
{
	++mInfectedMosquitoCount;

	mInfectedMosquitoCountText.setString(std::to_string(mInfectedMosquitoCount));
	trmb::centerOrigin(mInfectedMosquitoCountText, true, false);
}

void MainTrackerUI::subtractInfectedMosquito()
{
	--mInfectedMosquitoCount;
	assert(("Count cannot be less than 0.", 0 <= mInfectedMosquitoCount));

	mInfectedMosquitoCountText.setString(std::to_string(mInfectedMosquitoCount));
	trmb::centerOrigin(mInfectedMosquitoCountText, true, false);
}

void MainTrackerUI::addInfectedResident()
{
	++mInfectedResidentCount;

	mInfectedResidentCountText.setString(std::to_string(mInfectedResidentCount));
	trmb::centerOrigin(mInfectedResidentCountText, true, false);
}

void MainTrackerUI::subtractInfectedResident()
{
	--mInfectedResidentCount;
	assert(("Count cannot be less than 0.", 0 <= mInfectedResidentCount));

	mInfectedResidentCountText.setString(std::to_string(mInfectedResidentCount));
	trmb::centerOrigin(mInfectedResidentCountText, true, false);
}

void MainTrackerUI::handleEvent(const trmb::Event &gameEvent)
{
	// ALW - Currently, fullscreen and windowed mode are the same.
	if (mFullscreen == gameEvent.getType() || mWindowed == gameEvent.getType())
	{
		repositionGUI();
	}
	else if (mBeginSimulationEvent == gameEvent.getType())
	{
		unhide();
	}
}

void MainTrackerUI::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	// ALW - Save then change view
	sf::View previousView = target.getView();
	target.setView(target.getDefaultView());

	if (!mHide)
	{
		target.draw(mBackground, states);

		// ALW - Mosquito UI
		target.draw(mMosquitoTextBackground, states);
		target.draw(mMosquitoText, states);
		target.draw(mInfectedMosquitoTextBackground, states);
		target.draw(mTotalMosquitoTextBackground, states);
		target.draw(mInfectedMosquitoText, states);
		target.draw(mTotalMosquitoText, states);
		target.draw(mInfectedMosquitoCountBackground, states);
		target.draw(mMosquitoCountBackground, states);
		target.draw(mInfectedMosquitoCountText, states);
		target.draw(mMosquitoCountText, states);

		// ALW - Resident UI
		target.draw(mResidentTextTextBackground, states);
		target.draw(mResidentText, states);
		target.draw(mInfectedResidentTextBackground, states);
		target.draw(mTotalResidentTextBackground, states);
		target.draw(mInfectedResidentText, states);
		target.draw(mTotalResidentText, states);
		target.draw(mInfectedResidentCountBackground, states);
		target.draw(mResidentCountBackground, states);
		target.draw(mInfectedResidentCountText, states);
		target.draw(mResidentCountText, states);
	}

	// ALW - Restore the view
	target.setView(previousView);
}

void MainTrackerUI::unhide()
{
	mHide = false;
}

void MainTrackerUI::hide()
{
	mHide = true;
}

void MainTrackerUI::repositionGUI()
{
	// ALW - Calculate x, y coordinates relative to the center of the window,
	// ALW - so GUI elements are equidistance from the center in any resolution.
	const sf::Vector2f windowCenter = sf::Vector2f(mWindow.getSize() / 2u);
	const float bufferFromTop = 10.0f;
	setPosition(windowCenter.x, bufferFromTop);
}

void centerOrigin(MainTrackerUI &ui, bool centerXAxis, bool centerYAxis)
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
