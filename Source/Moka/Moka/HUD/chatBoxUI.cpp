#include "chatBoxUI.h"
#include "../HUD/optionsUI.h"
#include "../HUD/undoUI.h"
#include "../HUD/uiBundle.h"
#include "../Resources/resourceIdentifiers.h"

#include "Trambo/Camera/camera.h"
#include "Trambo/Events/event.h"
#include "Trambo/Localize/localize.h"
#include "Trambo/Sounds/soundPlayer.h"
#include "Trambo/Utilities/utility.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Transform.hpp>

#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <sstream>


ChatBoxUI::ChatBoxUI(const sf::RenderWindow &window, trmb::Camera &camera, Fonts::ID font, trmb::FontHolder &fonts
	, SoundEffects::ID soundEffect, trmb::SoundPlayer &soundPlayer, UIBundle &uiBundle)
: mCreateTextPrompt(0x25e87fd8)
, mClearTextPrompt(0xc1523265)
, mEnter(0xff349d1d)
, mFullscreen(0x5a0d2314)
, mWindowed(0x11e3c735)
, mHorizontalSpacing(2.0f)
, mVerticalSpacing(15.0f)
, mMaxLinesDrawn(5u)
, mWindow(window)
, mCamera(camera)
, mFonts(fonts)
, mSoundEffect(soundEffect)
, mSoundPlayer(soundPlayer)
, mUIBundle(uiBundle)
, mLinesToDraw(mMaxLinesDrawn)
, mForceEndPrompt(false)
, mMouseOver(false)
, mUIBundleDisabled(false)
{
	mBackground.setSize(sf::Vector2f(300.0f, 77.0f));
	mBackground.setFillColor(sf::Color(0u, 0u, 0u, 200u));
	mBackground.setOutlineColor(sf::Color(0u, 0u, 0u, 255u));
	mBackground.setOutlineThickness(1.0f);
	mBackground.setPosition(0.0f, 0.0f);

	// ALW - Position text at the top left of the ChatBoxUI
	mTextLine.setFont(mFonts.get(font));
	mTextLine.setCharacterSize(14u);
	mTextLine.setPosition(sf::Vector2f(0.0f, 0.0f));

	mPrompt.setFont(mFonts.get(font));
	mPrompt.setCharacterSize(14u);
	mPrompt.setString(trmb::Localize::getInstance().getString("prompt"));
	mPrompt.setColor(sf::Color(128u, 128u, 128u, 255u));
	float promptWidth = mPrompt.getGlobalBounds().width;
	trmb::centerOrigin(mPrompt, true, false);
	mPrompt.setPosition(mBackground.getSize().x / 2.0f, mVerticalSpacing * (mMaxLinesDrawn - 1));

	// ALW - Calculate x, y coordinates relative to the center of the window,
	// ALW - so GUI elements are equidistance from the center in any resolution.
	const sf::Vector2f windowCenter = sf::Vector2f(mWindow.getSize() / 2u);
	const float bufferFromBottom = 10.0f;
	centerOrigin(*this, true, false);
	setPosition(windowCenter.x, mWindow.getSize().y - mBackground.getSize().y - bufferFromBottom);
}

sf::Vector2f ChatBoxUI::getSize() const
{
	return mBackground.getSize();
}

sf::FloatRect ChatBoxUI::getRect() const
{
	// ALW - The ChatBoxUI has an absolute position in the world that is translated by the camera position.
	const sf::Vector2f backgroundPosition = mBackground.getPosition();
	const sf::Vector2f backgroundSize = mBackground.getSize();
	sf::FloatRect rect = sf::FloatRect(backgroundPosition.x, backgroundPosition.y, backgroundSize.x, backgroundSize.y);

	sf::Vector2f cameraPosition(mCamera.getViewBounds().left, mCamera.getViewBounds().top);
	sf::Transform translatedTransform = getTransform();
	translatedTransform = translatedTransform.translate(cameraPosition);
	rect = translatedTransform.transformRect(rect);

	return rect;
}

