#include "menuState.h"
#include "stateIdentifiers.h"
#include "../Resources/resourceIdentifiers.h"

#include "Trambo/Events/event.h"
#include "Trambo/Localize/localize.h"
#include "Trambo/Utilities/utility.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>


MenuState::MenuState(trmb::StateStack& stack, trmb::State::Context context)
: trmb::State(stack, context)
, mFullscreen(0x5a0d2314)
, mWindowed(0x11e3c735)
, mControlsInst(trmb::Localize::getInstance().getString("controlsInstUI"), context.fonts->get(Fonts::ID::Main), 13u)
, mPlusMouseInst("+", context.fonts->get(Fonts::ID::Main), 13u)
, mEscInst(trmb::Localize::getInstance().getString("escInstUI"), context.fonts->get(Fonts::ID::Main), 13u)
, mPlusFInst("+", context.fonts->get(Fonts::ID::Main), 13u)
, mToggleInst(trmb::Localize::getInstance().getString("toggleInstUI"), context.fonts->get(Fonts::ID::Main), 13u)
, mMosquitoInst(trmb::Localize::getInstance().getString("mosquitoInstUI"), context.fonts->get(Fonts::ID::Main), 13u)
, mMalariaMosquitoInst(trmb::Localize::getInstance().getString("malariaMosquitoInstUI"), context.fonts->get(Fonts::ID::Main), 13u)
, mDamagedNetInst(trmb::Localize::getInstance().getString("damagedNetInstUI"), context.fonts->get(Fonts::ID::Main), 13u)
, mNetInst(trmb::Localize::getInstance().getString("netInstUI"), context.fonts->get(Fonts::ID::Main), 13u)
, mBedInst(trmb::Localize::getInstance().getString("bedInstUI"), context.fonts->get(Fonts::ID::Main), 13u)
, mResidentInst(trmb::Localize::getInstance().getString("residentInstUI"), context.fonts->get(Fonts::ID::Main), 13u)
, mMalariaResidentInst(trmb::Localize::getInstance().getString("malariaResidentInstUI"), context.fonts->get(Fonts::ID::Main), 13u)
, mMalariaHouseMoquitoInst(trmb::Localize::getInstance().getString("malariaHouseMoquitoInstUI"), context.fonts->get(Fonts::ID::Main), 13u)
, mHouseMoquitoInst(trmb::Localize::getInstance().getString("houseMoquitoInstUI"), context.fonts->get(Fonts::ID::Main), 13u)
, mGUIContainer(context.window)
{
	// ALW - Calculate x, y coordinates relative to the center of the window,
	// ALW - so GUI elements are equidistance from the center in any resolution.
	const sf::Vector2f center = sf::Vector2f(context.window->getSize() / 2u);

	mBackgroundSprite.setTexture(context.textures->get(Textures::ID::MenuScreen));
	trmb::centerOrigin(mBackgroundSprite);
	mBackgroundSprite.setPosition(center);

	mText.setFont(context.fonts->get(Fonts::ID::Title));
	mText.setString(trmb::Localize::getInstance().getString("gameTitleUI"));
	mText.setColor(sf::Color(187, 10, 30, 255));
	mText.setCharacterSize(125);
	trmb::centerOrigin(mText);
	mText.setPosition(center - sf::Vector2f(0, 225));

	mControlsInst.setColor(sf::Color::Black);
	mControlsInst.setStyle(sf::Text::Bold);
	trmb::centerOrigin(mControlsInst);
//	mControlsInst.setPosition(151, 355);
	mControlsInst.setPosition(center.x - 249, center.y + 55);

	mKeyboardArrowUp.setTexture(context.textures->get(Textures::ID::KeyboardArrowUp));
	trmb::centerOrigin(mKeyboardArrowUp);
//  mKeyboardArrowUp.setPosition(93, 385);
	mKeyboardArrowUp.setPosition(center.x - 307, center.y + 85);

	mKeyboardArrowDown.setTexture(context.textures->get(Textures::ID::KeyboardArrowDown));
	trmb::centerOrigin(mKeyboardArrowDown);
//  mKeyboardArrowDown.setPosition(93, 420);
	mKeyboardArrowDown.setPosition(center.x - 307, center.y + 120);

	mKeyboardArrowLeft.setTexture(context.textures->get(Textures::ID::KeyboardArrowLeft));
	trmb::centerOrigin(mKeyboardArrowLeft);
//  mKeyboardArrowLeft.setPosition(58, 420);
	mKeyboardArrowLeft.setPosition(center.x - 342, center.y + 120);

	mKeyboardArrowRight.setTexture(context.textures->get(Textures::ID::KeyboardArrowRight));
	trmb::centerOrigin(mKeyboardArrowRight);
//  mKeyboardArrowRight.setPosition(128, 420);
	mKeyboardArrowRight.setPosition(center.x - 272, center.y + 120);

	mPlusMouseInst.setColor(sf::Color::Black);
	mPlusMouseInst.setStyle(sf::Text::Bold);
	trmb::centerOrigin(mPlusMouseInst);
//	mPlusMouseInst.setPosition(169, 402);
	mPlusMouseInst.setPosition(center.x - 231, center.y + 102);

	mKeyboardMouseLeft.setTexture(context.textures->get(Textures::ID::KeyboardMouseLeft));
	trmb::centerOrigin(mKeyboardMouseLeft);
//  mKeyboardMouseLeft.setPosition(210, 402);
	mKeyboardMouseLeft.setPosition(center.x - 190, center.y + 102);

	mKeyboardEsc.setTexture(context.textures->get(Textures::ID::KeyboardEsc));
	trmb::centerOrigin(mKeyboardEsc);
//  mKeyboardEsc.setPosition(58, 485);
	mKeyboardEsc.setPosition(center.x - 342, center.y + 185);

	mEscInst.setColor(sf::Color::Black);
	mEscInst.setStyle(sf::Text::Bold);
	trmb::centerOrigin(mEscInst, false, true);
//	mEscInst.setPosition(92, 485);
	mEscInst.setPosition(center.x - 308, center.y + 185);

	mKeyboardCtrl.setTexture(context.textures->get(Textures::ID::KeyboardCtrl));
	trmb::centerOrigin(mKeyboardCtrl);
//  mKeyboardCtrl.setPosition(58, 525);
	mKeyboardCtrl.setPosition(center.x - 342, center.y + 225);

	mPlusFInst.setColor(sf::Color::Black);
	mPlusFInst.setStyle(sf::Text::Bold);
	trmb::centerOrigin(mPlusFInst);
//  mPlusFInst.setPosition(85, 525);
	mPlusFInst.setPosition(center.x - 315, center.y + 225);

	mKeyboardF.setTexture(context.textures->get(Textures::ID::KeyboardF));
	trmb::centerOrigin(mKeyboardF);
//  mKeyboardF.setPosition(110, 525);
	mKeyboardF.setPosition(center.x - 290, center.y + 225);

	mToggleInst.setColor(sf::Color::Black);
	mToggleInst.setStyle(sf::Text::Bold);
	trmb::centerOrigin(mToggleInst, false, true);
//	mToggleInst.setPosition(144, 525);
	mToggleInst.setPosition(center.x - 256, center.y + 225);

	mMosquitoInst.setColor(sf::Color::Black);
	mMosquitoInst.setStyle(sf::Text::Bold);
//	mMosquitoInst.setPosition(630, 153);
	mMosquitoInst.setPosition(center.x + 230, center.y - 147);
	if (trmb::Localize::getInstance().getLanguage() == "mg")
		mMosquitoInst.setPosition(center.x + 255, center.y - 147);

	mMalariaMosquitoInst.setColor(sf::Color::Black);
	mMalariaMosquitoInst.setStyle(sf::Text::Bold);
//	mMalariaMosquitoInst.setPosition(543, 180);
	mMalariaMosquitoInst.setPosition(center.x + 143, center.y - 120);
	if (trmb::Localize::getInstance().getLanguage() == "mg")
		mMalariaMosquitoInst.setPosition(center.x + 110, center.y - 120);

	mDamagedNetInst.setColor(sf::Color::Black);
	mDamagedNetInst.setStyle(sf::Text::Bold);
//	mDamagedNetInst.setPosition(497, 205);
	mDamagedNetInst.setPosition(center.x + 97, center.y - 95);

	mNetInst.setColor(sf::Color::Black);
	mNetInst.setStyle(sf::Text::Bold);
//	mNetInst.setPosition(497, 232);
	mNetInst.setPosition(center.x + 97, center.y - 68);

	mBedInst.setColor(sf::Color::Black);
	mBedInst.setStyle(sf::Text::Bold);
//	mBedInst.setPosition(497, 261);
	mBedInst.setPosition(center.x + 97, center.y - 39);

	mResidentInst.setColor(sf::Color::Black);
	mResidentInst.setStyle(sf::Text::Bold);
//	mResidentInst.setPosition(562, 368);
	mResidentInst.setPosition(center.x + 162, center.y + 68);

	mMalariaResidentInst.setColor(sf::Color::Black);
	mMalariaResidentInst.setStyle(sf::Text::Bold);
//	mMalariaResidentInst.setPosition(477, 396);
	mMalariaResidentInst.setPosition(center.x + 77, center.y + 96);
	if (trmb::Localize::getInstance().getLanguage() == "mg")
		mMalariaResidentInst.setPosition(center.x + 28, center.y + 96);

	mMalariaHouseMoquitoInst.setColor(sf::Color::Black);
	mMalariaHouseMoquitoInst.setStyle(sf::Text::Bold);
//	mMalariaHouseMoquitoInst.setPosition(292, 550);
	mMalariaHouseMoquitoInst.setPosition(center.x - 108, center.y + 250);
	if (trmb::Localize::getInstance().getLanguage() == "mg")
		mMalariaHouseMoquitoInst.setPosition(center.x - 208, center.y + 250);

	mHouseMoquitoInst.setColor(sf::Color::Black);
	mHouseMoquitoInst.setStyle(sf::Text::Bold);
//	mHouseMoquitoInst.setPosition(378, 573);
	mHouseMoquitoInst.setPosition(center.x - 22, center.y + 273);
	if (trmb::Localize::getInstance().getLanguage() == "mg")
		mHouseMoquitoInst.setPosition(center.x - 17, center.y + 273);

	const float x = center.x - 355.0f;
	const float y = center.y - 130.0f;
	const float buttonHeight = 50.0f;

	mPlayButton = std::make_shared<trmb::Button>(context, Fonts::ID::Main, SoundEffects::ID::Button, Textures::ID::Buttons, 200, 50);
	mPlayButton->setPosition(x, y);
	mPlayButton->setText(trmb::Localize::getInstance().getString("playButton"));
	mPlayButton->setCallback([this] ()
	{
		requestStackPop();
		requestStackPush(States::ID::Game);
	});

	mCreditButton = std::make_shared<trmb::Button>(context, Fonts::ID::Main, SoundEffects::ID::Button, Textures::ID::Buttons, 200, 50);
	mCreditButton->setPosition(x, y + buttonHeight);
	mCreditButton->setText(trmb::Localize::getInstance().getString("creditButton"));
	mCreditButton->setCallback([this]()
	{
		requestStackPush(States::ID::Credits);
	});

	mExitButton = std::make_shared<trmb::Button>(context, Fonts::ID::Main, SoundEffects::ID::Button, Textures::ID::Buttons, 200, 50);
	mExitButton->setPosition(x, y + 2.0f * buttonHeight);
	mExitButton->setText(trmb::Localize::getInstance().getString("exitButton"));
	mExitButton->setCallback([this]()
	{
		requestStackPop();
	});

	mGUIContainer.pack(mPlayButton);
	mGUIContainer.pack(mCreditButton);
	mGUIContainer.pack(mExitButton);
}

