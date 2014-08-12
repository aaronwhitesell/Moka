#ifndef APPLICATION_H
#define APPLICATION_H

#include "toggleFullscreen.h"
#include "../Player/player.h"

#include "Trambo/Resources/resourceHolder.h"
#include "Trambo/Sounds/musicPlayer.h"
#include "Trambo/Sounds/soundPlayer.h"
#include "Trambo/States/stateStack.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Time.hpp>


class Application
{
public:
							Application();
	void					run();


private:
	void					processInput();
	void					update(sf::Time dt);
	void					render();

	void					updateStatistics(sf::Time dt);
	void					registerStates();


private:
	static const sf::Time	TimePerFrame;

	sf::RenderWindow		mWindow;
	trmb::TextureHolder		mTextures;
	trmb::FontHolder		mFonts;
	Player					mPlayer;

	trmb::MusicPlayer		mMusic;
	trmb::SoundPlayer		mSounds;
	trmb::StateStack		mStateStack;

	sf::Text				mStatisticsText;
	sf::Time				mStatisticsUpdateTime;
	std::size_t				mStatisticsNumFrames;

	ToggleFullscreen		mToggleFullscreen;
};

#endif
