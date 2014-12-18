#ifndef WINDOW_UPDATE_NODE_H
#define WINDOW_UPDATE_NODE_H

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

class WindowUpdateNode : public trmb::SceneNode, trmb::EventHandler
{
// ALW - The motivation for this class is to allow the updatable sprite to be attached
// ALW - to the Sprite layer. This draws the sprite below the selection highlight.
public:
								WindowUpdateNode(const InteractiveObject &interactiveObject, const sf::Texture &texture);
								WindowUpdateNode(const WindowUpdateNode &) = delete;
	WindowUpdateNode &			operator=(const WindowUpdateNode &) = delete;

	virtual void				handleEvent(const trmb::Event &gameEvent) override final;


private:
	typedef unsigned long EventGuid;


private:
	virtual void				drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override;
	void						setSprite();
	void						setWindowScreenSprite();
	void						setWindowClosedSprite();


private:
	const EventGuid				mDrawWindowScreenSprite;      // ALW - Matches the GUID in the WindowNode class.
	const EventGuid				mDoNotDrawWindowScreenSprite; // ALW - Matches the GUID in the WindowNode class.
	const EventGuid				mDrawWindowClosedSprite;      // ALW - Matches the GUID in the WindowNode class.
	const EventGuid				mDoNotDrawWindowClosedSprite; // ALW - Matches the GUID in the WindowNode class.
	const InteractiveObject		&mInteractiveObject;
	sf::Sprite					mSprite;
	bool						mWindowScreen;
	bool						mWindowClosed;
	bool						mDrawn;
};

#endif
