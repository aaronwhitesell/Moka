#include "daylightUI.h"
#include "../Resources/resourceIdentifiers.h"

#include "Trambo/Events/event.h"
#include "Trambo/Localize/localize.h"
#include "Trambo/Sounds/soundPlayer.h"
#include "Trambo/Utilities/utility.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/Window/Mouse.hpp>


DaylightUI::DaylightUI(sf::RenderWindow &window, Fonts::ID font, trmb::FontHolder &fonts, SoundEffects::ID soundEffect
	, trmb::SoundPlayer &soundPlayer, EventGuid leftClickPress, EventGuid leftClickRelease)
: mFullscreen(0x5a0d2314)
, mWindowed(0x11e3c735)
, mFloatPrecision(3)
, mWindow(window)
, mFonts(fonts)
, mSoundPlayer(soundPlayer)
, mBackground()
, mDaylightText()
, mHoursText()
, mHourCount(12.0f)
, mButton(std::make_shared<trmb::GameButton>(font, fonts, soundEffect, soundPlayer, sf::Vector2f(60.0f, 41.0f)))
, mContainer(leftClickPress, leftClickRelease)
, mMouseOver(false)
{
	mBackground.setSize(sf::Vector2f(60.0f, 41.0f));
	mBackground.setFillColor(sf::Color(0u, 0u, 0u, 200u));
	mBackground.setOutlineColor(sf::Color(0u, 0u, 0u, 255u));
	mBackground.setOutlineThickness(1.0f);
	mBackground.setPosition(0.0f, 0.0f);

	mDaylightText.setFont(mFonts.get(font));
	mDaylightText.setCharacterSize(14u);
	mDaylightText.setString(trmb::Localize::getInstance().getString("daylightUI"));
	trmb::centerOrigin(mDaylightText, true, false);
	mDaylightText.setPosition(sf::Vector2f(mBackground.getSize().x / 2.0f, 0.0f));

	mHoursText.setFont(mFonts.get(font));
	mHoursText.setCharacterSize(22u);
	mHoursText.setString(trmb::toStringWithPrecision(mHourCount, mFloatPrecision));
	trmb::centerOrigin(mHoursText, true, false);
	mHoursText.setPosition(sf::Vector2f(mBackground.getSize().x / 2.0f, 15.0f));

	mButton->setFont(font);
	mButton->setCharacterSize(22u);
	mButton->setText(trmb::Localize::getInstance().getString("daylightUIButton"));
	mButton->setVisualScheme(sf::Color(95u, 158u, 160u, 255u), sf::Color(255u, 255u, 255u, 255u), sf::Color(0u, 0u, 0u, 255u)
		, sf::Color(162u, 181u, 205u, 255u), sf::Color(255u, 255u, 255u, 255u), sf::Color(0u, 0u, 0u, 255u) // Hover
		, sf::Color(88u, 146u, 148u, 255u), sf::Color(255u, 255u, 255u, 255u), sf::Color(0u, 0u, 0u, 255u)  // Depress
		, sf::Color(0u, 0u, 0u, 0u), sf::Color(224u, 224u, 224u, 75u), sf::Color(0u, 0u, 0u, 255u)	        // Disable
		, 1.0f);
	mButton->setCallback(std::bind(&DaylightUI::done, this));
	mButton->setPosition(0.0f, 0.0f);

	mContainer.pack(mButton);
	mContainer.setPosition(0.0f, 0.0f);

	// ALW - Calculate x, y coordinates relative to the center of the window,
	// ALW - so GUI elements are equidistance from the center in any resolution.
	const sf::Vector2f windowCenter = sf::Vector2f(mWindow.getSize() / 2u);
	const float bufferFromTop = 10.0f;
	centerOrigin(*this, true, false);
	setPosition(windowCenter.x, bufferFromTop);
}

sf::Vector2f DaylightUI::getSize() const
{
	return mBackground.getSize();
}

void DaylightUI::add(float addend)
{
	mHourCount += addend;
	mHoursText.setString(trmb::toStringWithPrecision(mHourCount, mFloatPrecision));
	trmb::centerOrigin(mHoursText, true, false);
}

void DaylightUI::subtract(float subtrahend)
{
	mHourCount -= subtrahend;
	mHoursText.setString(trmb::toStringWithPrecision(mHourCount, mFloatPrecision));
	trmb::centerOrigin(mHoursText, true, false);
}

void DaylightUI::handler(const sf::RenderWindow &window, const sf::View &view, const sf::Transform &transform)
{
	sf::Transform combinedTransform = getTransform() * transform;

	const sf::Vector2i relativeToWindow = sf::Mouse::getPosition(window);
	const sf::Vector2f relativeToWorld = window.mapPixelToCoords(relativeToWindow, view);
	const sf::Vector2f mousePosition = relativeToWorld;

	// ALW - The UI origin is centered on the x-axis, so account for this.
	sf::FloatRect UIRect(getPosition().x - mBackground.getSize().x / 2.0f, getPosition().y, mBackground.getSize().x, mBackground.getSize().y);
	UIRect = transform.transformRect(UIRect);

	if (UIRect.contains(mousePosition))
	{
		mMouseOver = true;
	}
	else
	{
		mMouseOver = false;
	}

	mContainer.handler(window, view, combinedTransform);
}

void DaylightUI::handleEvent(const trmb::Event &gameEvent)
{
	// ALW - Currently, fullscreen and windowed mode are the same.
	if (mFullscreen == gameEvent.getType() || mWindowed == gameEvent.getType())
	{
		repositionGUI();
	}
}

void DaylightUI::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.setView(target.getDefaultView());
	target.draw(mBackground, states);
	target.draw(mDaylightText, states);
	target.draw(mHoursText, states);
	if (mMouseOver)
		target.draw(mContainer, states);
}

void DaylightUI::repositionGUI()
{
	// ALW - Calculate x, y coordinates relative to the center of the window,
	// ALW - so GUI elements are equidistance from the center in any resolution.
	const sf::Vector2f windowCenter = sf::Vector2f(mWindow.getSize() / 2u);
	const float bufferFromTop = 10.0f;
	setPosition(windowCenter.x, bufferFromTop);
}

void centerOrigin(DaylightUI &ui, bool centerXAxis, bool centerYAxis)
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

void DaylightUI::done()
{
	// ALW - TODO - Transition to simulation mode.
}
