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

	const trmb::ActionBinding::EventGuid	getUp() const;
	const trmb::ActionBinding::EventGuid	getDown() const;
	const trmb::ActionBinding::EventGuid	getLeft() const;
	const trmb::ActionBinding::EventGuid	getRight() const;
	const trmb::ActionBinding::EventGuid	getJump() const;
	const trmb::ActionBinding::EventGuid	getShoot() const;

	const std::vector<trmb::ActionBinding::ActionSharedPtr> &	getActions() const;

	sf::Keyboard::Key       getInputFromKeyboardKeyAsButtonBinding(trmb::ActionBinding::EventGuid eventGuid) const;
	sf::Mouse::Button       getInputFromMouseButtonAsButtonBinding(trmb::ActionBinding::EventGuid eventGuid) const;

	void					update();
	void					handleEvent(const sf::Event &inputEvent);

	void					assignKeyboardKeyAsButtonBinding(const trmb::KeyboardKeyAsButton &keyboardKeyAsButton, trmb::ActionBinding::EventGuid eventGuid);
	void					assignMouseButtonAsButtonBinding(const trmb::MouseButtonAsButton &MouseButtonAsButton, trmb::ActionBinding::EventGuid eventGuid);


private:
	const trmb::ActionBinding::EventGuid	mUp    = 0x84b05719;
	const trmb::ActionBinding::EventGuid	mDown  = 0xa95ea771;
	const trmb::ActionBinding::EventGuid	mLeft  = 0x84e6c13c;
	const trmb::ActionBinding::EventGuid	mRight = 0x19e343e8;
	const trmb::ActionBinding::EventGuid	mJump  = 0x6955d309;
	const trmb::ActionBinding::EventGuid	mShoot = 0x3e6524cd;


private:
	trmb::ActionBinding						mActionBindings;
};

#endif
