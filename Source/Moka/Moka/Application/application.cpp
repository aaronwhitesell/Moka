#include "application.h"

#include "../Resources/resourceIdentifiers.h"
#include "../States/gameState.h"
//#include "../Game/State/gameOverState.h"
#include "../States/menuState.h"
#include "../States/pauseState.h"
#include "../States/creditState.h"
#include "../States/stateIdentifiers.h"
#include "../States/titleState.h"

#include "Trambo/Localize/localize.h"
#include "Trambo/States/state.h"
#include "Trambo/Utilities/utility.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>

#include <string>


const sf::Time Application::TimePerFrame = sf::seconds(1.f / 60.f);

Application::Application()
: mWindow(sf::VideoMode(800, 600), "Moka", sf::Style::Close)
, mTextures()
, mFonts()
, mPlayer()
, mMusic()
, mSounds()
, mStateStack(trmb::State::Context(mWindow, mTextures, mFonts, mPlayer, mMusic, mSounds))
, mStatisticsText()
, mStatisticsUpdateTime()
, mStatisticsNumFrames(0)
, mToggleFullscreen(mWindow)
, mUpdateSkipped(false)
{
	mWindow.setKeyRepeatEnabled(false);

	trmb::Localize::getInstance().setFilePath("Data/Text/Text.xml");
	trmb::Localize::getInstance().setLanguage("en");

	mFonts.load(Fonts::ID::Main, "Data/Fonts/Sansation.ttf");
	mFonts.load(Fonts::ID::Title, "Data/Fonts/Lazy Sans.ttf");

	mTextures.load(Textures::ID::TitleScreen, "Data/Textures/TitleScreen.png");
	mTextures.load(Textures::ID::Buttons, "Data/Textures/Buttons.png");

	mMusic.load(Music::ID::MenuTheme, "Data/Music/MenuTheme.ogg");
	mMusic.load(Music::ID::MissionTheme, "Data/Music/MissionTheme.ogg");

	mSounds.load(SoundEffects::ID::Button, "Data/Sounds/Button.wav");

	mStatisticsText.setFont(mFonts.get(Fonts::ID::Main));
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setCharacterSize(10u);

	registerStates();
	mStateStack.pushState(States::ID::Title);

	mMusic.setVolume(75.f);
}

void Application::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (mWindow.isOpen())
	{
		sf::Time dt = clock.restart();
		timeSinceLastUpdate += dt;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;

			processInput();
			update(TimePerFrame);

			// ALW - An update may be skipped, because the application lost focus.
			// ALW - Don't close the window until an update() has run and then the
			// ALW - mStateStack is empty. Breakpoints were causing the window to
			// ALW - close.
			if (!mUpdateSkipped)
			{
				// ALW - Stack might be empty after call to update(sf::Time)
				if (mStateStack.isEmpty())
					mWindow.close();
			}
		}

		updateStatistics(dt);
		render();
	}
}

void Application::processInput()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		mToggleFullscreen.handleEvent(event);

		if (trmb::isWindowFocused(mWindow))
			mStateStack.handleEvent(event);

		if (event.type == sf::Event::Closed)
			mWindow.close();
	}
}

void Application::update(sf::Time dt)
{
	if (trmb::isWindowFocused(mWindow))
	{
		mStateStack.update(dt);
		mUpdateSkipped = false;
	}
	else
		mUpdateSkipped = true;
}

void Application::render()
{
	// ALW - Clear the background to match menu.
	mWindow.clear(sf::Color(141u, 196u, 53u, 255u));

	mStateStack.draw();

	mWindow.setView(mWindow.getDefaultView());
	mWindow.draw(mStatisticsText);

	mWindow.display();
}

void Application::updateStatistics(sf::Time dt)
{
	mStatisticsUpdateTime += dt;
	mStatisticsNumFrames += 1;
	if (mStatisticsUpdateTime >= sf::seconds(1.0f))
	{
		mStatisticsText.setString("FPS: " + std::to_string(mStatisticsNumFrames));

		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsNumFrames = 0;
	}
}

void Application::registerStates()
{
	mStateStack.registerState<TitleState>(States::ID::Title);
	mStateStack.registerState<MenuState>(States::ID::Menu);
	mStateStack.registerState<GameState>(States::ID::Game);
	mStateStack.registerState<PauseState>(States::ID::Pause);
	mStateStack.registerState<CreditState>(States::ID::Credits);
//	mStateStack.registerState<GameOverState>(States::ID::GameOver);
}