void ChatBoxUI::handler()
{
	// ALW - The ChatBoxUI has an absolute position in the world that is translated by the camera position.
	sf::Vector2f cameraPosition(mCamera.getViewBounds().left, mCamera.getViewBounds().top);
	sf::Transform translatedTransform = getTransform();
	translatedTransform = translatedTransform.translate(cameraPosition);

	const sf::Vector2i relativeToWindow = sf::Mouse::getPosition(mWindow);
	const sf::Vector2f relativeToWorld = mWindow.mapPixelToCoords(relativeToWindow, mCamera.getView());
	const sf::Vector2f mousePosition = relativeToWorld;

	sf::FloatRect UIRect(mBackground.getPosition().x, mBackground.getPosition().y, mBackground.getSize().x, mBackground.getSize().y);
	UIRect = translatedTransform.transformRect(UIRect);

	if (UIRect.contains(mousePosition))
	{
		mMouseOver = true;

		if (!mUIBundleDisabled)
		{
			mUIBundleDisabled = true;

			if (!mUIBundle.getBarrelUI().isHidden())
				mUIBundle.getBarrelUI().disable(false);

			if (!mUIBundle.getDoorUI().isHidden())
				mUIBundle.getDoorUI().disable(false);

			if (!mUIBundle.getWindowUI().isHidden())
				mUIBundle.getWindowUI().disable(false);

			if (!mUIBundle.getClinicUI().isHidden())
				mUIBundle.getClinicUI().disable(false);

			if (!mUIBundle.getHouseUI().isHidden())
				mUIBundle.getHouseUI().disable(false);
		}
	}
	else
	{
		mMouseOver = false;

		if (mUIBundleDisabled)
		{
			mUIBundleDisabled = false;

			if (!mUIBundle.getBarrelUI().isHidden())
				mUIBundle.getBarrelUI().enable();

			if (!mUIBundle.getDoorUI().isHidden())
				mUIBundle.getDoorUI().enable();

			if (!mUIBundle.getWindowUI().isHidden())
				mUIBundle.getWindowUI().enable();

			if (!mUIBundle.getClinicUI().isHidden())
				mUIBundle.getClinicUI().enable();

			if (!mUIBundle.getHouseUI().isHidden())
				mUIBundle.getHouseUI().enable();
		}
	}
}

void ChatBoxUI::handleEvent(const trmb::Event &gameEvent)
{
	if (mEnter == gameEvent.getType())
	{
		// ALW - There are three possibilities when an <Enter> is handled. There is an overflow of of text.
		// ALW - More text will be displayed and the prompt will be cleared if there is not more text and
		// ALW - an end prompt is not being forced. Next, if there is no more text to be displayed and an
		// ALW - end prompt was forced then it will be cleared. Otherwise, there is nothing to do here.
		if (isOverFlow())
		{
			displayMoreText();
		}
		else if (mForceEndPrompt)
		{
			clearPrompt();
		}
	}
	// ALW - Currently, fullscreen and windowed mode are the same.
	else if (mFullscreen == gameEvent.getType() || mWindowed == gameEvent.getType())
	{
		repositionGUI();
	}
}

void ChatBoxUI::updateText(std::string string, bool forceEndPrompt)
{
	// ALW - Non-interupting messages do not force an end prompt and their length is less than the capacity of a chatbox. These 
	// ALW - messages are used when you select a barrel for example. They do not interrupt gameplay. Interupting messages force
	// ALW - an end prompt and can be of any length. They force the user to acknowledge the message. They are used for narrtive
	// ALW - and educational messages.
	mForceEndPrompt = forceEndPrompt;
	mWordWrapText.clear();
	formatText(string);
	setTextLinePosition();
	calculateLinesToDraw();

	if (!mForceEndPrompt)
		assert(("Non-interupting messages do not force an end prompt and cannot be longer that the capacity of a chatbox!", !isOverFlow()));

	if (isOverFlow() || mForceEndPrompt)
		EventHandler::sendEvent(trmb::Event(mCreateTextPrompt));
}

void ChatBoxUI::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	// ALW - Save then change view
	sf::View previousView = target.getView();
	target.setView(target.getDefaultView());

	target.draw(mBackground, states);

	if (!mWordWrapText.empty())
	{
		int count = 0;
		std::vector<sf::Text>::const_iterator line = begin(mWordWrapText);
		while (count < mLinesToDraw)
		{
			target.draw(*line, states);
			++line;
			++count;
		}
	}

	if (isOverFlow() || mForceEndPrompt)
		target.draw(mPrompt, states);

	// ALW - Restore the view
	target.setView(previousView);
}

