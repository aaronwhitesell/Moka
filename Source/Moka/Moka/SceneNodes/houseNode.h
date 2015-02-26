#ifndef HOUSE_NODE_H
#define HOUSE_NODE_H

#include "buildingNode.h"
#include "../HUD/houseTrackerUI.h"

#include "Trambo/Events/event.h"
#include "Trambo/Events/eventStr.h"
#include "Trambo/Resources/resourceHolder.h"

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

class HouseNode : public BuildingNode // inherits SceneNode, EventHandler, and InteractiveNode indirectly
{
public:
								HouseNode(const InteractiveObject &interactiveObject, const sf::RenderWindow &window
									, const sf::View &view, UIBundle &uiBundle, std::vector<sf::FloatRect> attachedRects
									, trmb::FontHolder &fonts, trmb::SoundPlayer &soundPlayer);
								HouseNode(const HouseNode &) = delete;
	HouseNode &					operator=(const HouseNode &) = delete;

	int							getTotalMintNets() const;
	int							getTotalDamagedNets() const;
	virtual sf::FloatRect		getBoundingRect() const override;

	void						addInfectedMosquito();
	void						subtractInfectedMosquito();

	void						addMosquitoTotal();
	void						subtractMosquitoTotal();

	virtual void				handleEvent(const trmb::Event &gameEvent) override final;


private:
	enum NetCount
	{
		NoNets    = 0,
		OneNet    = 1,
		TwoNets   = 2,
		ThreeNets = 3
	};


private:
	virtual void				drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override final;
	virtual void				updateCurrent(sf::Time dt) override final;
	virtual void				activate() override final;
	void						updateOptionsUI();

	void						incrementPurchaseBedNet();
	void						decrementPurchaseBedNet();
	void						incrementRepair();
	void						decrementRepair();

	void						calculateNetPurchaseEvent();
	void						calculateNetRefundEvent();
	void						calculateRepairPurchaseEvent();
	void						calculateRepairRefundEvent();
	void						updateNetDisableState();
	void						updateRepairDisableState();


private:
	const trmb::Event			mHouseUIActivated;
	const trmb::EventStr		mAddNet1;
	const trmb::EventStr		mAddNet2;
	const trmb::EventStr		mAddNet3;
	const trmb::EventStr		mRemoveNet1;
	const trmb::EventStr		mRemoveNet2;
	const trmb::EventStr		mRemoveNet3;
	const trmb::EventStr		mRepairNet1;
	const trmb::EventStr		mRepairNet2;
	const trmb::EventStr		mRepairNet3;
	const trmb::EventStr		mUnrepairNet1;
	const trmb::EventStr		mUnrepairNet2;
	const trmb::EventStr		mUnrepairNet3;
	const trmb::Event			mDrawHouseUI;
	const trmb::Event			mDoNotDrawHouseUI;
	const EventGuid				mLeftClickPress;   // ALW - Matches the GUID in the Controller class.
	const float					mNetCost;
	const float					mRepairCost;
	trmb::SoundPlayer			&mSoundPlayer;
	HouseTrackerUI				mHouseTrackerUI;
	DaylightUI					&mDaylightUI;
	ChatBoxUI					&mChatBoxUI;
	bool						mHouseUIActive;

	const int					mTotalBeds;
	const int					mTotalOldNets;
	int							mNewNetCount;
	int							mRepairCount;
};

#endif
