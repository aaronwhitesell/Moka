#ifndef BUILDING_NODE_H
#define BUILDING_NODE_H

#include "interactiveNode.h"

#include <SFML/Graphics/Rect.hpp>

#include <vector>


namespace sf
{
	class RenderWindow;
	class View;
}

class InteractiveObject;
class UIBundle;

class BuildingNode : public InteractiveNode // inherits SceneNode and EventHandler indirectly
{
public:
								BuildingNode(const InteractiveObject &interactiveObject, const sf::RenderWindow &window
									, const sf::View &view, UIBundle &uiBundle, std::vector<sf::FloatRect> attachedRects);
								BuildingNode(const BuildingNode &) = delete;
	BuildingNode &				operator=(const BuildingNode &) = delete;


private:
	virtual void				activate() = 0;


protected:
	virtual bool				isMouseOverObject() const override final;


private:
	std::vector<sf::FloatRect>	mAttachedRects;
};

#endif
