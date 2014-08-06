#include "world.h"
#include "../Resources/resourceIdentifiers.h"

#include "Trambo/SceneNodes/mapLayerNode.h"
#include "Trambo/SceneNodes/spriteNode.h"
#include "Trambo/Sounds/soundPlayer.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

#include <algorithm>


World::World(sf::RenderTarget& outputTarget, trmb::FontHolder& fonts, trmb::SoundPlayer& sounds)
: mTarget(outputTarget)
, mTextures()
, mFonts(fonts)
, mSounds(sounds)
, mSceneGraph()
, mSceneLayers()
, mWorldBounds(0.f, 0.f, 1600.0, 1600.0)
, mSpawnPosition(mWorldBounds.width / 2.f, mWorldBounds.height / 2.f)
, mCamera(outputTarget.getDefaultView(), mWorldBounds)
, mMap("Data/Maps/World.tmx")
, mHero(nullptr)
{
	loadTextures();
	buildScene();
}

void World::update(sf::Time dt)
{
	mSceneGraph.update(dt);					// ALW - Update the hero along with the rest of the scene graph
	mCamera.update(mHero->getPosition());	// ALW - Update the camera position
}

void World::draw()
{
	mTarget.setView(mCamera.getView());
	mTarget.draw(mSceneGraph);
}

void World::loadTextures()
{
	mTextures.load(Textures::ID::Heroes, "Data/Textures/Heroes.png");
}

void World::buildScene()
{
	// Initialize the different layers
	for (std::size_t i = 0; i < LayerCount; ++i)
	{
		trmb::SceneNode::Ptr layer(new trmb::SceneNode());
		mSceneLayers[i] = layer.get();

		mSceneGraph.attachChild(std::move(layer));
	}

	// Add tiled background
	std::unique_ptr<trmb::MapLayerNode> layer0(new trmb::MapLayerNode(mMap, 0));
	mSceneLayers[Background]->attachChild(std::move(layer0));

	// Add tiled houses
	std::unique_ptr<trmb::MapLayerNode> layer1(new trmb::MapLayerNode(mMap, 1));
	mSceneLayers[Foreground]->attachChild(std::move(layer1));

	// Add tiled roofs
	std::unique_ptr<trmb::MapLayerNode> layer2(new trmb::MapLayerNode(mMap, 2));
	mSceneLayers[Foreground]->attachChild(std::move(layer2));

	// Add player's character
	std::unique_ptr<Hero> player(new Hero(Hero::Type::Wizard, mTextures, mFonts, mWorldBounds));
	mHero = player.get();
	mHero->setPosition(mSpawnPosition);
	mSceneLayers[Middleground]->attachChild(std::move(player));
}