void MenuState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.setView(window.getDefaultView());

	window.draw(mBackgroundSprite);
	window.draw(mText);

	window.draw(mControlsInst);
	window.draw(mKeyboardArrowUp);
	window.draw(mKeyboardArrowDown);
	window.draw(mKeyboardArrowLeft);
	window.draw(mKeyboardArrowRight);
	window.draw(mPlusMouseInst);
	window.draw(mKeyboardMouseLeft);
	window.draw(mKeyboardEsc);
	window.draw(mEscInst);
	window.draw(mKeyboardCtrl);
	window.draw(mPlusFInst);
	window.draw(mKeyboardF);
	window.draw(mToggleInst);

	window.draw(mMosquitoInst);
	window.draw(mMalariaMosquitoInst);
	window.draw(mDamagedNetInst);
	window.draw(mNetInst);
	window.draw(mBedInst);
	window.draw(mResidentInst);
	window.draw(mMalariaResidentInst);
	window.draw(mMalariaHouseMoquitoInst);
	window.draw(mHouseMoquitoInst);
	window.draw(mGUIContainer);
}

bool MenuState::update(sf::Time)
{
	return true;
}

bool MenuState::handleEvent(const sf::Event& event)
{
	mGUIContainer.handleEvent(event);
	return false;
}

