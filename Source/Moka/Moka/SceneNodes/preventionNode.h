#ifndef TRAMBO_PREVENTION_NODE_H
#define TRAMBO_PREVENTION_NODE_H

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
}

class InteractiveObject;

class PreventionNode : public InteractiveNode
{
public:
								PreventionNode(sf::RenderWindow &window, const sf::View &view, const InteractiveObject &interactiveObject);
								PreventionNode(const PreventionNode &) = delete;
	PreventionNode &			operator=(const PreventionNode &) = delete;


protected:
	virtual void				updateSelection() final;


private:
	const sf::RenderWindow		&mWindow;
	const sf::View				&mView;
	const InteractiveObject		&mInteractiveObject;
};

#endif
