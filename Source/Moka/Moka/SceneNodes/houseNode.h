#ifndef HOUSE_NODE_H
#define HOUSE_NODE_H

#include "buildingNode.h"

#include <SFML/Graphics/Rect.hpp>

#include <vector>


namespace sf
{
	class RenderWindow;
	class View;
}

namespace trmb
{
	class SoundPlayer;
}

class ChatBox;
class InteractiveObject;
class OptionsUI;

class HouseNode : public BuildingNode // inherits SceneNode, EventHandler, and InteractiveNode indirectly
{
public:
								HouseNode(const sf::RenderWindow &window, const sf::View &view, const InteractiveObject &interactiveObject
									, std::vector<sf::FloatRect> attachedRects, trmb::SoundPlayer &soundPlayer, ChatBox &chatBox
									, OptionsUI &optionsUI);
								HouseNode(const HouseNode &) = delete;
	HouseNode &					operator=(const HouseNode &) = delete;


private:
	virtual void				activate() override final;
	void						updateOptionsUI();

	void						incrementPurchaseBedNetClick();
	void						decrementPurchaseBedNetClick();
	void						incrementRepairClick();
	void						decrementRepairClick();


private:
	trmb::SoundPlayer			&mSoundPlayer;
	ChatBox						&mChatBox;
};

#endif
