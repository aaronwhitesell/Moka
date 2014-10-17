#ifndef WORLD_H
#define WORLD_H

#include "../HUD/chatBox.h"
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
	void								buildScene();
	std::vector<sf::FloatRect>			buildAttachedRects(const InteractiveObject &interactiveObj);


private:
	enum Layer
	{
		Background,
		Middleground,
		Interactive,
		Foreground,
		LayerCount
	};


private:
	const EventGuid								mFullscreen; // ALW - Matches the GUID in the ToggleFullscreen class.
	const EventGuid								mWindowed;   // ALW - Matches the GUID in the ToggleFullscreen class.

	sf::RenderWindow							&mWindow;
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
	ChatBox										mChatBox;
	ObjectGroups								mObjectGroups;
	HeroNode									*mHero;
};

#endif
