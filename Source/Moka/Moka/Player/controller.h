#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Trambo/Inputs/actionBinding.h"
#include "Trambo/Inputs/keyboardKeyAsButton.h"
#include "Trambo/Inputs/mouseButtonAsButton.h"

#include <vector>


namespace sf
{
	class Event;
}

class Controller
{
public:
	enum Action
	{
		MoveUp,
		MoveDown,
		MoveLeft,
		MoveRight,
		Jump,
		Shoot,
		ActionCount
	};


public:
							Controller();
							Controller(const Controller &) = delete;
	Controller &			operator=(const Controller &) = delete;

	trmb::ActionBinding::EventGuid	getUp() const;
	trmb::ActionBinding::EventGuid	getDown() const;
	trmb::ActionBinding::EventGuid	getLeft() const;
	trmb::ActionBinding::EventGuid	getRight() const;
	trmb::ActionBinding::EventGuid	getJump() const;
	trmb::ActionBinding::EventGuid	getShoot() const;

	const std::vector<trmb::ActionBinding::ActionSharedPtr> &	getActions() const;

	sf::Keyboard::Key       getInputFromKeyboardKeyAsButtonBinding(trmb::ActionBinding::EventGuid eventGuid) const;
	sf::Mouse::Button       getInputFromMouseButtonAsButtonBinding(trmb::ActionBinding::EventGuid eventGuid) const;

	void					update();
	void					handleEvent(const sf::Event &inputEvent);

	void					assignKeyboardKeyAsButtonBinding(const trmb::KeyboardKeyAsButton &keyboardKeyAsButton, trmb::ActionBinding::EventGuid eventGuid);
	void					assignMouseButtonAsButtonBinding(const trmb::MouseButtonAsButton &MouseButtonAsButton, trmb::ActionBinding::EventGuid eventGuid);


private:
	const trmb::ActionBinding::EventGuid	mUp;
	const trmb::ActionBinding::EventGuid	mDown;
	const trmb::ActionBinding::EventGuid	mLeft;
	const trmb::ActionBinding::EventGuid	mRight;
	const trmb::ActionBinding::EventGuid	mJump;
	const trmb::ActionBinding::EventGuid	mShoot;


private:
	trmb::ActionBinding						mActionBindings;
};

#endif