void MenuState::handleEvent(const trmb::Event &gameEvent)
{
	// ALW - Currently, fullscreen and windowed mode are the same.
	if (mFullscreen == gameEvent.getType() || mWindowed == gameEvent.getType())
	{
		repositionGUI();
	}
}

void MenuState::repositionGUI()
{
	// ALW - Calculate x, y coordinates relative to the center of the window,
	// ALW - so GUI elements are equidistance from the center in any resolution.
	const sf::Vector2f center = sf::Vector2f(getContext().window->getSize() / 2u);

	mBackgroundSprite.setPosition(center);
	mText.setPosition(center - sf::Vector2f(0, 225));

	mControlsInst.setPosition(center.x - 249, center.y + 55);
	mKeyboardArrowUp.setPosition(center.x - 307, center.y + 85);
	mKeyboardArrowDown.setPosition(center.x - 307, center.y + 120);
	mKeyboardArrowLeft.setPosition(center.x - 342, center.y + 120);
	mKeyboardArrowRight.setPosition(center.x - 272, center.y + 120);
	mPlusMouseInst.setPosition(center.x - 231, center.y + 102);
	mKeyboardMouseLeft.setPosition(center.x - 190, center.y + 102);
	mKeyboardEsc.setPosition(center.x - 342, center.y + 185);
	mEscInst.setPosition(center.x - 308, center.y + 185);
	mKeyboardCtrl.setPosition(center.x - 342, center.y + 225);
	mPlusFInst.setPosition(center.x - 315, center.y + 225);
	mKeyboardF.setPosition(center.x - 290, center.y + 225);
	mToggleInst.setPosition(center.x - 256, center.y + 225);

	mMosquitoInst.setPosition(center.x + 230, center.y - 147);
	if (trmb::Localize::getInstance().getLanguage() == "mg")
		mMosquitoInst.setPosition(center.x + 255, center.y - 147);
	mMalariaMosquitoInst.setPosition(center.x + 143, center.y - 120);
	if (trmb::Localize::getInstance().getLanguage() == "mg")
		mMalariaMosquitoInst.setPosition(center.x + 110, center.y - 120);
	mDamagedNetInst.setPosition(center.x + 97, center.y - 95);
	mNetInst.setPosition(center.x + 97, center.y - 68);
	mBedInst.setPosition(center.x + 97, center.y - 39);
	mResidentInst.setPosition(center.x + 162, center.y + 68);
	mMalariaResidentInst.setPosition(center.x + 77, center.y + 96);
	if (trmb::Localize::getInstance().getLanguage() == "mg")
		mMalariaResidentInst.setPosition(center.x + 28, center.y + 96);
	mMalariaHouseMoquitoInst.setPosition(center.x - 108, center.y + 250);
	if (trmb::Localize::getInstance().getLanguage() == "mg")
		mMalariaHouseMoquitoInst.setPosition(center.x - 208, center.y + 250);
	mHouseMoquitoInst.setPosition(center.x - 22, center.y + 273);
	if (trmb::Localize::getInstance().getLanguage() == "mg")
		mHouseMoquitoInst.setPosition(center.x - 17, center.y + 273);

	const float x = center.x - 355.0f;
	const float y = center.y - 130.0f;
	const float buttonHeight = 50.0f;

	mPlayButton->setPosition(sf::Vector2f(x, y));
	mCreditButton->setPosition(sf::Vector2f(x, y + buttonHeight));
	mExitButton->setPosition(sf::Vector2f(x, y + 2.0f * buttonHeight));
}
