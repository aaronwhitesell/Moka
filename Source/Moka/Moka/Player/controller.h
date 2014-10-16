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
	typedef unsigned long EventGuid;


public:
	enum Action
	{
		MoveUp,
		MoveDown,
		MoveLeft,
		MoveRight,
		LeftClick,
		RightClick,
		ActionCount
	};


public:
							Controller();
							Controller(const Controller &) = delete;
	Controller &			operator=(const Controller &) = delete;

	const std::vector<trmb::ActionBinding::ActionSharedPtr> &	getActions() const;

	sf::Keyboard::Key       getInputFromKeyboardKeyAsButtonBinding(EventGuid eventGuid) const;
	sf::Mouse::Button       getInputFromMouseButtonAsButtonBinding(EventGuid eventGuid) const;

	void					update();
	void					handleEvent(const sf::Event &inputEvent);

	void					assignKeyboardKeyAsButtonBinding(const trmb::KeyboardKeyAsButton &keyboardKeyAsButton, EventGuid eventGuid);
	void					assignMouseButtonAsButtonBinding(const trmb::MouseButtonAsButton &MouseButtonAsButton, EventGuid eventGuid);


private:
	const EventGuid	mUp;
	const EventGuid	mDown;
	const EventGuid	mLeft;
	const EventGuid	mRight;
	const EventGuid mEnter;
	const EventGuid	mLeftClick;
	const EventGuid	mRightClick;


private:
	trmb::ActionBinding						mActionBindings;
};

#endif
