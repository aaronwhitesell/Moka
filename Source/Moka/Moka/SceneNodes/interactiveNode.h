#ifndef INTERACTIVE_NODE_H
#define INTERACTIVE_NODE_H

#include "Trambo/Events/eventHandler.h"
#include "Trambo/SceneNodes/sceneNode.h"

#include <SFML/Graphics/RectangleShape.hpp>


namespace sf
{
	class RenderStates;
	class RenderTarget;
}

namespace trmb
{
	class Event;
}

class InteractiveObject;

class InteractiveNode : public trmb::SceneNode, trmb::EventHandler
{
protected:
	typedef unsigned long EventGuid;


public:
								InteractiveNode(const InteractiveObject &interactiveObject);
								InteractiveNode(const InteractiveNode &) = delete;
	InteractiveNode &			operator=(const InteractiveNode &) = delete;

	virtual void				handleEvent(const trmb::Event &gameEvent) override;


private:
	virtual void				drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override;
	virtual bool				isMouseOverObject() const = 0;
	virtual void				activate() = 0;


protected:
	const InteractiveObject		&mInteractiveObject;
	sf::RectangleShape			mHightlight;
	bool						mSelected;
	bool						mPreviousSelectedState;
	bool						mDisableInput;


private:
	const EventGuid				mCreateTextPrompt; // ALW - Matches the GUID in the ChatBox class.
	const EventGuid				mClearTextPrompt;  // ALW - Matches the GUID in the ChatBox class.
};

#endif
