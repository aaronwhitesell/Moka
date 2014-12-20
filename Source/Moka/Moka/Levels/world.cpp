#include "world.h"
#include "../SceneNodes/barrelNode.h"
#include "../SceneNodes/barrelUpdateNode.h"
#include "../SceneNodes/barrelUINode.h"
#include "../SceneNodes/doorNode.h"
#include "../SceneNodes/doorUINode.h"
#include "../SceneNodes/doorUpdateNode.h"
#include "../SceneNodes/windowNode.h"
#include "../SceneNodes/windowUINode.h"
#include "../SceneNodes/windowUpdateNode.h"
#include "../SceneNodes/clinicNode.h"
#include "../SceneNodes/clinicUINode.h"
#include "../SceneNodes/clinicUpdateNode.h"
#include "../SceneNodes/houseNode.h"
#include "../SceneNodes/houseUINode.h"
#include "../SceneNodes/houseUpdateNode.h"
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
, mDaylightUI(window, Fonts::ID::Main, fonts, SoundEffects::ID::Button, soundPlayer)
, mBarrelUI(Fonts::ID::Main, fonts, SoundEffects::ID::Button, soundPlayer, 0x6955d309, 0x128b8b25)
, mDoorUI(Fonts::ID::Main, fonts, SoundEffects::ID::Button, soundPlayer, 0x6955d309, 0x128b8b25)
, mWindowUI(Fonts::ID::Main, fonts, SoundEffects::ID::Button, soundPlayer, 0x6955d309, 0x128b8b25)
, mClinicUI(Fonts::ID::Main, fonts, SoundEffects::ID::Button, soundPlayer, 0x6955d309, 0x128b8b25)
, mHouseUI(Fonts::ID::Main, fonts, SoundEffects::ID::Button, soundPlayer, 0x6955d309, 0x128b8b25)
, mUIBundle(mBarrelUI, mDoorUI, mWindowUI, mClinicUI, mHouseUI)
, mObjectGroups("Data/Maps/World.tmx")
, mHero(nullptr)
{
	mTextures.load(Textures::ID::Tiles, "Data/Textures/Tiles.png");
	configureUIs();
	buildScene();
}

void World::update(sf::Time dt)
{
	mSceneGraph.update(dt);					// ALW - Update the hero along with the rest of the scene graph
	mCamera.update(mHero->getPosition());	// ALW - Update the camera position
	updateSoundPlayer();
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
	mTarget.draw(mDaylightUI);
}

void World::updateSoundPlayer()
{
	// ALW - Delete sound effects that have finished playing.
	mSoundPlayer.removeStoppedSounds();
}

