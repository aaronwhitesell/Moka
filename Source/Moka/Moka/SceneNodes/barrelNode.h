#ifndef BARREL_NODE_H
#define BARREL_NODE_H

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

class BarrelNode : public PreventionNode
{
private:
	using CallbackPair = UndoUI::CallbackPair;


public:
								BarrelNode(const InteractiveObject &interactiveObject, const sf::RenderWindow &window
									, const sf::View &view, UIBundle &uiBundle, const trmb::TextureHolder &textures
									, trmb::SoundPlayer &soundPlayer, DaylightUI &daylightUI, ChatBoxUI &chatBoxUI);
								BarrelNode(const BarrelNode &) = delete;
	BarrelNode &				operator=(const BarrelNode &) = delete;

	virtual void				handleEvent(const trmb::Event &gameEvent) override final;


private:
	virtual void				drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override final;
	virtual void				updateCurrent(sf::Time dt) override final;
	virtual void				activate() override final;
	void						updateUndoUI();

	void						addCover();
	void						undoCover();


private:
	const trmb::Event			mBarrelUIActivated;
	const trmb::Event			mDrawBarrelUI;
	const trmb::Event			mDoNotDrawBarrelUI;
	const trmb::EventStr		mDrawBarrelSprite;
	const trmb::EventStr		mDoNotDrawBarrelSprite;
	const EventGuid				mLeftClickPress;   // ALW - Matches the GUID in the Controller class.
	const float					mCoverCost;
	const trmb::TextureHolder	&mTextures;
	trmb::SoundPlayer			&mSoundPlayer;
	DaylightUI					&mDaylightUI;
	ChatBoxUI					&mChatBoxUI;
	bool						mBarrelUIActive;
	bool						mIsBarrelCovered;

	// ALW - STL containers are used here, so the interface of the UndoUI class is the same for an instance of UndoUI with 
	// ALW - one or multiple UI elements.
	std::vector<CallbackPair>	mCallbackPairs;
	std::deque<bool>			mUIElemStates;		// ALW - Tracks which button is to be displayed and hidden.
};

#endif