void ChatBoxUI::formatText(std::string string)
{
	string = standardizeString(string);
	mTextLine.setString(string);
	int lineCount = 0;
	const int ellipsisLine = 3;
	const float chatBoxWidth = getBounds().width;
	while (mTextLine.getLocalBounds().width > chatBoxWidth)
	{
		// ALW - Current line may need ellipsis
		if (lineCount == ellipsisLine)
		{
			// ALW - If we have more lines than the current and next line then an ellipsis is needed
			if (isEllipsisLine(string))
			{
				// ALW - Add ellipsis to this line
				mTextLine.setString(getLine(string, true));
				lineCount = 0;
			}
			else
				mTextLine.setString(getLine(string));
			// ALW - Last time through loop.  No need for ++lineCount;
		}
		else
		{
			mTextLine.setString(getLine(string));
			++lineCount;
		}

		mWordWrapText.emplace_back(mTextLine);

		// ALW - mTextLine is set to the still unread portion of the dialog
		mTextLine.setString(string);
	}

	// ALW - Ignore an empty string
	if (mTextLine.getLocalBounds().width != 0.0f)
		// ALW - Push the last verified line onto the vector
		mWordWrapText.emplace_back(mTextLine);
}

void ChatBoxUI::displayMoreText()
{
	if (isOverFlow())
	{
		assert(("ALW - Logic Error: The vector cannot be empty!", mWordWrapText.size() > (mMaxLinesDrawn - 1)));

		// ALW - Calculate the beginning and ending iterators for the lines that have already been drawn
		std::vector<sf::Text>::const_iterator line = begin(mWordWrapText);
		for (int i = 0; i < mLinesToDraw; ++i)
			++line;

		std::vector<sf::Text>::const_iterator endLine = line;
		line = begin(mWordWrapText);

		// ALW - Delete the lines that have already been drawn
		mWordWrapText.erase(line, endLine);

		setTextLinePosition();
		calculateLinesToDraw();
		mSoundPlayer.play(mSoundEffect);
		if (!isOverFlow() && !mForceEndPrompt)
			EventHandler::sendEvent(trmb::Event(mClearTextPrompt));
	}
}

void ChatBoxUI::setTextLinePosition()
{
	// ALW - Set the position for each line of text in mWordWrapText. Do this by
	// ALW - starting at the top-left position of the chat box. Then account for
	// ALW - horizontal and vertical spacing, and the line number.

	int lineCount = 0;
	sf::Vector2f topLeftOfBackground(0.0f, 0.0f);

	for (auto &textLine : mWordWrapText)
	{
		textLine.setPosition(topLeftOfBackground + sf::Vector2f(mHorizontalSpacing, mVerticalSpacing * lineCount));
		++lineCount;
	}
}

void ChatBoxUI::calculateLinesToDraw()
{
	if (mWordWrapText.size() < mMaxLinesDrawn)
	{
		// ALW - All lines in mWordWrapText will be drawn
		mLinesToDraw = mWordWrapText.size();
	}
	else if (mWordWrapText.size() > mMaxLinesDrawn
		|| (mForceEndPrompt && mWordWrapText.size() == mMaxLinesDrawn))
	{
		// ALW - Not all of the lines in mWordWrapText can be drawn.
		// ALW - Draw one minus the maximum number of lines, so a text
		// ALW - prompt can be displayed on the last line.
		mLinesToDraw = mMaxLinesDrawn - 1;
	}
	else
		mLinesToDraw = mMaxLinesDrawn;
}

void ChatBoxUI::clearPrompt()
{
	mForceEndPrompt = false;
	mSoundPlayer.play(mSoundEffect);
	EventHandler::sendEvent(trmb::Event(mClearTextPrompt));
}

bool ChatBoxUI::isOverFlow() const
{
	// ALW - The text fills an entire chatbox and then some. Or an end prompt is forced
	// ALW - and the text plus the prompt fills an entire chatbox and then some.
	bool ret = false;
	if (mWordWrapText.size() > mMaxLinesDrawn
		|| (mForceEndPrompt && mWordWrapText.size() == mMaxLinesDrawn))
	{
		ret = true;
	}

	return ret;
}

