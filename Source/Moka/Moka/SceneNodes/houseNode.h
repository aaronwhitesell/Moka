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
	class SoundPlayer;
}

class ChatBox;
class InteractiveObject;

class HouseNode : public InteractiveNode // inherits SceneNode and EventHandler indirectly
{
public:
								HouseNode(sf::RenderWindow &window, const sf::View &view, const InteractiveObject &interactiveObject
									, std::vector<sf::FloatRect> attachedRects, trmb::SoundPlayer &soundPlayer, ChatBox &chatBox);
								HouseNode(const HouseNode &) = delete;
	HouseNode &					operator=(const HouseNode &) = delete;

	virtual void				handleEvent(const trmb::Event &gameEvent) override final;


protected:
	virtual bool				isMouseOverObject() const override final;
	virtual void				activate() override final;


private:
	std::vector<sf::FloatRect>	mAttachedRects;
	trmb::SoundPlayer			&mSoundPlayer;
	ChatBox						&mChatBox;
};

#endif
