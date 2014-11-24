#ifndef PREVENTION_NODE_H
#define PREVENTION_NODE_H

#include "interactiveNode.h"


namespace sf
{
	class RenderWindow;
	class View;
}

class InteractiveObject;
class UIBundle;

class PreventionNode : public InteractiveNode
{
public:
								PreventionNode(const InteractiveObject &interactiveObject, const sf::RenderWindow &window
									, const sf::View &view, UIBundle &uiBundle);
								PreventionNode(const PreventionNode &) = delete;
	PreventionNode &			operator=(const PreventionNode &) = delete;


private:
	virtual void				activate() = 0;


protected:
	virtual bool				isMouseOverObject() const override final;
};

#endif
