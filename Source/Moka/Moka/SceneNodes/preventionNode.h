#ifndef PREVENTION_NODE_H
#define PREVENTION_NODE_H

#include "interactiveNode.h"


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
	class SoundPlayer;
}

class ChatBox;
class InteractiveObject;
class UndoUI;

class PreventionNode : public InteractiveNode
{
public:
								PreventionNode(const InteractiveObject &interactiveObject, const sf::RenderWindow &window
									, const sf::View &view, UndoUI &undoUI);
								PreventionNode(const PreventionNode &) = delete;
	PreventionNode &			operator=(const PreventionNode &) = delete;

	virtual void				handleEvent(const trmb::Event &gameEvent) override final;


private:
	virtual void				drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override final;
	virtual void				updateCurrent(sf::Time dt) override final;
	virtual void				activate() = 0;
	virtual bool				isMouseOverObject() const override final;
	bool						isMouseOverUndoUI() const;


protected:
	UndoUI						&mUndoUI;


private:
	const EventGuid				mLeftClickPress;   // ALW - Matches the GUID in the Controller class.
	const sf::RenderWindow		&mWindow;
	const sf::View				&mView;
};

#endif
