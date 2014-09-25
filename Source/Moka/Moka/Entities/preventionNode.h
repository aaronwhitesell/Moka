#ifndef TRAMBO_PREVENTION_NODE_H
#define TRAMBO_PREVENTION_NODE_H

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

class PreventionObject;

class PreventionNode : public trmb::SceneNode, trmb::EventHandler
{
public:
						PreventionNode(sf::RenderWindow &window, const sf::View &view, const PreventionObject &preventionObject);
						PreventionNode(const PreventionNode &) = delete;
	PreventionNode &	operator=(const PreventionNode &) = delete;

	virtual void		handleEvent(const trmb::Event& gameEvent) final;


private:
	typedef unsigned long EventGuid;


private:
	virtual void					drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;

	bool							isLeftClick(const trmb::Event& gameEvent) const;
	bool							isRightClick(const trmb::Event& gameEvent) const;
	void							updateSelection();


private:
	const EventGuid					mRightClick; // ALW - Matches the GUID in the Controller class.
	const EventGuid					mLeftClick;  // ALW - Matches the GUID in the Controller class.

	const sf::RenderWindow			&mWindow;
	const sf::View					&mView;
	const PreventionObject			&mPreventionObject;

	sf::RectangleShape				mHightlight;
	bool							mIsSelected;
};

#endif