bool ChatBoxUI::isEllipsisLine(std::string string)
{
	// ALW - Remember string
	std::string text = mTextLine.getString();
	bool ret = false;

	std::vector<sf::Text> lines;
	std::size_t maxLinesLeft = 0;
	if (mForceEndPrompt)
		maxLinesLeft = 1; // ALW - Consists of (potential) ellipsis line and assumes there will be a prompt
	else
		maxLinesLeft = 2; // ALW - Consists of (potential) ellipsis line and next line

	while (mTextLine.getLocalBounds().width != 0.0f)
	{
		mTextLine.setString(getLine(string));
		lines.emplace_back(mTextLine);
		if (lines.size() > maxLinesLeft)
		{
			ret = true;
			break;
		}

		// ALW - mTextLine is set to the still unread portion of the dialog
		mTextLine.setString(string);
	}

	// ALW - Restore string
	mTextLine.setString(text);

	return ret;
}

std::string ChatBoxUI::getLine(std::string &string, bool addEllipsis)
{
	std::string verifiedLine;
	std::istringstream iss;
	iss.str(string);
	std::string word;
	iss >> word;
	std::string line = word;
	mTextLine.setString(line);
	float chatBoxWidth(getBounds().width);
	if (addEllipsis)
	{
		// ALW - Subtracting width of ellipsis from chatBox width is easier than adding it to the mTextLine's width
		mTextLine.setString("...");
		chatBoxWidth -= mTextLine.getLocalBounds().width;
	}

	if (mTextLine.getLocalBounds().width > chatBoxWidth)
	{
		throw std::runtime_error("ChatBoxUI.cpp - A single word cannot be larger than the chat box!");
	}
	else
	{
		std::streampos pos;
		while (mTextLine.getLocalBounds().width <= chatBoxWidth)
		{
			// ALW - The line was verified
			verifiedLine = line;

			// ALW - Remember the current position in iss's buffer
			pos = iss.tellg();

			// ALW - Add the next word to the line and test 
			iss >> word;
			line += " " + word;
			mTextLine.setString(line);
		}

		// ALW - Revert iss's buffer to before the last word was read
		iss.clear();
		iss.seekg(pos);

		// ALW - Set string to the unread portion of iss's buffer
		std::ostringstream oss;
		oss << iss.rdbuf();
		string = oss.str();
		string = trimLeadingLeft(string);
	}

	if (addEllipsis)
		verifiedLine += "...";

	return verifiedLine;
}

sf::FloatRect ChatBoxUI::getBounds() const
{
	const sf::FloatRect bounds = mBackground.getLocalBounds();
	const float outLineThickness = mBackground.getOutlineThickness();

	// ALW - Return the writeable bounds.  Remove outline thickness and horizontal spacing.
	return sf::FloatRect(bounds.left + outLineThickness + mHorizontalSpacing, bounds.top + outLineThickness
		, bounds.width - outLineThickness * 2.0f - mHorizontalSpacing * 2.0f, bounds.height - outLineThickness * 2.0f);
}

std::string ChatBoxUI::standardizeString(std::string string)
{
	// ALW - A standardized string is string with a single space between each word.
	std::replace(begin(string), end(string), '\n', ' ');
	std::istringstream iss(string);
	std::string word;
	if (iss >> word)
	{
		string = word;

		while (iss >> word)
			string += " " + word;
	}

	return string;
}

std::string ChatBoxUI::trimLeadingLeft(std::string string) const
{
	string.erase(begin(string), std::find_if(begin(string), end(string),
		[](char c)
		{
			return c != ' ';
		}));

	return string;
}

void ChatBoxUI::repositionGUI()
{
	// ALW - Calculate x, y coordinates relative to the center of the window,
	// ALW - so GUI elements are equidistance from the center in any resolution.
	const sf::Vector2f windowCenter = sf::Vector2f(mWindow.getSize() / 2u);
	const float bufferFromBottom = 10.0f;
	setPosition(windowCenter.x, mWindow.getSize().y - mBackground.getSize().y - bufferFromBottom);
}

void centerOrigin(ChatBoxUI &ui, bool centerXAxis, bool centerYAxis)
{
	sf::Vector2f size = ui.getSize();
	float xAxis = 0.0f;
	float yAxis = 0.0f;

	if (centerXAxis)
		xAxis = std::floor(size.x / 2.0f);

	if (centerYAxis)
		yAxis = std::floor(size.y / 2.0f);

	ui.setOrigin(xAxis, yAxis);
}
