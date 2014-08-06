#ifndef SETTINGS_STATE_H
#define SETTINGS_STATE_H

#include "../Player/controller.h"

#include "Trambo/GUI/button.h"
#include "Trambo/GUI/container.h"
#include "Trambo/GUI/label.h"
#include "Trambo/States/state.h"

#include <SFML/Graphics/Sprite.hpp>

#include <array>


namespace sf
{
	class Event;
	class Time;
}

namespace trmb
{
	class StateStack;
}

class SettingsState : public trmb::State
{
public:
							SettingsState(trmb::StateStack& stack, trmb::State::Context context);
							SettingsState(const SettingsState &) = delete;
	SettingsState &			operator=(const SettingsState &) = delete;

	virtual void			draw();
	virtual bool			update(sf::Time dt);
	virtual bool			handleEvent(const sf::Event& event);


private:
	void					updateLabels();
	void					addButtonAndLabel(Controller::Action action, float y, const std::string& text, trmb::State::Context context);


private:
	sf::Sprite												mBackgroundSprite;
	trmb::Container											mGUIContainer;
	std::array<trmb::Button::Ptr, Controller::ActionCount>	mBindingButtons;
	std::array<trmb::Label::Ptr, Controller::ActionCount>	mBindingLabels;
};

#endif
