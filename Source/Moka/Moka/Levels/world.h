#ifndef WORLD_H
#define WORLD_H

#include "../Entities/hero.h"

#include "Trambo/Camera/camera.h"
#include "Trambo/Resources/resourceHolder.h"
#include "Trambo/SceneNodes/sceneNode.h"
#include "Trambo/Tiles/map.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>

#include <array>


namespace sf
{
	class RenderTarget;
}

namespace trmb
{
	class SoundPlayer;
}

class World
{
public:
										World(sf::RenderTarget &outputTarget, trmb::FontHolder &fonts, trmb::SoundPlayer &sounds);
										World(const World &) = delete;
	World &								operator=(const World &) = delete;

	void								update(sf::Time dt);
	void								draw();


private:
	void								loadTextures();
	void								buildScene();


private:
	enum Layer
	{
		Background,
		Middleground,
		Foreground,
		LayerCount
	};


private:
	sf::RenderTarget							&mTarget;
	trmb::TextureHolder							mTextures;
	trmb::FontHolder							&mFonts;
	trmb::SoundPlayer							&mSounds;
	
	trmb::SceneNode								mSceneGraph;
	std::array<trmb::SceneNode *, LayerCount>	mSceneLayers;

	sf::FloatRect								mWorldBounds;
	sf::Vector2f								mSpawnPosition;

	trmb::Camera								mCamera;
	trmb::Map									mMap;
	Hero										*mHero;
};

#endif
