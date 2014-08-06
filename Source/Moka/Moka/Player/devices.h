#ifndef DEVICES_H
#define DEVICES_H


class Devices
{
public:
				Devices(const Devices &) = delete;
	Devices &	operator=(const Devices &) = delete;

	static bool isKeyboardEnabled() {return KeyboardEnabled;}
	static bool isMouseEnabled()    {return MouseEnabled;}
	static bool isJoystickEnabled() {return JoystickEnabled;}


private:
	static bool KeyboardEnabled;
	static bool MouseEnabled;
	static bool JoystickEnabled;
};

#endif
