#ifndef CLINIC_NODE_H
#define CLINIC_NODE_H

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

class ClinicNode : public InteractiveNode // inherits SceneNode and EventHandler indirectly
{
public:
								ClinicNode(const sf::RenderWindow &window, const sf::View &view, const InteractiveObject &interactiveObject
									, std::vector<sf::FloatRect> attachedRects, trmb::SoundPlayer &soundPlayer, ChatBox &chatBox
									, OptionsUI &mClinicUI);
								ClinicNode(const ClinicNode &) = delete;
	ClinicNode &				operator=(const ClinicNode &) = delete;

	virtual void				handleEvent(const trmb::Event &gameEvent) override final;


protected:
	virtual bool				isMouseOverObject() const override final;


private:
	virtual void				drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override final;
	virtual void				updateCurrent(sf::Time dt) override final;
	virtual void				activate() override final;
	bool						isMouseOverClinicUI() const;
	void						updateClinicUI();

	void						incrementPurchaseClick();
	void						decrementPurchaseClick();
	void						incrementRepairClick();
	void						decrementRepairClick();


private:
	std::vector<sf::FloatRect>	mAttachedRects;
	trmb::SoundPlayer			&mSoundPlayer;
	ChatBox						&mChatBox;
	OptionsUI				    &mClinicUI;
};

#endif
