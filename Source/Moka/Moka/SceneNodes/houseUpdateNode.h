#ifndef HOUSE_UPDATE_NODE_H
#define HOUSE_UPDATE_NODE_H

#include "Trambo/Events/eventHandler.h"
#include "Trambo/SceneNodes/sceneNode.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>


namespace sf
{
	class RenderStates;
	class RenderTarget;
}

namespace trambo
{
	class Event;
}

class InteractiveObject;

class HouseUpdateNode : public trmb::SceneNode, trmb::EventHandler
{
// ALW - The motivation for this class is to allow the updatable sprite to be attached
// ALW - to the Sprite layer. This draws the sprite below the selection highlight.
public:
								HouseUpdateNode(const InteractiveObject &interactiveObject);
								HouseUpdateNode(const HouseUpdateNode &) = delete;
	HouseUpdateNode &			operator=(const HouseUpdateNode &) = delete;

	virtual void				handleEvent(const trmb::Event &gameEvent) override final;


private:
	enum BedCount
	{
		OneBed    = 1,
		TwoBeds   = 2,
		ThreeBeds = 3
	};

private:
	typedef unsigned long EventGuid;


private:
	virtual void				drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override;


private:
	const InteractiveObject		&mInteractiveObject;

	const EventGuid				mAddNet1;        // ALW - Matches the GUID in the HouseNode class.
	const EventGuid				mAddNet2;        // ALW - Matches the GUID in the HouseNode class.
	const EventGuid				mAddNet3;	     // ALW - Matches the GUID in the HouseNode class.
	const EventGuid				mRemoveNet1;     // ALW - Matches the GUID in the HouseNode class.
	const EventGuid				mRemoveNet2;     // ALW - Matches the GUID in the HouseNode class.
	const EventGuid				mRemoveNet3;     // ALW - Matches the GUID in the HouseNode class.
	const EventGuid				mRepairNet1;     // ALW - Matches the GUID in the HouseNode class.
	const EventGuid				mRepairNet2;     // ALW - Matches the GUID in the HouseNode class.
	const EventGuid				mRepairNet3;     // ALW - Matches the GUID in the HouseNode class.
	const EventGuid				mUnrepairNet1;   // ALW - Matches the GUID in the HouseNode class.
	const EventGuid				mUnrepairNet2;   // ALW - Matches the GUID in the HouseNode class.
	const EventGuid				mUnrepairNet3;   // ALW - Matches the GUID in the HouseNode class.

	sf::RectangleShape			mBed1;
	sf::RectangleShape			mBed2;
	sf::RectangleShape			mBed3;

	sf::RectangleShape			mSheet1;
	sf::RectangleShape			mSheet2;
	sf::RectangleShape			mSheet3;

	sf::RectangleShape			mNet1;
	sf::RectangleShape			mNet2;
	sf::RectangleShape			mNet3;

	bool						mDrawBed1;
	bool						mDrawBed2;
	bool						mDrawBed3;

	bool						mDrawNet1;
	bool						mDrawNet2;
	bool						mDrawNet3;
};

#endif
