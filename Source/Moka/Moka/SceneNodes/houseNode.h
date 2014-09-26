#ifndef TRAMBO_HOUSE_NODE_H
#define TRAMBO_HOUSE_NODE_H

#include "interactiveNode.h"

#include <SFML/Graphics/Rect.hpp>

#include <vector>


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

class HouseNode : public InteractiveNode // inherits SceneNode and EventHandler indirectly
{
public:
								HouseNode(sf::RenderWindow &window, const sf::View &view
									, const PreventionObject &preventionObject, std::vector<sf::IntRect> attachedRects);
								HouseNode(const HouseNode &) = delete;
	HouseNode &					operator=(const HouseNode &) = delete;


protected:
	virtual void				updateSelection() final;


private:
	const sf::RenderWindow		&mWindow;
	const sf::View				&mView;
	const PreventionObject		&mPreventionObject;
	std::vector<sf::IntRect>	mAttachedRects;
};

#endif
