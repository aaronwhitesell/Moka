#include "daylightUI.h"
#include "chatBoxUI.h"
#include "../HUD/optionsUI.h"
#include "../HUD/undoUI.h"
#include "../HUD/uiBundle.h"
#include "../Resources/resourceIdentifiers.h"

#include "Trambo/Camera/camera.h"
#include "Trambo/Events/event.h"
#include "Trambo/Localize/localize.h"
#include "Trambo/Sounds/soundPlayer.h"
#include "Trambo/Utilities/utility.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/Window/Mouse.hpp>

#include <cassert>


DaylightUI::DaylightUI(const sf::RenderWindow &window, trmb::Camera &camera, Fonts::ID font, trmb::FontHolder &fonts
	, SoundEffects::ID soundEffect, trmb::SoundPlayer &soundPlayer, UIBundle &uiBundle, EventGuid leftClickPress
	, EventGuid leftClickRelease)
: mBeginSimulationEvent(0x5000e550)
, mFullscreen(0x5a0d2314)
, mWindowed(0x11e3c735)
, mCreateTextPrompt(0x25e87fd8)
, mClearTextPrompt(0xc1523265)
, mMaxHours(12.0f)
, mMinHours(0.0f)
, mFloatPrecision(3)
, mWindow(window)
, mCamera(camera)
, mFonts(fonts)
, mSoundPlayer(soundPlayer)
, mUIBundle(uiBundle)
, mBackground()
, mDaylightText()
, mHoursText()
, mHourCount(mMaxHours)
, mButton(std::make_shared<trmb::GameButton>(font, fonts, soundEffect, soundPlayer, sf::Vector2f(60.0f, 41.0f)))
, mContainer(leftClickPress, leftClickRelease)
, mDisable(false)
, mHide(false)
, mMouseOver(false)
, mUIBundleDisabled(false)
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
	mButton->setText(trmb::Localize::getInstance().getString("daylightButton"));
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

sf::FloatRect DaylightUI::getRect() const
{
	// ALW - The DaylightUI has an absolute position in the world that is translated by the camera position.
	const sf::Vector2f backgroundPosition = mBackground.getPosition();
	const sf::Vector2f backgroundSize = mBackground.getSize();
	sf::FloatRect rect = sf::FloatRect(backgroundPosition.x, backgroundPosition.y, backgroundSize.x, backgroundSize.y);

	sf::Vector2f cameraPosition(mCamera.getViewBounds().left, mCamera.getViewBounds().top);
	sf::Transform translatedTransform = getTransform();
	translatedTransform = translatedTransform.translate(cameraPosition);
	rect = translatedTransform.transformRect(rect);

	return rect;
}

void DaylightUI::add(float addend)
{
	mHourCount += addend;
	assert(("ALW - Logic Error: The hour count exceeds the max hours!", mMaxHours >= mHourCount));

	mHoursText.setString(trmb::toStringWithPrecision(mHourCount, mFloatPrecision));
	trmb::centerOrigin(mHoursText, true, false);
}

bool DaylightUI::subtract(float subtrahend)
{
	bool ret = false;
	float hourCount = mHourCount - subtrahend;

	if (mMinHours <= hourCount)
	{
		ret = true;
		mHourCount -= subtrahend;
		mHoursText.setString(trmb::toStringWithPrecision(mHourCount, mFloatPrecision));
		trmb::centerOrigin(mHoursText, true, false);
	}

	return ret;
}

void DaylightUI::handler()
{
	mMouseOver = false;

	if (!mDisable)
	{
		// ALW - The DaylightUI has an absolute position in the world that is translated by the camera position.
		sf::Vector2f cameraPosition(mCamera.getViewBounds().left, mCamera.getViewBounds().top);
		sf::Transform translatedTransform = getTransform();
		translatedTransform = translatedTransform.translate(cameraPosition);

		const sf::Vector2i relativeToWindow = sf::Mouse::getPosition(mWindow);
		const sf::Vector2f relativeToWorld = mWindow.mapPixelToCoords(relativeToWindow, mCamera.getView());
		const sf::Vector2f mousePosition = relativeToWorld;

		sf::FloatRect UIRect(mBackground.getPosition().x, mBackground.getPosition().y, mBackground.getSize().x, mBackground.getSize().y);
		UIRect = translatedTransform.transformRect(UIRect);

		if (UIRect.contains(mousePosition))
		{
			mMouseOver = true;

			if (!mUIBundleDisabled)
			{
				mUIBundleDisabled = true;

				if (!mUIBundle.getBarrelUI().isHidden())
					mUIBundle.getBarrelUI().disable(false);

				if (!mUIBundle.getDoorUI().isHidden())
					mUIBundle.getDoorUI().disable(false);

				if (!mUIBundle.getWindowUI().isHidden())
					mUIBundle.getWindowUI().disable(false);

				if (!mUIBundle.getClinicUI().isHidden())
					mUIBundle.getClinicUI().disable(false);

				if (!mUIBundle.getHouseUI().isHidden())
					mUIBundle.getHouseUI().disable(false);
			}
		}
		else
		{
			mMouseOver = false;

			if (mUIBundleDisabled)
			{
				mUIBundleDisabled = false;

				if (!mUIBundle.getBarrelUI().isHidden())
					mUIBundle.getBarrelUI().enable();

				if (!mUIBundle.getDoorUI().isHidden())
					mUIBundle.getDoorUI().enable();

				if (!mUIBundle.getWindowUI().isHidden())
					mUIBundle.getWindowUI().enable();

				if (!mUIBundle.getClinicUI().isHidden())
					mUIBundle.getClinicUI().enable();

				if (!mUIBundle.getHouseUI().isHidden())
					mUIBundle.getHouseUI().enable();
			}
		}

		mContainer.handler(mWindow, mCamera.getView(), translatedTransform);
	}
}

void DaylightUI::handleEvent(const trmb::Event &gameEvent)
{
	// ALW - Currently, fullscreen and windowed mode are the same.
	if (mFullscreen == gameEvent.getType() || mWindowed == gameEvent.getType())
	{
		repositionGUI();
	}
	else if (mCreateTextPrompt == gameEvent.getType())
	{
		disable();
	}
	else if (mClearTextPrompt == gameEvent.getType())
	{
		enable();
	}
}

void DaylightUI::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	// ALW - Save then change view
	sf::View previousView = target.getView();
	target.setView(target.getDefaultView());

	if (!mHide)
	{
		target.draw(mBackground, states);
		target.draw(mDaylightText, states);
		target.draw(mHoursText, states);
	}

	if (!mHide && mMouseOver)
		target.draw(mContainer, states);

	// ALW - Restore the view
	target.setView(previousView);
}

void DaylightUI::enable()
{
	mDisable = false;
	mContainer.enable();
}

void DaylightUI::disable()
{
	mDisable = true;
	mContainer.disable(true);
}

void DaylightUI::hide()
{
	mHide = true;
	disable();
}

void DaylightUI::repositionGUI()
{
	// ALW - Calculate x, y coordinates relative to the center of the window,
	// ALW - so GUI elements are equidistance from the center in any resolution.
	const sf::Vector2f windowCenter = sf::Vector2f(mWindow.getSize() / 2u);
	const float bufferFromTop = 10.0f;
	setPosition(windowCenter.x, bufferFromTop);
}

void DaylightUI::done()
{
	EventHandler::sendEvent(mBeginSimulationEvent);
	mUIBundle.getChatBoxUI().updateText(trmb::Localize::getInstance().getString("simulationGreeting"), true);
	hide();
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