void World::configureUIs()
{
	const unsigned int characterSize = 13;

	mBarrelUI.addUIElem("Cover", "Undo");
	mBarrelUI.setSize(sf::Vector2f(75.0f, 20.0f));
	mBarrelUI.setCharacterSize(characterSize);
	centerOrigin(mBarrelUI, true, false);
	mBarrelUI.hide();

	mDoorUI.addUIElem("Close", "Undo");
	mDoorUI.setSize(sf::Vector2f(75.0f, 20.0f));
	mDoorUI.setCharacterSize(characterSize);
	centerOrigin(mDoorUI, true, false);
	mDoorUI.hide();

	mWindowUI.addUIElem("Screen", "Undo");
	mWindowUI.addUIElem("Close", "Undo");
	mWindowUI.setSize(sf::Vector2f(75.0f, 20.0f));
	mWindowUI.setCharacterSize(characterSize);
	centerOrigin(mWindowUI, true, false);
	mWindowUI.hide();

	mClinicUI.setTabSize(sf::Vector2f(75.0f, 20.0f));
	mClinicUI.setLHSTabText("RDTs");
	mClinicUI.setRHSTabText("ACTs");
	mClinicUI.setCharacterSize(characterSize);
	centerOrigin(mClinicUI, true, false);
	mClinicUI.hide();

	mHouseUI.setTabSize(sf::Vector2f(75.0f, 20.0f));
	mHouseUI.setLHSTabText("Bed Net");
	mHouseUI.setRHSTabText("Repair");
	mHouseUI.setCharacterSize(characterSize);
	centerOrigin(mHouseUI, true, false);
	mHouseUI.hide();
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
	mSceneLayers[Background]->attachChild(std::move(layer1));

	// Add tiled roofs
	std::unique_ptr<trmb::MapLayerNode> layer2(new trmb::MapLayerNode(mMap, 2));
	mSceneLayers[Background]->attachChild(std::move(layer2));

	// ALW - Add sprite and logic nodes
	std::vector<InteractiveObject>::const_iterator iter    = begin(mObjectGroups.getInteractiveGroup().getInteractiveObjects());
	std::vector<InteractiveObject>::const_iterator iterEnd = end(mObjectGroups.getInteractiveGroup().getInteractiveObjects());

	for (; iter != end(mObjectGroups.getInteractiveGroup().getInteractiveObjects()); ++iter)
	{
		if (iter->getType() == "Barrel")
		{
			mSceneLayers[Update]->attachChild(std::move(std::unique_ptr<BarrelUpdateNode>(
				new BarrelUpdateNode(*iter, mTextures.get(Textures::ID::Tiles)))));

			mSceneLayers[Selection]->attachChild(std::move(std::unique_ptr<BarrelNode>(
				new BarrelNode(*iter, mWindow, mCamera.getView(), mUIBundle, mTextures, mSoundPlayer, mDaylightUI, mChatBox))));
		}
		else if (iter->getType() == "Door")
		{
			mSceneLayers[Update]->attachChild(std::move(std::unique_ptr<DoorUpdateNode>(
				new DoorUpdateNode(*iter, mTextures.get(Textures::ID::Tiles)))));

			mSceneLayers[Selection]->attachChild(std::move(std::unique_ptr<DoorNode>(
				new DoorNode(*iter, mWindow, mCamera.getView(), mUIBundle, mTextures, mSoundPlayer, mDaylightUI, mChatBox))));
		}
		else if (iter->getType() == "Window")
		{
			mSceneLayers[Update]->attachChild(std::move(std::unique_ptr<WindowUpdateNode>(
				new WindowUpdateNode(*iter, mTextures.get(Textures::ID::Tiles)))));

			mSceneLayers[Selection]->attachChild(std::move(std::unique_ptr<WindowNode>(
				new WindowNode(*iter, mWindow, mCamera.getView(), mUIBundle, mTextures, mSoundPlayer, mDaylightUI, mChatBox))));
		}
		else if (iter->getType() == "Clinic")
		{
			mSceneLayers[Update]->attachChild(std::move(std::unique_ptr<ClinicUpdateNode>(
				new ClinicUpdateNode(*iter, mTextures.get(Textures::ID::Tiles)))));

			mSceneLayers[Selection]->attachChild(std::move(std::unique_ptr<ClinicNode>(
				new ClinicNode(*iter, mWindow, mCamera.getView(), mUIBundle, buildAttachedRects(*iter), mSoundPlayer, mDaylightUI
				, mChatBox))));
		}
		else if (iter->getType() == "House")
		{
			mSceneLayers[Update]->attachChild(std::move(std::unique_ptr<HouseUpdateNode>(new HouseUpdateNode(*iter))));

			mSceneLayers[Selection]->attachChild(std::move(std::unique_ptr<HouseNode>(
				new HouseNode(*iter, mWindow, mCamera.getView(), mUIBundle, buildAttachedRects(*iter), mSoundPlayer, mDaylightUI
				, mChatBox))));
		}
		else
		{
			// ALW - TODO - Uncomment when all object types have been handled.
			//assert(("ALW - Logic Error: The interactive object type is not handled!", false));
		}
	}

	// Add UIs
	mSceneLayers[UI]->attachChild(std::move(std::unique_ptr<BarrelUINode>(new BarrelUINode(mBarrelUI))));
	mSceneLayers[UI]->attachChild(std::move(std::unique_ptr<DoorUINode>(new DoorUINode(mDoorUI))));
	mSceneLayers[UI]->attachChild(std::move(std::unique_ptr<WindowUINode>(new WindowUINode(mWindowUI))));
	mSceneLayers[UI]->attachChild(std::move(std::unique_ptr<ClinicUINode>(new ClinicUINode(mClinicUI))));
	mSceneLayers[UI]->attachChild(std::move(std::unique_ptr<HouseUINode>(new HouseUINode(mHouseUI))));

	// Add player's character
	std::unique_ptr<HeroNode> player(new HeroNode(mWorldBounds, mCamera.getView()));
	mHero = player.get();
	mHero->setPosition(mSpawnPosition);
	mSceneLayers[Camera]->attachChild(std::move(player));
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
