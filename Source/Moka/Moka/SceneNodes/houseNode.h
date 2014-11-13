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
class HouseUI;

class HouseNode : public InteractiveNode // inherits SceneNode and EventHandler indirectly
{
public:
								HouseNode(const sf::RenderWindow &window, const sf::View &view, const InteractiveObject &interactiveObject
									, std::vector<sf::FloatRect> attachedRects, trmb::SoundPlayer &soundPlayer, ChatBox &chatBox
									, HouseUI &mHouseUI);
								HouseNode(const HouseNode &) = delete;
	HouseNode &					operator=(const HouseNode &) = delete;

	virtual void				handleEvent(const trmb::Event &gameEvent) override final;


protected:
	virtual bool				isMouseOverObject() const override final;


private:
	virtual void				drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override final;
	virtual void				updateCurrent(sf::Time dt) override final;
	virtual void				activate() override final;
	bool						isMouseOverHouseUI() const;
	void						updateHouseUI();

	void						incrementPurchaseClick();
	void						decrementPurchaseClick();
	void						incrementRepairClick();
	void						decrementRepairClick();


private:
	std::vector<sf::FloatRect>	mAttachedRects;
	trmb::SoundPlayer			&mSoundPlayer;
	ChatBox						&mChatBox;
	HouseUI				        &mHouseUI;
};

#endif
