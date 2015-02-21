#ifndef RESIDENT_UPDATE_NODE_H
#define RESIDENT_UPDATE_NODE_H

#include "Trambo/Events/eventHandler.h"
#include "Trambo/SceneNodes/sceneNode.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>


namespace sf
{
	class RenderStates;
	class RenderTarget;
}

namespace trambo
{
	class Event;
}

class HouseNode;

class ResidentUpdateNode : public trmb::SceneNode, trmb::EventHandler
{
// ALW - The motivation for this class is to allow the drawn image to be attached
// ALW - to the Update layer. This draws the image below the selection highlight.
public:
								ResidentUpdateNode(int residentID, const HouseNode * const houseNode);
								ResidentUpdateNode(const ResidentUpdateNode &) = delete;
	ResidentUpdateNode &		operator=(const ResidentUpdateNode &) = delete;

	virtual void				handleEvent(const trmb::Event &gameEvent) override final;


private:
	enum ResidentCount
	{
		Resident0 = 0,
		Resident1 = 1,
		Resident2 = 2,
		Resident3 = 3,
		Resident4 = 4,
		Resident5 = 5,
		Resident6 = 6,
	};


private:
	typedef unsigned long		EventGuid;


private:
	void						setUIPosition();
	virtual void				drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override;


private:
	const EventGuid				mCured0;		// ALW - Matches the GUID in the ResidentNode class.
	const EventGuid				mCured1;		// ALW - Matches the GUID in the ResidentNode class.
	const EventGuid				mCured2;		// ALW - Matches the GUID in the ResidentNode class.
	const EventGuid				mCured3;		// ALW - Matches the GUID in the ResidentNode class.
	const EventGuid				mCured4;		// ALW - Matches the GUID in the ResidentNode class.
	const EventGuid				mCured5;		// ALW - Matches the GUID in the ResidentNode class.
	const EventGuid				mCured6;		// ALW - Matches the GUID in the ResidentNode class.

	const EventGuid				mHasMalaria0;	// ALW - Matches the GUID in the ResidentNode class.
	const EventGuid				mHasMalaria1;	// ALW - Matches the GUID in the ResidentNode class.
	const EventGuid				mHasMalaria2;	// ALW - Matches the GUID in the ResidentNode class.
	const EventGuid				mHasMalaria3;	// ALW - Matches the GUID in the ResidentNode class.
	const EventGuid				mHasMalaria4;	// ALW - Matches the GUID in the ResidentNode class.
	const EventGuid				mHasMalaria5;	// ALW - Matches the GUID in the ResidentNode class.
	const EventGuid				mHasMalaria6;	// ALW - Matches the GUID in the ResidentNode class.

	const int					mResidentID;
	const HouseNode * const		mHouseNode;

	const sf::Color				mSkin;
	const sf::Color				mRed;
	sf::CircleShape				mUI;
};

#endif
