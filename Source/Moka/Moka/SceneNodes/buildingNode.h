#ifndef BUILDING_NODE_H
#define BUILDING_NODE_H

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
	class SoundPlayer;
}

class ChatBox;
class InteractiveObject;
class OptionsUI;

class BuildingNode : public InteractiveNode // inherits SceneNode and EventHandler indirectly
{
public:
								BuildingNode(const sf::RenderWindow &window, const sf::View &view, const InteractiveObject &interactiveObject
									, std::vector<sf::FloatRect> attachedRects, OptionsUI &optionsUI);
								BuildingNode(const BuildingNode &) = delete;
	BuildingNode &				operator=(const BuildingNode &) = delete;

	virtual void				handleEvent(const trmb::Event &gameEvent) override final;


protected:
	OptionsUI				    &mOptionsUI;

private:
	virtual void				activate() = 0;
	virtual void				drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override final;
	virtual void				updateCurrent(sf::Time dt) override final;
	virtual bool				isMouseOverObject() const override final;
	bool						isMouseOverOptionsUI() const;


private:
	std::vector<sf::FloatRect>	mAttachedRects;
};

#endif
