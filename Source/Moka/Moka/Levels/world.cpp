#include "world.h"
#include "../SceneNodes/barrelNode.h"
#include "../SceneNodes/clinicNode.h"
#include "../SceneNodes/HouseNode.h"
#include "../GameObjects/interactiveObject.h"
#include "../Resources/resourceIdentifiers.h"

#include "Trambo/Events/event.h"
#include "Trambo/SceneNodes/mapLayerNode.h"
#include "Trambo/SceneNodes/spriteNode.h"
#include "Trambo/Sounds/soundPlayer.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <algorithm>


World::World(sf::RenderWindow& window, trmb::FontHolder& fonts, trmb::SoundPlayer& soundPlayer)
: mFullscreen(0x5a0d2314)
, mWindowed(0x11e3c735)
, mWindow(window)
, mTarget(window)
, mTextures()
, mFonts(fonts)
, mSoundPlayer(soundPlayer)
, mSceneGraph()
, mSceneLayers()
, mWorldBounds(0.f, 0.f, 1600.0, 1600.0)
, mSpawnPosition(mWorldBounds.width / 2.f, mWorldBounds.height / 2.f)
, mCamera(window.getDefaultView(), mWorldBounds)
, mMap("Data/Maps/World.tmx")
, mChatBox(window, fonts, soundPlayer)
, mBarrelUI(Fonts::ID::Main, fonts, SoundEffects::ID::Button, soundPlayer, 0x6955d309, 0x128b8b25)
, mClinicUI(Fonts::ID::Main, fonts, SoundEffects::ID::Button, soundPlayer, 0x6955d309, 0x128b8b25)
, mHouseUI(Fonts::ID::Main, fonts, SoundEffects::ID::Button, soundPlayer, 0x6955d309, 0x128b8b25)
, mObjectGroups("Data/Maps/World.tmx")
, mHero(nullptr)
{
	configureUIs();
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
	mTarget.draw(mChatBox);
}

void World::configureUIs()
{
	mBarrelUI.addUIElem("Cover", "Undo");
	mBarrelUI.setSize(sf::Vector2f(75.0f, 20.0f));
	centerOrigin(mBarrelUI, true, false);

	mClinicUI.setTabSize(sf::Vector2f(75.0f, 20.0f));
	mClinicUI.setLHSTabText("RDT");
	mClinicUI.setRHSTabText("ACT");
	centerOrigin(mClinicUI, true, false);

	mHouseUI.setTabSize(sf::Vector2f(75.0f, 20.0f));
	mHouseUI.setLHSTabText("Bed Net");
	mHouseUI.setRHSTabText("Repair");
	centerOrigin(mHouseUI, true, false);
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

	// ALW - Add interactive objects
	std::vector<InteractiveObject>::const_iterator iter    = begin(mObjectGroups.getInteractiveGroup().getInteractiveObjects());
	std::vector<InteractiveObject>::const_iterator iterEnd = end(mObjectGroups.getInteractiveGroup().getInteractiveObjects());

	for (; iter != end(mObjectGroups.getInteractiveGroup().getInteractiveObjects()); ++iter)
	{
		if (iter->getType() == "Barrel")
		{
			mSceneLayers[Interactive]->attachChild(std::move(std::unique_ptr<BarrelNode>(
				new BarrelNode(*iter, mWindow, mCamera.getView(), mBarrelUI, mSoundPlayer, mChatBox))));
		}
		else if (iter->getType() == "Clinic")
		{
			mSceneLayers[Interactive]->attachChild(std::move(std::unique_ptr<ClinicNode>(
				new ClinicNode(*iter, mWindow, mCamera.getView(), mClinicUI, buildAttachedRects(*iter), mSoundPlayer, mChatBox))));
		}
		else if (iter->getType() == "House")
		{
			mSceneLayers[Interactive]->attachChild(std::move(std::unique_ptr<HouseNode>(
				new HouseNode(*iter, mWindow, mCamera.getView(), mHouseUI, buildAttachedRects(*iter), mSoundPlayer, mChatBox))));
		}
		else
		{
			// ALW - TODO - Uncomment when all object types have been handled.
			//assert(("ALW - Logic Error: The interactive object type is not handled!", false));
		}
	}

	// Add player's character
	std::unique_ptr<HeroNode> player(new HeroNode(mWorldBounds, mCamera.getView()));
	mHero = player.get();
	mHero->setPosition(mSpawnPosition);
	mSceneLayers[Foreground]->attachChild(std::move(player));
}

std::vector<sf::FloatRect> World::buildAttachedRects(const InteractiveObject &interactiveObj)
{
	std::vector<InteractiveObject>::const_iterator iter    = begin(mObjectGroups.getInteractiveGroup().getInteractiveObjects());
	std::vector<InteractiveObject>::const_iterator iterEnd = end(mObjectGroups.getInteractiveGroup().getInteractiveObjects());
	std::vector<sf::FloatRect> attachedRects;

	// ALW - Store all the rects of objects (windows, doors, etc) with the house they are attached to. Later
	// ALW - the HouseNode object can use these rects to detect whether it is clicked or an attached object is.
	for (; iter != iterEnd; ++iter)
	{
		if (interactiveObj.getName() == iter->getAttachedTo())
			attachedRects.emplace_back(sf::FloatRect(iter->getX(), iter->getY(), iter->getWidth(), iter->getHeight()));
	}

	return attachedRects;
}
