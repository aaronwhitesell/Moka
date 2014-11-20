#ifndef CLINIC_NODE_H
#define CLINIC_NODE_H

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

class ClinicNode : public BuildingNode // inherits SceneNode, EventHandler, and InteractiveNode indirectly
{
public:
									ClinicNode(const InteractiveObject &interactiveObject, const sf::RenderWindow &window
										, const sf::View &view, OptionsUI &optionsUI, std::vector<sf::FloatRect> attachedRects
										, trmb::SoundPlayer &soundPlayer, ChatBox &chatBox);
									ClinicNode(const ClinicNode &) = delete;
	ClinicNode &					operator=(const ClinicNode &) = delete;


private:
	virtual void				activate() override final;
	void						updateOptionsUI();

	void						incrementPurchaseRDTClick();
	void						decrementPurchaseRDTClick();
	void						incrementPurchaseACTClick();
	void						decrementPurchaseACTClick();


private:
	trmb::SoundPlayer			&mSoundPlayer;
	ChatBox						&mChatBox;
};

#endif