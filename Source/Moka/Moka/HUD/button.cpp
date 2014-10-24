#include "button.h"
#include "../Resources/resourceIdentifiers.h"

#include "Trambo/Events/event.h"
#include "Trambo/Utilities/utility.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Mouse.hpp>


Button::Button(trmb::FontHolder &fonts, sf::Vector2f size)
: mLeftClickPress(0x6955d309)
, mLeftClickRelease(0x128b8b25)
, mFonts(fonts)
, mBackgroundColor(sf::Color(0u, 51u, 102u, 255u))
, mTextColor(sf::Color(255u, 255u, 255u, 255u))
, mOutlineColor(sf::Color(0u, 0u, 0u, 255u))
, mHoverActive(true)
, mHoverBackgroundColor(sf::Color(0u, 76u, 153u, 255u))
, mHoverTextColor(sf::Color(255u, 255u, 255u, 255u))
, mHoverOutlineColor(sf::Color(0u, 0u, 0u, 255u))
, mDepressBackgroundColor(sf::Color(0u, 25u, 51u, 255u))
, mDepressTextColor(sf::Color(255u, 255u, 255u, 255u))
, mDepressOutlineColor(sf::Color(0u, 0u, 0u, 255u))
, mDisableActive(false)
, mDisableBackgroundColor(sf::Color(160u, 160u, 160u, 255u))
, mDisableTextColor(sf::Color(224u, 224u, 224u, 255u))
, mDisableOutlineColor(sf::Color(128u, 128u, 128u, 255u))
, mMouseCursorOver(false)
, mButtonPressed(false)
{
	setSize(size);
	setFont(Fonts::ID::Main);

	mButton.setFillColor(mBackgroundColor);
	mButton.setOutlineThickness(1.0f);
	mButton.setOutlineColor(mOutlineColor);
}

void Button::setSize(sf::Vector2f size)
{
	mSize = size;
	mButton.setSize(mSize);
	resizeFont();
}

void Button::setFont(Fonts::ID fontID)
{
	mText.setFont(mFonts.get(fontID));
	resizeFont();
}

void Button::setString(std::string string)
{
	mText.setString(string);
	resizeFont();
}

void Button::setBackgroundColor(const sf::Color &color)
{
	mBackgroundColor = color;
	mButton.setFillColor(color);
}

void Button::setTextColor(const sf::Color &color)
{
	mTextColor = color;
	mText.setColor(color);
}

void Button::setOutlineThickness(float thickness)
{
	mButton.setOutlineThickness(thickness);
}

void Button::setOutlineColor(const sf::Color &color)
{
	mOutlineColor = color;
	mButton.setOutlineColor(color);
}

void Button::setHoverAction(bool active)
{
	mHoverActive = active;
}

void Button::setHoverBackgroundColor(const sf::Color &color)
{
	mHoverBackgroundColor = color;
}

void Button::setHoverTextColor(const sf::Color &color)
{
	mHoverTextColor = color;
}

void Button::setHoverOutlineColor(const sf::Color &color)
{
	mHoverOutlineColor = color;
}

void Button::setDepressBackgroundColor(const sf::Color &color)
{
	mDepressBackgroundColor = color;
}

void Button::setDepressTextColor(const sf::Color &color)
{
	mDepressTextColor = color;
}

void Button::setDepressOutlineColor(const sf::Color &color)
{
	mDisableOutlineColor = color;
}

void Button::setDisableAction(bool active)
{
	mDisableActive = active;
}

void Button::setDisableBackgroundColor(const sf::Color &color)
{
	mDisableBackgroundColor = color;
}

void Button::setDisableTextColor(const sf::Color &color)
{
	mDisableTextColor = color;
}

void Button::setDisableOutlineColor(const sf::Color &color)
{
	mDisableOutlineColor = color;
}

void Button::setCallback(Callback callback)
{
	mCallback = std::move(callback);
}

void Button::handler(const sf::RenderWindow &window, const sf::View &view)
{
	const sf::Vector2i relativeToWindow = sf::Mouse::getPosition(window);
	const sf::Vector2f relativeToWorld = window.mapPixelToCoords(relativeToWindow, view);
	const sf::Vector2f mousePosition = relativeToWorld;
	const sf::FloatRect buttonRect(getPosition().x, getPosition().y, mSize.x, mSize.y);
	mMouseCursorOver = false;

	if (mDisableActive)
	{
		mText.setColor(mDisableTextColor);
		mButton.setFillColor(mDisableBackgroundColor);
		mButton.setOutlineColor(mDisableOutlineColor);
	}
	else if (buttonRect.contains(mousePosition))
	{
		if (mButtonPressed)
		{
			mText.setColor(mDepressTextColor);
			mButton.setFillColor(mDepressBackgroundColor);
			mButton.setOutlineColor(mDepressOutlineColor);
		}
		else if (mHoverActive)
		{
			mText.setColor(mHoverTextColor);
			mButton.setFillColor(mHoverBackgroundColor);
			mButton.setOutlineColor(mHoverOutlineColor);
		}

		mMouseCursorOver = true;
	}
	else
	{
		mText.setColor(mTextColor);
		mButton.setFillColor(mBackgroundColor);
		mButton.setOutlineColor(mOutlineColor);
	}
}

void Button::handleEvent(const trmb::Event &gameEvent)
{
	if (mLeftClickPress == gameEvent.getType() && mMouseCursorOver)
	{
		mButtonPressed = true;
	}
	else if (mButtonPressed && mLeftClickRelease == gameEvent.getType())
	{
		mButtonPressed = false;
		if (mMouseCursorOver && mCallback)
		{
			mCallback();
		}
	}
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(mButton, states);
	target.draw(mText, states);
}

void Button::resizeFont()
{
	// ALW - The text width is approximately 3/4 of the button width and the text
	// ALW - height is approximately the button height minus the vertical buffer.
	const float verticalBuffer = 10.0f;

	if (mSize.x > 0 && mSize.y > 0 && mText.getString() != "")
	{
		if (mText.getGlobalBounds().width < mSize.x / 4 * 3 && mText.getGlobalBounds().height < mSize.y - verticalBuffer)
		{
			while (mText.getGlobalBounds().width < mSize.x / 4 * 3 && mText.getLocalBounds().height < mSize.y - verticalBuffer)
			{
				mText.setCharacterSize(mText.getCharacterSize() + 1);
			}
			if (mText.getGlobalBounds().width < mSize.x / 4 * 3 || mText.getGlobalBounds().height < mSize.y - verticalBuffer)
			{
				mText.setCharacterSize(mText.getCharacterSize() - 1);
			}
		}
		else
		{
			while (mText.getGlobalBounds().width > mSize.x / 4 * 3 && mText.getGlobalBounds().height > mSize.y - verticalBuffer)
			{
				mText.setCharacterSize(mText.getCharacterSize() - 1);
			}
			if (mText.getGlobalBounds().width > mSize.x / 4 * 3 || mText.getGlobalBounds().height > mSize.y - verticalBuffer)
			{
				mText.setCharacterSize(mText.getCharacterSize() + 1);
			}
		}

		trmb::centerOrigin(mText);
		mText.setPosition(std::floor(mSize.x / 2.0f), std::floor(mSize.y / 2.0f));
	}
}
