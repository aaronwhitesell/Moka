#ifndef WORLD_H
#define WORLD_H

#include "../HUD/chatBoxUI.h"
#include "../HUD/daylightUI.h"
#include "../HUD/optionsUI.h"
#include "../HUD/uiBundle.h"
#include "../HUD/undoUI.h"
#include "../SceneNodes/heroNode.h"
#include "../GameObjects/objectGroups.h"

#include "Trambo/Camera/camera.h"
#include "Trambo/Events/eventHandler.h"
#include "Trambo/Resources/resourceHolder.h"
#include "Trambo/SceneNodes/sceneNode.h"
#include "Trambo/Tiles/map.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>

#include <SFML/Graphics/Rect.hpp>

#include <array>
#include <vector>


namespace sf
{
	class RenderTarget;
	class RenderWindow;
}

namespace trmb
{
	class Event;
	class SoundPlayer;
}

class World : public trmb::EventHandler
{
public:
										World(sf::RenderWindow &window, trmb::FontHolder &fonts, trmb::SoundPlayer &soundPlayer);
										World(const World &) = delete;
	World &								operator=(const World &) = delete;

	void								update(sf::Time dt);
	virtual void						handleEvent(const trmb::Event &gameEvent);
	void								draw();


private:
	typedef unsigned long EventGuid;


private:
	void								updateSoundPlayer();
	void								configureUIs();
	void								buildScene();
	std::vector<sf::FloatRect>			buildAttachedRects(const InteractiveObject &interactiveObj);


private:
	enum Layer
	{
		Background,
		Update,
		Selection,
		UI,
		Camera,
		LayerCount
	};


private:
	const EventGuid								mFullscreen; // ALW - Matches the GUID in the ToggleFullscreen class.
	const EventGuid								mWindowed;   // ALW - Matches the GUID in the ToggleFullscreen class.

	const sf::RenderWindow						&mWindow;
	sf::RenderTarget							&mTarget;
	trmb::TextureHolder							mTextures;
	trmb::FontHolder							&mFonts;
	trmb::SoundPlayer							&mSoundPlayer;
	
	trmb::SceneNode								mSceneGraph;
	std::array<trmb::SceneNode *, LayerCount>	mSceneLayers;

	sf::FloatRect								mWorldBounds;
	sf::Vector2f								mSpawnPosition;

	trmb::Camera								mCamera;
	trmb::Map									mMap;
	UIBundle									mUIBundle;
	ChatBoxUI									mChatBoxUI;
	DaylightUI                                  mDaylightUI;
	UndoUI										mBarrelUI;
	UndoUI										mDoorUI;
	UndoUI										mWindowUI;
	OptionsUI									mClinicUI;
	OptionsUI									mHouseUI;
	ObjectGroups								mObjectGroups;
	HeroNode									*mHero;
};

#endif
