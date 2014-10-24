#ifndef BUTTON_H
#define BUTTON_H

#include "Trambo/Resources/resourceHolder.h"
#include "Trambo/Events/eventHandler.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Vector2.hpp>

#include <functional>


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
}

Fonts::ID;

class Button : public sf::Transformable, public sf::Drawable, public trmb::EventHandler
{
public:
	typedef std::function<void()> Callback;


public:
						  Button(trmb::FontHolder &fonts, sf::Vector2f size = sf::Vector2f(0, 0));
						  Button(const Button &) = delete;
	Button &			  operator=(const Button &) = delete;

	void				  setSize(sf::Vector2f size);
	void				  setFont(Fonts::ID fontID);
	void				  setString(std::string string);

	void				  setBackgroundColor(const sf::Color &color);
	void				  setTextColor(const sf::Color &color);
	void				  setOutlineThickness(float thickness);
	void				  setOutlineColor(const sf::Color &color);

	void				  setHoverAction(bool active = true);
	void				  setHoverBackgroundColor(const sf::Color &color);
	void				  setHoverTextColor(const sf::Color &color);
	void				  setHoverOutlineColor(const sf::Color &color);

	void				  setDepressBackgroundColor(const sf::Color &color);
	void				  setDepressTextColor(const sf::Color &color);
	void				  setDepressOutlineColor(const sf::Color &color);

	void                  setDisableAction(bool active = true);
	void				  setDisableBackgroundColor(const sf::Color &color);
	void				  setDisableTextColor(const sf::Color &color);
	void				  setDisableOutlineColor(const sf::Color &color);

	void				  setCallback(Callback callback);

	void				  handler(const sf::RenderWindow &window, const sf::View &view);
	virtual void		  handleEvent(const trmb::Event &gameEvent) override final;


private:
	typedef unsigned long EventGuid;


private:
	virtual void		  draw(sf::RenderTarget& target, sf::RenderStates states) const override final;
	void				  resizeFont();


private:
	const EventGuid		  mLeftClickPress;    // ALW - Matches the GUID in the Controller class.
	const EventGuid		  mLeftClickRelease;  // ALW - Matches the GUID in the Controller class.

	trmb::FontHolder	  &mFonts;
	sf::Vector2f		  mSize;

	sf::RectangleShape    mButton;
	sf::Text              mText;

	sf::Color			  mBackgroundColor;
	sf::Color			  mTextColor;
	sf::Color			  mOutlineColor;

	bool				  mHoverActive;
	sf::Color			  mHoverBackgroundColor;
	sf::Color			  mHoverTextColor;
	sf::Color			  mHoverOutlineColor;

	sf::Color			  mDepressBackgroundColor;
	sf::Color			  mDepressTextColor;
	sf::Color			  mDepressOutlineColor;

	bool				  mDisableActive;
	sf::Color			  mDisableBackgroundColor;
	sf::Color			  mDisableTextColor;
	sf::Color			  mDisableOutlineColor;

	Callback			  mCallback;

	bool				  mMouseCursorOver;
	bool				  mButtonPressed;
};

#endif
