#include "world.h"
#include "../SceneNodes/preventionNode.h"
#include "../GameObjects/preventionObject.h"
#include "../Resources/resourceIdentifiers.h"

#include "Trambo/Events/event.h"
#include "Trambo/SceneNodes/mapLayerNode.h"
#include "Trambo/SceneNodes/spriteNode.h"
#include "Trambo/Sounds/soundPlayer.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <algorithm>
#include <vector>


World::World(sf::RenderWindow& window, trmb::FontHolder& fonts, trmb::SoundPlayer& sounds)
: mFullscreen(0x5a0d2314)
, mWindowed(0x11e3c735)
, mWindow(window)
, mTarget(window)
, mTextures()
, mFonts(fonts)
, mSounds(sounds)
, mSceneGraph()
, mSceneLayers()
, mWorldBounds(0.f, 0.f, 1600.0, 1600.0)
, mSpawnPosition(mWorldBounds.width / 2.f, mWorldBounds.height / 2.f)
, mCamera(window.getDefaultView(), mWorldBounds)
, mMap("Data/Maps/World.tmx")
, mObjectGroups("Data/Maps/World.tmx")
, mHero(nullptr)
{
	buildScene();
}

void World::update(sf::Time dt)
{
	mSceneGraph.update(dt);					// ALW - Update the hero along with the rest of the scene graph
	mCamera.update(mHero->getPosition());	// ALW - Update the camera position
}

void World::handleEvent(const trmb::Event &gameEvent)
{
	// ALW - Currently, fullscreen and windowed mode are the same.
	if (mFullscreen == gameEvent.getType() || mWindowed == gameEvent.getType())
	{
		mCamera.setSize(mTarget.getDefaultView().getSize());

		// ALW - Manually correct position of camera, necessary when pause menu is active.
		mCamera.update(mHero->getPosition());
	}
}

void World::draw()
{
	mTarget.setView(mCamera.getView());
	mTarget.draw(mSceneGraph);
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
	mSceneLayers[Middleground]->attachChild(std::move(layer1));

	// Add tiled roofs
	std::unique_ptr<trmb::MapLayerNode> layer2(new trmb::MapLayerNode(mMap, 2));
	mSceneLayers[Middleground]->attachChild(std::move(layer2));

	// ALW - Add prevention objects
	std::vector<PreventionObject>::const_iterator prevObjIter = begin(mObjectGroups.getPreventionGroup().getPreventionObjects());
	std::vector<PreventionObject>::const_iterator prevObjIterEnd = end(mObjectGroups.getPreventionGroup().getPreventionObjects());

	for (; prevObjIter != prevObjIterEnd; ++prevObjIter)
		mSceneLayers[Prevention]->attachChild(std::move(std::unique_ptr<PreventionNode>(new PreventionNode(mWindow, mCamera.getView(), *prevObjIter))));

	// Add player's character
	std::unique_ptr<HeroNode> player(new HeroNode(mWorldBounds, mCamera.getView()));
	mHero = player.get();
	mHero->setPosition(mSpawnPosition);
	mSceneLayers[Foreground]->attachChild(std::move(player));
}
