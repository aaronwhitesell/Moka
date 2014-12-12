#ifndef CLINIC_SPRITE_NODE_H
#define CLINIC_SPRITE_NODE_H

#include "Trambo/Events/eventHandler.h"
#include "Trambo/SceneNodes/sceneNode.h"

#include <SFML/Graphics/Sprite.hpp>


namespace sf
{
	class RenderStates;
	class RenderTarget;
	class Texture;
}

namespace trambo
{
	class Event;
}

class InteractiveObject;

class ClinicSpriteNode : public trmb::SceneNode, trmb::EventHandler
{
// ALW - The motivation for this class is to allow the updatable sprite to be attached
// ALW - to the Sprite layer. This draws the sprite below the selection highlight.
public:
								ClinicSpriteNode(const InteractiveObject &interactiveObject, const sf::Texture &texture);
								ClinicSpriteNode(const ClinicSpriteNode &) = delete;
	ClinicSpriteNode &			operator=(const ClinicSpriteNode &) = delete;

	virtual void				handleEvent(const trmb::Event &gameEvent) override final;


private:
	typedef unsigned long EventGuid;


private:
	virtual void				drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override;


private:
	const EventGuid				mDrawSmallRDTCrateSprite;  // ALW - Matches the GUID in the ClinicNode class.
	const EventGuid				mDrawLargeRDTCrateSprite; // ALW - Matches the GUID in the ClinicNode class.
	const EventGuid				mDrawRDTBarrelSprite;      // ALW - Matches the GUID in the ClinicNode class.
	const EventGuid				mDoNotRDTDrawSprite;       // ALW - Matches the GUID in the ClinicNode class.
	const EventGuid				mDrawSmallACTCrateSprite;  // ALW - Matches the GUID in the ClinicNode class.
	const EventGuid				mDrawLargeACTCrateSprite; // ALW - Matches the GUID in the ClinicNode class.
	const EventGuid				mDrawACTBarrelSprite;      // ALW - Matches the GUID in the ClinicNode class.
	const EventGuid				mDoNotACTDrawSprite;       // ALW - Matches the GUID in the ClinicNode class.	
	const InteractiveObject		&mInteractiveObject;
	sf::Sprite					mRDTSprite;
	sf::Sprite					mACTSprite;
	bool						mDrawRDT;
	bool						mDrawACT;
};

#endif
