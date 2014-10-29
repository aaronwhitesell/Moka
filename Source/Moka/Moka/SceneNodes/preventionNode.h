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
	class SoundPlayer;
}

class ChatBox;
class InteractiveObject;

class PreventionNode : public InteractiveNode
{
public:
								PreventionNode(sf::RenderWindow &window, const sf::View &view, const InteractiveObject &interactiveObject
									, trmb::SoundPlayer &soundPlayer, ChatBox &chatBox);
								PreventionNode(const PreventionNode &) = delete;
	PreventionNode &			operator=(const PreventionNode &) = delete;

	virtual void				handleEvent(const trmb::Event &gameEvent) override final;


protected:
	virtual void				activate() override final;


private:
	trmb::SoundPlayer			&mSoundPlayer;
	ChatBox						&mChatBox;
};

#endif
