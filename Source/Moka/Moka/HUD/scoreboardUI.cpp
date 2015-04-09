#include "scoreboardUI.h"

#include "Trambo/Camera/camera.h"
#include "Trambo/Events/event.h"
#include "Trambo/Localize/localize.h"
#include "Trambo/Sounds/soundPlayer.h"
#include "Trambo/Utilities/utility.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <sstream>
#include <iomanip>


ScoreboardUI::ScoreboardUI(const sf::RenderWindow &window, trmb::Camera &camera, trmb::FontHolder &fonts, SoundEffects::ID soundEffect
	, trmb::SoundPlayer &soundPlayer, EventGuid leftClickPress, EventGuid leftClickRelease)
: mFullscreen(0x5a0d2314)
, mWindowed(0x11e3c735)
, mBeginScoreboardEvent(0xf5e88b6e)
, mWindow(window)
, mCamera(camera)
, mFonts(fonts)
, mSoundPlayer(soundPlayer)
, mBackground()
, mResultText()
, mStatsBackground()
, mBarrelDescText(trmb::Localize::getInstance().getString("barrelResultUI"), fonts.get(Fonts::ID::Main), 13u)
, mDoorDescText(trmb::Localize::getInstance().getString("doorResultUI"), fonts.get(Fonts::ID::Main), 13u)
, mWindowDescText(trmb::Localize::getInstance().getString("windowResultUI"), fonts.get(Fonts::ID::Main), 13u)
, mNetDescText(trmb::Localize::getInstance().getString("netResultUI"), fonts.get(Fonts::ID::Main), 13u)
, mCuredDescText(trmb::Localize::getInstance().getString("curedResultUI"), fonts.get(Fonts::ID::Main), 13u)
, mMalariaDescText(trmb::Localize::getInstance().getString("malariaResultUI"), fonts.get(Fonts::ID::Main), 13u)
, mButton(std::make_shared<trmb::GameButton>(Fonts::ID::Main, fonts, soundEffect, soundPlayer, sf::Vector2f(120.0f, 31.0f)))
, mBarrelResultText("", fonts.get(Fonts::ID::Main), 13u)
, mDoorResultText("", fonts.get(Fonts::ID::Main), 13u)
, mWindowResultText("", fonts.get(Fonts::ID::Main), 13u)
, mNetResultText("", fonts.get(Fonts::ID::Main), 13u)
, mCuredResultText("", fonts.get(Fonts::ID::Main), 13u)
, mMalariaResultText("", fonts.get(Fonts::ID::Main), 13u)
, mSpawnedMosquitoes(0)
, mDoorDeflections(0)
, mWindowDeflections(0)
, mNetDeflections(0)
, mCuredResident(0)
, mInfectedResident(1) // ALW - Patient zero
, mContainer(leftClickPress, leftClickRelease)
, mHide(true)
, mDisable(true)
, mFinished(false)
{
}

