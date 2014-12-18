#ifndef DOOR_UPDATE_NODE_H
#define DOOR_UPDATE_NODE_H

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

class DoorUpdateNode : public trmb::SceneNode, trmb::EventHandler
{
// ALW - The motivation for this class is to allow the updatable sprite to be attached
// ALW - to the Sprite layer. This draws the sprite below the selection highlight.
public:
								DoorUpdateNode(const InteractiveObject &interactiveObject, const sf::Texture &texture);
								DoorUpdateNode(const DoorUpdateNode &) = delete;
	DoorUpdateNode &			operator=(const DoorUpdateNode &) = delete;

	virtual void				handleEvent(const trmb::Event &gameEvent) override final;


private:
	typedef unsigned long EventGuid;


private:
	virtual void				drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override;


private:
	const EventGuid				mDrawSprite;      // ALW - Matches the GUID in the DoorNode class.
	const EventGuid				mDoNotDrawSprite; // ALW - Matches the GUID in the DoorNode class.
	const InteractiveObject		&mInteractiveObject;
	sf::Sprite					mSprite;
	bool						mDrawn;
};

#endif
