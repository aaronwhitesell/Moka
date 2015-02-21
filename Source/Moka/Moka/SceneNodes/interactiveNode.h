#ifndef INTERACTIVE_NODE_H
#define INTERACTIVE_NODE_H

#include "Trambo/Events/eventHandler.h"
#include "Trambo/SceneNodes/sceneNode.h"

#include <SFML/Graphics/Rect.hpp>
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
class UIBundle;

class InteractiveNode : public trmb::SceneNode, trmb::EventHandler
{
protected:
	typedef unsigned long EventGuid;


public:
								InteractiveNode(const InteractiveObject &interactiveObject, const sf::RenderWindow &window
									, const sf::View &view, UIBundle &uiBundle);
								InteractiveNode(const InteractiveNode &) = delete;
	InteractiveNode &			operator=(const InteractiveNode &) = delete;

	sf::FloatRect				getCollisionBox() const;
	const InteractiveObject &   getInteractiveObject() const;

	virtual void				handleEvent(const trmb::Event &gameEvent) override;


private:
	virtual void				drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override;
	virtual bool				isMouseOverObject() const = 0;
	virtual void				activate() = 0;


protected:
	bool						isMouseOverUI(sf::FloatRect rect) const;
	void						sendEvent(const trmb::Event &gameEvent);


protected:
	const InteractiveObject		&mInteractiveObject;
	const sf::RenderWindow		&mWindow;
	const sf::View				&mView;
	UIBundle					&mUIBundle;
	sf::RectangleShape			mHightlight;
	bool						mSelected;
	bool						mPreviousSelectedState;
	bool                        mDisableBuildMode;
	bool						mDisableInput;


private:
	const EventGuid             mBeginSimulationEvent; // ALW - Matches the GUID in the DaylightUI class.
	const EventGuid				mCreateTextPrompt; // ALW - Matches the GUID in the ChatBoxUI class.
	const EventGuid				mClearTextPrompt;  // ALW - Matches the GUID in the ChatBoxUI class.
};

#endif
