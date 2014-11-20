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
								BuildingNode(const InteractiveObject &interactiveObject, const sf::RenderWindow &window
									, const sf::View &view, OptionsUI &optionsUI, std::vector<sf::FloatRect> attachedRects);
								BuildingNode(const BuildingNode &) = delete;
	BuildingNode &				operator=(const BuildingNode &) = delete;

	virtual void				handleEvent(const trmb::Event &gameEvent) override final;


private:
	virtual void				drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override final;
	virtual void				updateCurrent(sf::Time dt) override final;
	virtual void				activate() = 0;
	virtual bool				isMouseOverObject() const override final;
	bool						isMouseOverOptionsUI() const;


protected:
	OptionsUI				    &mOptionsUI;


private:
	const EventGuid				mLeftClickPress;   // ALW - Matches the GUID in the Controller class.
	const sf::RenderWindow		&mWindow;
	const sf::View				&mView;
	std::vector<sf::FloatRect>	mAttachedRects;
};

#endif
