#ifndef CLINIC_NODE_H
#define CLINIC_NODE_H

#include "buildingNode.h"

#include "Trambo/Events/event.h"
#include "Trambo/Events/eventStr.h"

#include <SFML/Graphics/Rect.hpp>

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

class ClinicNode : public BuildingNode // inherits SceneNode, EventHandler, and InteractiveNode indirectly
{
public:
								ClinicNode(const InteractiveObject &interactiveObject, const sf::RenderWindow &window
									, const sf::View &view, UIBundle &mUIBundle, std::vector<sf::FloatRect> attachedRects
									, trmb::SoundPlayer &soundPlayer, DaylightUI &daylightUI, ChatBoxUI &chatBoxUI);
								ClinicNode(const ClinicNode &) = delete;
	ClinicNode &				operator=(const ClinicNode &) = delete;

	int							getTotalRDTs() const;
	int							getTotalACTs() const;

	virtual void				handleEvent(const trmb::Event &gameEvent) override final;


private:
	enum RDTSupplies
	{
		NoRDTs        = 0,
		SmallRDTCrate = 1,
		LargeRDTCrate = 2,
		RDTBarrel     = 3,
		MinRDTCount   = 0,
		MaxRDTCount   = 3
	};

	enum ACTSupplies
	{
		NoACTs        = 0,
		SmallACTCrate = 1,
		LargeACTCrate = 2,
		ACTBarrel     = 3,
		MinACTCount   = 0,
		MaxACTCount   = 3
	};


private:
	virtual void				drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override final;
	virtual void				updateCurrent(sf::Time dt) override final;
	virtual void				activate() override final;
	void						updateOptionsUI();

	void						incrementPurchaseRDT();
	void						decrementPurchaseRDT();
	void						incrementPurchaseACT();
	void						decrementPurchaseACT();

	void						calculateRDTEvent();
	void						calculateACTEvent();
	void						updateRDTDisableState();
	void						updateACTDisableState();


private:
	const trmb::Event			mClinicUIActivated;
	const trmb::Event			mDrawClinicUI;
	const trmb::Event			mDoNotDrawClinicUI;
	const trmb::EventStr		mDrawSmallRDTCrateSprite;
	const trmb::EventStr		mDrawLargeRDTCrateSprite;
	const trmb::EventStr		mDrawRDTBarrelSprite;
	const trmb::EventStr		mDoNotRDTDrawSprite;
	const trmb::EventStr		mDrawSmallACTCrateSprite;
	const trmb::EventStr		mDrawLargeACTCrateSprite;
	const trmb::EventStr		mDrawACTBarrelSprite;
	const trmb::EventStr		mDoNotACTDrawSprite;
	const EventGuid				mLeftClickPress;   // ALW - Matches the GUID in the Controller class.
	const float					mRDTCost;
	const float					mACTCost;
	trmb::SoundPlayer			&mSoundPlayer;
	DaylightUI					&mDaylightUI;
	ChatBoxUI					&mChatBoxUI;
	bool						mClinicUIActive;
	int							mRDTCount;
	int							mACTCount;
};

#endif