void ScoreboardUI::initialize(int totalResidents)
{
	// ALW - Initialize text with results
	mBarrelResultText.setString(std::to_string(mSpawnedMosquitoes));
	mDoorResultText.setString(std::to_string(mDoorDeflections));
	mWindowResultText.setString(std::to_string(mWindowDeflections));
	mNetResultText.setString(std::to_string(mNetDeflections));
	mCuredResultText.setString(std::to_string(mCuredResident));
	mMalariaResultText.setString(std::to_string(mInfectedResident));

	// ALW - Position the text
	const float margin = 3.0f;
	const float space = 10.0f;
	const float maxResultTextLength = getMaxResultTextLength();
	const sf::Vector2f resultTextOffset = sf::Vector2f(margin + maxResultTextLength, 100.0f);		// ALW - Right justified
	const sf::Vector2f descTextOffset = sf::Vector2f(margin + maxResultTextLength + space, 100.0f); // ALW - Left justified

	leftJustify(mBarrelResultText);
	mBarrelResultText.setPosition(sf::Vector2f(resultTextOffset));
	mBarrelDescText.setPosition(sf::Vector2f(descTextOffset));

	leftJustify(mDoorResultText);
	mDoorResultText.setPosition(sf::Vector2f(resultTextOffset.x, resultTextOffset.y + 15.0f));
	mDoorDescText.setPosition(sf::Vector2f(descTextOffset.x, descTextOffset.y + 15.0f));

	leftJustify(mWindowResultText);
	mWindowResultText.setPosition(sf::Vector2f(resultTextOffset.x, resultTextOffset.y + 30.0f));
	mWindowDescText.setPosition(sf::Vector2f(descTextOffset.x, descTextOffset.y + 30.0f));

	leftJustify(mNetResultText);
	mNetResultText.setPosition(sf::Vector2f(resultTextOffset.x, resultTextOffset.y + 45.0f));
	mNetDescText.setPosition(sf::Vector2f(descTextOffset.x, descTextOffset.y + 45.0f));

	leftJustify(mCuredResultText);
	mCuredResultText.setPosition(sf::Vector2f(resultTextOffset.x, resultTextOffset.y + 60.0f));
	mCuredDescText.setPosition(sf::Vector2f(descTextOffset.x, descTextOffset.y + 60.0f));

	leftJustify(mMalariaResultText);
	mMalariaResultText.setPosition(sf::Vector2f(resultTextOffset.x, resultTextOffset.y + 75.0f));
	mMalariaDescText.setPosition(sf::Vector2f(descTextOffset.x, descTextOffset.y + 75.0f));

	// ALW - Sizing backgrounds based on text length
	const float backgroundWidth = margin + maxResultTextLength + space + getMaxDescTextLength() + margin;
	const sf::Vector2f backgroundSize = sf::Vector2f(backgroundWidth, 232.0f);
	const sf::Vector2f statsBackgroundSize = sf::Vector2f(backgroundWidth, 93.0f);
	const sf::Color backgroundColor = sf::Color(0u, 0u, 0u, 200u);
	const sf::Color outlineColor = sf::Color(0u, 0u, 0u, 255u);
	const float outlineThickness = 1.0f;

	// ALW - The mBackground is not drawn, but it is used to center the scoreboard.
	mBackground.setSize(backgroundSize);
	mBackground.setFillColor(sf::Color(255u, 0u, 255u, 100u));
	mBackground.setOutlineColor(sf::Color(255u, 0u, 255u, 255u));
	mBackground.setOutlineThickness(outlineThickness);
	mBackground.setPosition(0.0f, 0.0f);

	mStatsBackground.setSize(statsBackgroundSize);
	mStatsBackground.setFillColor(backgroundColor);
	mStatsBackground.setOutlineColor(outlineColor);
	mStatsBackground.setOutlineThickness(outlineThickness);
	mStatsBackground.setPosition(0.0f, 100.0f);

	// ALW - Success / Failure
	const float percent = 20.0f;
	const float infectedResidentsThreshold = totalResidents * percent / 100.0f;
	const bool success = infectedResidentsThreshold >= mInfectedResident;

	const sf::Color malariaRed = sf::Color(187, 10, 30, 255);
	const sf::Color winnerGreen = sf::Color(0, 204, 0, 255);

	if (success)
	{
		mResultText.setColor(winnerGreen);
		mResultText.setString(trmb::Localize::getInstance().getString("successResultUI"));
		mMalariaResultText.setColor(winnerGreen);
		mMalariaDescText.setColor(winnerGreen);
	}
	else
	{
		mResultText.setColor(malariaRed);
		mResultText.setString(trmb::Localize::getInstance().getString("failureResultUI"));
		mMalariaResultText.setColor(malariaRed);
		mMalariaDescText.setColor(malariaRed);
	}

	mResultText.setFont(mFonts.get(Fonts::ID::Title));
	mResultText.setCharacterSize(125);
	trmb::centerOrigin(mResultText, true, false);
	mResultText.setPosition(std::floor(backgroundSize.x / 2.0f), -35.0f);

	mButton->setFont(Fonts::ID::Main);
	mButton->setCharacterSize(20u);
	mButton->setText(trmb::Localize::getInstance().getString("daylightButton"), false);
	mButton->setVisualScheme(sf::Color(95u, 158u, 160u, 255u), sf::Color(255u, 255u, 255u, 255u), sf::Color(0u, 0u, 0u, 255u)
		, sf::Color(162u, 181u, 205u, 255u), sf::Color(255u, 255u, 255u, 255u), sf::Color(0u, 0u, 0u, 255u) // Hover
		, sf::Color(88u, 146u, 148u, 255u), sf::Color(255u, 255u, 255u, 255u), sf::Color(0u, 0u, 0u, 255u)  // Depress
		, sf::Color(0u, 0u, 0u, 0u), sf::Color(224u, 224u, 224u, 75u), sf::Color(0u, 0u, 0u, 255u)	        // Disable
		, 1.0f);
	mButton->setCallback(std::bind(&ScoreboardUI::done, this));
	mButton->setPosition(0.0f, 0.0f);

	mContainer.pack(mButton);
	const float center = std::floor(backgroundSize.x / 2.0f) - std::floor(mButton->getSize().x / 2.0f);
	mContainer.setPosition(center, 202.0f);

	// ALW - Calculate x, y coordinates relative to the center of the window,
	// ALW - so GUI elements are equidistance from the center in any resolution.
	centerOrigin(*this, true, true);
	const sf::Vector2f windowCenter = sf::Vector2f(std::floor(mWindow.getSize().x / 2.0f), std::floor(mWindow.getSize().y / 2.0f));
	setPosition(windowCenter);
}

bool ScoreboardUI::isFinished() const
{
	return mFinished;
}

sf::Vector2f ScoreboardUI::getSize() const
{
	return mBackground.getSize();
}

