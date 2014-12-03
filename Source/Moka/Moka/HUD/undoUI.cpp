#include "undoUI.h"
#include "../Resources/resourceIdentifiers.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <algorithm>
#include <cassert>
#include <cmath>


UndoUI::UndoUI(Fonts::ID font, trmb::FontHolder &fonts, SoundEffects::ID soundEffect, trmb::SoundPlayer &soundPlayer,
	EventGuid leftClickPress, EventGuid leftClickRelease)
: mLeftClickPress(leftClickPress)
, mLeftClickRelease(leftClickRelease)
, mFont(font)
, mFonts(fonts)
, mSoundEffect(soundEffect)
, mSoundPlayer(soundPlayer)
, mUIElemSize(sf::Vector2f(50.0f, 20.0f))
, mOutLineThickness(1.0f)
, mFrameBuffer(sf::Vector2f(7.0f, 7.0f))
, mHorizontalBuffer(1.0f)
, mDisable(false)
, mRestoreBackgroundSize()
{
	mBackground.setPosition(0.0f, 0.0f);
	mBackground.setFillColor(sf::Color(0u, 0u, 0u, 200u));
	mBackground.setOutlineColor(sf::Color(0u, 0u, 0u, 255u));
	mBackground.setOutlineThickness(mOutLineThickness);
	// ALW - Default mBackground size as if there is one UI element, so it is visible by default.
	mBackground.setSize(sf::Vector2f(mFrameBuffer.x * 2.0f + mUIElemSize.x, mFrameBuffer.y * 2.0f + mUIElemSize.y));
}

sf::Vector2f UndoUI::getSize() const
{
	return mBackground.getSize();
}

sf::FloatRect UndoUI::getRect() const
{
	const sf::Vector2f backgroundPosition = mBackground.getPosition();
	const sf::Vector2f backgroundSize = mBackground.getSize();
	sf::FloatRect rect = sf::FloatRect(backgroundPosition.x, backgroundPosition.y, backgroundSize.x, backgroundSize.y);
	rect = getTransform().transformRect(rect);

	return rect;
}

void UndoUI::setSize(sf::Vector2f size)
{
	for (const auto &uiElem : mUIElems)
	{
		uiElem->setSize(size);
	}

	buildUI();
}

void UndoUI::setCharacterSize(unsigned int size)
{
	for (const auto &uiElem : mUIElems)
	{
		uiElem->setCharacterSize(size);
	}
}

void UndoUI::setUIElemState(const std::deque<bool> &flags)
{
	assert(("The number of boolean flags must match the number of UI elements!", flags.size() == mUIElems.size()));

	std::deque<bool>::const_iterator iter = begin(flags);

	for (const auto &uiElem : mUIElems)
	{
		uiElem->setState(*iter);
		++iter;
	}
}

void UndoUI::enable()
{
	mDisable = false;
	for (const auto &uiElem : mUIElems)
	{
		uiElem->enable();
	}
}

void UndoUI::disable()
{
	mDisable = true;
	for (const auto &uiElem : mUIElems)
	{
		uiElem->disable();
	}
}

void UndoUI::unhide()
{
	mBackground.setSize(mRestoreBackgroundSize);

	for (const auto &uiElem : mUIElems)
	{
		uiElem->unhide();
	}

	enable();
}

void UndoUI::hide()
{
	// ALW - hide() must be called before unhide(), so the restore values are assigned.
	disable();

	mRestoreBackgroundSize = mBackground.getSize();
	const sf::Vector2f hideBackground = sf::Vector2f(0.0f, 0.0f);
	mBackground.setSize(hideBackground);

	for (const auto &uiElem : mUIElems)
	{
		uiElem->hide();
	}
}

void UndoUI::setCallbacks(const std::vector<CallbackPair> &callbacks)
{
	assert(("The number of callbacks must match the number of UI elements!", callbacks.size() == mUIElems.size()));

	std::vector<CallbackPair>::const_iterator iter = begin(callbacks);

	for (const auto &uiElem : mUIElems)
	{
		uiElem->setCallbacks(iter->first, iter->second);
		++iter;
	}
}

void UndoUI::handler(const sf::RenderWindow &window, const sf::View &view, const sf::Transform &transform)
{
	if (!mDisable)
	{
		sf::Transform combinedTransform = getTransform() * transform;

		for (const auto &uiElem : mUIElems)
		{
			uiElem->handler(window, view, combinedTransform);
		}
	}
}

void UndoUI::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(mBackground, states);

	for (const auto &uiElem : mUIElems)
	{
		target.draw(*uiElem, states);
	}
}

void UndoUI::addUIElem(std::string doString, std::string undoString)
{
	mUIElems.emplace_back(std::unique_ptr<trmb::UndoUIElem>(new trmb::UndoUIElem(mFont, mFonts, mSoundEffect, mSoundPlayer
		, mLeftClickPress, mLeftClickRelease)));
	mUIElems.back()->setText(doString, undoString);
	mUIElems.back()->setVisualScheme(sf::Color(95u, 158u, 160u, 255u), sf::Color(255u, 255u, 255u, 255u), sf::Color(0u, 0u, 0u, 255u)
		, sf::Color(162u, 181u, 205u, 255u), sf::Color(255u, 255u, 255u, 255u), sf::Color(0u, 0u, 0u, 255u)       // Hover
		, sf::Color(88u, 146u, 148u, 255u), sf::Color(255u, 255u, 255u, 255u), sf::Color(0u, 0u, 0u, 255u)        // Depress
		, sf::Color(160u, 160u, 160u, 255u), sf::Color(224u, 224u, 224u, 255u), sf::Color(128u, 128u, 128u, 255u) //Disablle
		, mOutLineThickness);

	buildUI();
}

void UndoUI::buildUI()
{
	if (!mUIElems.empty())
	{
		sf::Vector2f uiElemOffset = mFrameBuffer;
		sf::Vector2f backgroundSize;

		for (const auto &uiElems : mUIElems)
		{
			uiElems->setPosition(uiElemOffset);
			uiElemOffset.x = uiElems->getPosition().x + uiElems->getSize().x + mHorizontalBuffer;

			backgroundSize.x += uiElems->getSize().x;
			backgroundSize.y = std::max(backgroundSize.y, uiElems->getSize().y);
		}

		backgroundSize.x = backgroundSize.x + mFrameBuffer.x * 2.0f + mHorizontalBuffer * (mUIElems.size() - 1);
		backgroundSize.y = backgroundSize.y + mFrameBuffer.y * 2.0f;
		mBackground.setSize(backgroundSize);
	}
}

void centerOrigin(UndoUI &undoUI, bool centerXAxis, bool centerYAxis)
{
	sf::Vector2f size = undoUI.getSize();
	float xAxis = 0.0f;
	float yAxis = 0.0f;

	if (centerXAxis)
		xAxis = std::floor(size.x / 2.0f);

	if (centerYAxis)
		yAxis = std::floor(size.y / 2.0f);

	undoUI.setOrigin(xAxis, yAxis);
}
