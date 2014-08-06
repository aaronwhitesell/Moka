#include "Controller.h"

#include <SFML/Window/Event.hpp>


Controller::Controller()
{
	// ALW - Create a map of event guids and corresponding actions
	mActionBindings.createAction(mUp);
	mActionBindings.createAction(mDown);
	mActionBindings.createAction(mLeft);
	mActionBindings.createAction(mRight);
	mActionBindings.createAction(mJump);
	mActionBindings.createAction(mShoot);

	// ALW - Create a map of keyboard buttons and corresponding actions (default bindings)
	mActionBindings.assignKeyboardKeyAsButtonBinding(trmb::KeyboardKeyAsButton(sf::Keyboard::W,  trmb::KeyboardKeyAsButton::ButtonType::RealTime), mUp);
	mActionBindings.assignKeyboardKeyAsButtonBinding(trmb::KeyboardKeyAsButton(sf::Keyboard::S,  trmb::KeyboardKeyAsButton::ButtonType::RealTime), mDown);
	mActionBindings.assignKeyboardKeyAsButtonBinding(trmb::KeyboardKeyAsButton(sf::Keyboard::A,  trmb::KeyboardKeyAsButton::ButtonType::RealTime), mLeft);
	mActionBindings.assignKeyboardKeyAsButtonBinding(trmb::KeyboardKeyAsButton(sf::Keyboard::D,  trmb::KeyboardKeyAsButton::ButtonType::RealTime), mRight);
	mActionBindings.assignMouseButtonAsButtonBinding(trmb::MouseButtonAsButton(sf::Mouse::Right, trmb::MouseButtonAsButton::ButtonType::RealTime), mJump);
	mActionBindings.assignMouseButtonAsButtonBinding(trmb::MouseButtonAsButton(sf::Mouse::Left,	 trmb::MouseButtonAsButton::ButtonType::RealTime), mShoot);
}

const trmb::ActionBinding::EventGuid Controller::getUp() const
{
	return mUp;
}

const trmb::ActionBinding::EventGuid Controller::getDown() const
{
	return mDown;
}

const trmb::ActionBinding::EventGuid Controller::getLeft() const
{
	return mLeft;
}

const trmb::ActionBinding::EventGuid Controller::getRight() const
{
	return mRight;
}

const trmb::ActionBinding::EventGuid Controller::getJump() const
{
	return mJump;
}

const trmb::ActionBinding::EventGuid Controller::getShoot() const
{
	return mShoot;
}

const std::vector<trmb::ActionBinding::ActionSharedPtr> & Controller::getActions() const
{
	return mActionBindings.getActions();
}

sf::Keyboard::Key Controller::getInputFromKeyboardKeyAsButtonBinding(trmb::ActionBinding::EventGuid eventGuid) const
{
	return mActionBindings.getInputFromKeyboardKeyAsButtonBinding(eventGuid);
}

sf::Mouse::Button Controller::getInputFromMouseButtonAsButtonBinding(trmb::ActionBinding::EventGuid eventGuid) const
{
	return mActionBindings.getInputFromMouseButtonAsButtonBinding(eventGuid);
}

void Controller::update()
{
	mActionBindings.update();
}

void Controller::handleEvent(const sf::Event &inputEvent)
{
	mActionBindings.handleEvent(inputEvent);
}

void Controller::assignKeyboardKeyAsButtonBinding(const trmb::KeyboardKeyAsButton &keyboardKeyAsButton, trmb::ActionBinding::EventGuid eventGuid)
{
	mActionBindings.assignKeyboardKeyAsButtonBinding(keyboardKeyAsButton, eventGuid);
}

void Controller::assignMouseButtonAsButtonBinding(const trmb::MouseButtonAsButton &mouseButtonAsButton, trmb::ActionBinding::EventGuid eventGuid)
{
	mActionBindings.assignMouseButtonAsButtonBinding(mouseButtonAsButton, eventGuid);
}
