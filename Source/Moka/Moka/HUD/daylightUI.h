#ifndef DAYLIGHT_UI_H
#define DAYLIGHT_UI_H

#include "Trambo/Events/eventHandler.h"
#include "Trambo/HUD/buttonContainer.h"
#include "Trambo/HUD/gameButton.h"
#include "Trambo/Resources/resourceHolder.h"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Vector2.hpp>

#include <SFML/Graphics/Text.hpp>


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

class DaylightUI : public sf::Transformable, public sf::Drawable, public trmb::EventHandler
{
public:
	using EventGuid = unsigned long;


public:
						  DaylightUI(sf::RenderWindow &window, Fonts::ID font, trmb::FontHolder &fonts, SoundEffects::ID soundEffect
							  , trmb::SoundPlayer &soundPlayer, EventGuid leftClickPress, EventGuid leftClickRelease);
						  DaylightUI(const DaylightUI &) = delete;
	DaylightUI            operator=(const DaylightUI &) = delete;

	sf::Vector2f		  getSize() const;

	void				  handler(const sf::RenderWindow &window, const sf::View &view, const sf::Transform &transform);
	virtual void		  handleEvent(const trmb::Event &gameEvent) final;

	void				  add(float addend);
	void				  subtract(float subtrahend);

	void			      done();


private:
	using EventGuid = unsigned long;
	using ButtonPtr = trmb::ButtonContainer::Ptr;

	
private:
	virtual void		  draw(sf::RenderTarget &target, sf::RenderStates states) const final;
	void				  repositionGUI();


private:
	const EventGuid		  mFullscreen;		 // ALW - Matches the GUID in the ToggleFullscreen class.
	const EventGuid		  mWindowed;		 // ALW - Matches the GUID in the ToggleFullscreen class.

	const int			  mFloatPrecision;

	sf::RenderWindow	  &mWindow;
	trmb::FontHolder	  &mFonts;
	trmb::SoundPlayer	  &mSoundPlayer;

	sf::RectangleShape	  mBackground;
	sf::Text			  mDaylightText;
	sf::Text			  mHoursText;
	float				  mHourCount;

	// ALW - Button portion
	ButtonPtr             mButton;
	trmb::ButtonContainer mContainer;
	bool                  mMouseOver;
};

void	centerOrigin(DaylightUI &ui, bool centerXAxis = true, bool centerYAxis = true);

#endif
