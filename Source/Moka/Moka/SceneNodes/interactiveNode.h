#ifndef INTERACTIVE_NODE_H
#define INTERACTIVE_NODE_H

#include "Trambo/Events/eventHandler.h"
#include "Trambo/SceneNodes/sceneNode.h"

#include <SFML/Graphics/RectangleShape.hpp>


namespace sf
{
	class RenderStates;
	class RenderTarget;
	class RenderWindow;
	class View;
}

namespace trmb
{
	class Event;
}

class InteractiveObject;

class InteractiveNode : public trmb::SceneNode, trmb::EventHandler
{
private:
	typedef unsigned long EventGuid;


public:
								InteractiveNode(const sf::RenderWindow &window, const sf::View &view, const InteractiveObject &interactiveObject);
								InteractiveNode(const InteractiveNode &) = delete;
	InteractiveNode &			operator=(const InteractiveNode &) = delete;

	virtual void				handleEvent(const trmb::Event &gameEvent) override;


protected:
	virtual bool				isMouseOverObject() const;


private:
	virtual void				activate() = 0;
	virtual void				drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override;


protected:
	const sf::RenderWindow		&mWindow;
	const sf::View				&mView;
	const InteractiveObject		&mInteractiveObject;
	const EventGuid				mLeftClickPress;   // ALW - Matches the GUID in the Controller class.
	sf::RectangleShape			mHightlight;
	bool						mSelected;
	bool						mPreviousSelectedState;
	bool						mDisableInput;


private:
	const EventGuid				mCreateTextPrompt; // ALW - Matches the GUID in the ChatBox class.
	const EventGuid				mClearTextPrompt;  // ALW - Matches the GUID in the ChatBox class.
};

#endif
