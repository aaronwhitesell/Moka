#ifndef DOOR_NODE_H
#define DOOR_NODE_H

#include "PreventionNode.h"
#include "../HUD/undoUI.h"

#include "Trambo/Events/event.h"
#include "Trambo/Events/eventStr.h"
#include "Trambo/Resources/resourceHolder.h"

#include <SFML/Graphics/Sprite.hpp>

#include <deque>
#include <vector>


namespace sf
{
	class RenderStates;
	class RenderTarget;
	class RenderWindow;
	class Time;
	class View;
}

namespace trmb
{
	class SoundPlayer;
}

class ChatBoxUI;
class DaylightUI;
class InteractiveObject;
class UIBundle;

class DoorNode : public PreventionNode
{
private:
	using CallbackPair = UndoUI::CallbackPair;


public:
								DoorNode(const InteractiveObject &interactiveObject, const sf::RenderWindow &window
									, const sf::View &view, UIBundle &uiBundle, const trmb::TextureHolder &textures
									, trmb::SoundPlayer &soundPlayer, DaylightUI &daylightUI, ChatBoxUI &chatBoxUI);
								DoorNode(const DoorNode &) = delete;
	DoorNode &					operator=(const DoorNode &) = delete;

	virtual void				handleEvent(const trmb::Event &gameEvent) override final;


private:
	virtual void				drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override final;
	virtual void				updateCurrent(sf::Time dt) override final;
	virtual void				activate() override final;
	void						updateUndoUI();

	void						closeDoor();
	void						openDoor();


private:
	const trmb::Event			mDoorUIActivated;
	const trmb::Event			mDrawDoorUI;
	const trmb::Event			mDoNotDrawDoorUI;
	const trmb::EventStr		mDrawDoorSprite;
	const trmb::EventStr		mDoNotDrawDoorSprite;
	const EventGuid				mLeftClickPress;   // ALW - Matches the GUID in the Controller class.
	const float					mCloseCost;
	const trmb::TextureHolder	&mTextures;
	trmb::SoundPlayer			&mSoundPlayer;
	DaylightUI					&mDaylightUI;
	ChatBoxUI					&mChatBoxUI;
	bool						mDoorUIActive;
	bool						mIsDoorClosed;

	// ALW - STL containers are used here, so the interface of the UndoUI class is the same for an instance of UndoUI with 
	// ALW - one or multiple UI elements.
	std::vector<CallbackPair>	mCallbackPairs;
	std::deque<bool>			mUIElemStates;		// ALW - Tracks which button is to be displayed and hidden.
};

#endif
