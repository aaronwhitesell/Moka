#include "Controller.h"

#include <SFML/Window/Event.hpp>


Controller::Controller()
: mUp(0x84b05719)
, mDown(0xa95ea771)
, mLeft(0x84e6c13c)
, mRight(0x19e343e8)
, mEnter(0xff349d1d)
, mLeftClickPress(0x6955d309)
, mLeftClickRelease(0x128b8b25)
, mRightClick(0x3e6524cd)
{
	// ALW - Create a map of event guids and corresponding actions
	mActionBindings.createAction(mUp);
	mActionBindings.createAction(mDown);
	mActionBindings.createAction(mLeft);
	mActionBindings.createAction(mRight);
	mActionBindings.createAction(mEnter);
	mActionBindings.createAction(mLeftClickPress);
	mActionBindings.createAction(mLeftClickRelease);
	mActionBindings.createAction(mRightClick);

	// ALW - Create a map of keyboard buttons and corresponding actions (default bindings)
	mActionBindings.assignKeyboardKeyAsButtonBinding(trmb::KeyboardKeyAsButton(sf::Keyboard::W,      trmb::KeyboardKeyAsButton::ButtonType::RealTime),  mUp);
	mActionBindings.assignKeyboardKeyAsButtonBinding(trmb::KeyboardKeyAsButton(sf::Keyboard::S,      trmb::KeyboardKeyAsButton::ButtonType::RealTime),  mDown);
	mActionBindings.assignKeyboardKeyAsButtonBinding(trmb::KeyboardKeyAsButton(sf::Keyboard::A,      trmb::KeyboardKeyAsButton::ButtonType::RealTime),  mLeft);
	mActionBindings.assignKeyboardKeyAsButtonBinding(trmb::KeyboardKeyAsButton(sf::Keyboard::D,      trmb::KeyboardKeyAsButton::ButtonType::RealTime),  mRight);
	mActionBindings.assignKeyboardKeyAsButtonBinding(trmb::KeyboardKeyAsButton(sf::Keyboard::Return, trmb::KeyboardKeyAsButton::ButtonType::OnPress),   mEnter);
	mActionBindings.assignMouseButtonAsButtonBinding(trmb::MouseButtonAsButton(sf::Mouse::Left,		 trmb::MouseButtonAsButton::ButtonType::OnPress),   mLeftClickPress);
	mActionBindings.assignMouseButtonAsButtonBinding(trmb::MouseButtonAsButton(sf::Mouse::Left,		 trmb::MouseButtonAsButton::ButtonType::OnRelease), mLeftClickRelease);
	mActionBindings.assignMouseButtonAsButtonBinding(trmb::MouseButtonAsButton(sf::Mouse::Right,     trmb::MouseButtonAsButton::ButtonType::OnPress),   mRightClick);
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
