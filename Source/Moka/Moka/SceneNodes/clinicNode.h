#ifndef CLINIC_NODE_H
#define CLINIC_NODE_H

#include "buildingNode.h"

#include "Trambo/Events/event.h"

#include <SFML/Graphics/Rect.hpp>

#include <vector>


namespace sf
{
	class RenderStates;
	class RenderTarget;
	class RenderWindow;
	class Time;
	class View;
}

namespace trmb
{
	class Event;
	class SoundPlayer;
}

class ChatBox;
class InteractiveObject;
class UIBundle;

class ClinicNode : public BuildingNode // inherits SceneNode, EventHandler, and InteractiveNode indirectly
{
public:
								ClinicNode(const InteractiveObject &interactiveObject, const sf::RenderWindow &window
									, const sf::View &view, UIBundle &mUIBundle, std::vector<sf::FloatRect> attachedRects
									, trmb::SoundPlayer &soundPlayer, ChatBox &chatBox);
								ClinicNode(const ClinicNode &) = delete;
	ClinicNode &				operator=(const ClinicNode &) = delete;

	virtual void				handleEvent(const trmb::Event &gameEvent) override final;


private:
	virtual void				drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override final;
	virtual void				updateCurrent(sf::Time dt) override final;
	virtual void				activate() override final;
	void						updateOptionsUI();

	void						incrementPurchaseRDT();
	void						decrementPurchaseRDT();
	void						incrementPurchaseACT();
	void						decrementPurchaseACT();


private:
	const trmb::Event			mClinicUIActivated;
	const trmb::Event			mDrawClinicUI;
	const trmb::Event			mDoNotDrawClinicUI;
	const EventGuid				mLeftClickPress;   // ALW - Matches the GUID in the Controller class.
	trmb::SoundPlayer			&mSoundPlayer;
	ChatBox						&mChatBox;
	bool						mClinicUIActive;
};

#endif
