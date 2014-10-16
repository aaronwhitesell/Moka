#ifndef TRAMBO_INTERACTIVE_NODE_H
#define TRAMBO_INTERACTIVE_NODE_H

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

class InteractiveNode : public trmb::SceneNode, trmb::EventHandler
{
private:
	typedef unsigned long EventGuid;


public:
						InteractiveNode();
						InteractiveNode(const InteractiveNode &) = delete;
	InteractiveNode &	operator=(const InteractiveNode &) = delete;

	virtual void		handleEvent(const trmb::Event &gameEvent);


protected:
	virtual void		drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;
	virtual void		updateSelection() = 0;


private:
	bool				isLeftClick(const trmb::Event& gameEvent) const;
	bool				isRightClick(const trmb::Event& gameEvent) const;


protected:
	sf::RectangleShape	mHightlight;
	bool				mIsSelected;
	bool				mDisableInput;


private:
	const EventGuid		mRightClick;	   // ALW - Matches the GUID in the Controller class.
	const EventGuid		mLeftClick;		   // ALW - Matches the GUID in the Controller class.
	const EventGuid		mCreateTextPrompt; // ALW - Matches the GUID in the ChatBox class.
	const EventGuid		mClearTextPrompt;  // ALW - Matches the GUID in the ChatBox class.
};

#endif
