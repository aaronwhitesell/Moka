#ifndef CHAT_BOX_UI_H
#define CHAT_BOX_UI_H

#include "Trambo/Events/eventHandler.h"
#include "Trambo/Resources/resourceHolder.h"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

#include <string>


namespace sf
{
	class RenderStates;
	class RenderTarget;
	class RenderWindow;
}

namespace trmb
{
	class Event;
	class SoundPlayer;
}

class ChatBoxUI : public sf::Drawable, public trmb::EventHandler
{
public:
						  ChatBoxUI(sf::RenderWindow &window, Fonts::ID font, trmb::FontHolder &fonts, SoundEffects::ID soundEffect
							  , trmb::SoundPlayer &soundPlayer);
						  ChatBoxUI(const ChatBoxUI &) = delete;
	ChatBoxUI &			  operator=(const ChatBoxUI &) = delete;

	virtual void		  handleEvent(const trmb::Event &gameEvent) final;
	void				  updateText(std::string string);


private:
	typedef unsigned long EventGuid;

	
private:
	virtual void		  draw(sf::RenderTarget &target, sf::RenderStates states) const final;

	void				  formatText(std::string string);
	void				  displayMoreText();
	void				  setTextLinePosition();
	void				  calculateLinesToDraw();

	bool				  isPrompt() const;
	bool				  isEllipsisLine(std::string string);

	std::string 		  getLine(std::string &string, bool addEllipsis = false);
	sf::FloatRect		  getBounds() const;

	std::string			  standardizeString(std::string);
	std::string			  trimLeadingLeft(std::string string) const;
	void				  repositionGUI();


private:
	const EventGuid		  mCreateTextPrompt; // ALW - Is sent from here.
	const EventGuid		  mClearTextPrompt;  // ALW - Is sent from here.
	const EventGuid		  mEnter;            // ALW - Matches the GUID in the Controller class.
	const EventGuid		  mFullscreen;		 // ALW - Matches the GUID in the ToggleFullscreen class.
	const EventGuid		  mWindowed;		 // ALW - Matches the GUID in the ToggleFullscreen class.

	const float			  mHorizontalSpacing;
	const float			  mVerticalSpacing;
	const unsigned int	  mMaxLinesDrawn;

	sf::RenderWindow	  &mWindow;
	trmb::FontHolder	  &mFonts;
	SoundEffects::ID	  mSoundEffect;
	trmb::SoundPlayer	  &mSoundPlayer;

	sf::RectangleShape	  mBackground;
	sf::Text			  mTextLine;
	sf::Text			  mPrompt;
	std::vector<sf::Text> mWordWrapText;
	int					  mLinesToDraw;
};

#endif