void ScoreboardUI::addMosquitoSpawn()
{
	++mSpawnedMosquitoes;
}

void ScoreboardUI::addDoorDeflection()
{
	++mDoorDeflections;
}

void ScoreboardUI::addWindowDeflection()
{
	++mWindowDeflections;
}

void ScoreboardUI::addNetDeflection()
{
	++mNetDeflections;
}

void ScoreboardUI::addCuredResident()
{
	++mCuredResident;
}

void ScoreboardUI::addInfectedResident()
{
	++mInfectedResident;
}

void ScoreboardUI::handler()
{
	if (!mDisable)
	{
		// ALW - The ScoreboardUI has an absolute position in the world that is translated by the camera position.
		sf::Vector2f cameraPosition(mCamera.getViewBounds().left, mCamera.getViewBounds().top);
		sf::Transform translatedTransform = getTransform();
		translatedTransform = translatedTransform.translate(cameraPosition);

		mContainer.handler(mWindow, mCamera.getView(), translatedTransform);
	}
}

void ScoreboardUI::handleEvent(const trmb::Event &gameEvent)
{
	// ALW - Currently, fullscreen and windowed mode are the same.
	if (mFullscreen == gameEvent.getType() || mWindowed == gameEvent.getType())
	{
		repositionGUI();
	}
	else if (mBeginScoreboardEvent == gameEvent.getType())
	{
		unhide();
	}
}

void ScoreboardUI::done()
{
	mFinished = true;
}

void ScoreboardUI::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (!mHide)
	{
		states.transform *= getTransform();

		// ALW - Save then change view
		sf::View previousView = target.getView();
		target.setView(target.getDefaultView());

//		target.draw(mBackground, states);
		target.draw(mStatsBackground, states);
		target.draw(mResultText, states);

		target.draw(mBarrelResultText, states);
		target.draw(mBarrelDescText, states);

		target.draw(mDoorResultText, states);
		target.draw(mDoorDescText, states);

		target.draw(mWindowResultText, states);
		target.draw(mWindowDescText, states);

		target.draw(mNetResultText, states);
		target.draw(mNetDescText, states);

		target.draw(mCuredResultText, states);
		target.draw(mCuredDescText, states);

		target.draw(mMalariaResultText, states);
		target.draw(mMalariaDescText, states);

		target.draw(mContainer, states);

		// ALW - Restore the view
		target.setView(previousView);
	}
}

float ScoreboardUI::getMaxResultTextLength()
{
	// ALW - Find the result text with the max length and return it.
	float length = mBarrelResultText.getLocalBounds().width;

	if (length < mDoorResultText.getLocalBounds().width)
		length = mDoorResultText.getLocalBounds().width;

	if (length < mWindowResultText.getLocalBounds().width)
		length = mWindowResultText.getLocalBounds().width;

	if (length < mNetResultText.getLocalBounds().width)
		length = mNetResultText.getLocalBounds().width;

	if (length < mCuredResultText.getLocalBounds().width)
		length = mCuredResultText.getLocalBounds().width;

	if (length < mMalariaResultText.getLocalBounds().width)
		length = mMalariaResultText.getLocalBounds().width;

	return length;
}

float ScoreboardUI::getMaxDescTextLength()
{
	// ALW - Find the description text with the max length and return it.
	float length = mBarrelDescText.getLocalBounds().width;

	if (length < mDoorDescText.getLocalBounds().width)
		length = mDoorDescText.getLocalBounds().width;

	if (length < mWindowDescText.getLocalBounds().width)
		length = mWindowDescText.getLocalBounds().width;

	if (length < mNetDescText.getLocalBounds().width)
		length = mNetDescText.getLocalBounds().width;

	if (length < mCuredDescText.getLocalBounds().width)
		length = mCuredDescText.getLocalBounds().width;

	if (length < mMalariaDescText.getLocalBounds().width)
		length = mMalariaDescText.getLocalBounds().width;

	return length;
}

void ScoreboardUI::leftJustify(sf::Text &text)
{
	text.setOrigin(sf::Vector2f(text.getLocalBounds().width, 0.0f));
}

void ScoreboardUI::enable()
{
	mDisable = false;
}

void ScoreboardUI::disable()
{
	mDisable = true;
}

void ScoreboardUI::unhide()
{
	mHide = false;
	enable();
}

void ScoreboardUI::hide()
{
	mHide = true;
	disable();
}

void ScoreboardUI::repositionGUI()
{
	// ALW - Calculate x, y coordinates relative to the center of the window,
	// ALW - so GUI elements are equidistance from the center in any resolution.
	const sf::Vector2f windowCenter = sf::Vector2f(std::floor(mWindow.getSize().x / 2.0f), std::floor(mWindow.getSize().y / 2.0f));
	setPosition(windowCenter);
}

void centerOrigin(ScoreboardUI &ui, bool centerXAxis, bool centerYAxis)
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
