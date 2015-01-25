#ifndef DAYLIGHT_UI_H
#define DAYLIGHT_UI_H

#include "Trambo/Events/event.h"
#include "Trambo/Events/eventHandler.h"
#include "Trambo/HUD/buttonContainer.h"
#include "Trambo/HUD/gameButton.h"
#include "Trambo/Resources/resourceHolder.h"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Rect.hpp>
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
	class Camera;
	class Event;
	class SoundPlayer;
}

class UIBundle;

class DaylightUI : public sf::Transformable, public sf::Drawable, public trmb::EventHandler
{
public:
	using EventGuid = unsigned long;


public:
						  DaylightUI(sf::RenderWindow &window, trmb::Camera &camera, Fonts::ID font, trmb::FontHolder &fonts
						     , SoundEffects::ID soundEffect, trmb::SoundPlayer &soundPlayer, UIBundle &uiBundle
							 , EventGuid leftClickPress, EventGuid leftClickRelease);
						  DaylightUI(const DaylightUI &) = delete;
	DaylightUI            operator=(const DaylightUI &) = delete;

	sf::Vector2f		  getSize() const;
	sf::FloatRect		  getRect() const;

	void				  handler();
	virtual void		  handleEvent(const trmb::Event &gameEvent) final;

	void				  add(float addend);
	bool				  subtract(float subtrahend);

	void			      done();


private:
	using EventGuid = unsigned long;
	using ButtonPtr = trmb::ButtonContainer::Ptr;

	
private:
	virtual void		  draw(sf::RenderTarget &target, sf::RenderStates states) const final;
	void				  disable();
	void				  repositionGUI();


private:
	const trmb::Event     mBeginSimulationMode; // ALW - Is sent from here.
	const EventGuid		  mFullscreen;		    // ALW - Matches the GUID in the ToggleFullscreen class.
	const EventGuid		  mWindowed;		    // ALW - Matches the GUID in the ToggleFullscreen class.

	const float		      mMaxHours;
	const float			  mMinHours;
	const int			  mFloatPrecision;

	sf::RenderWindow	  &mWindow;
	trmb::Camera          &mCamera;
	trmb::FontHolder	  &mFonts;
	trmb::SoundPlayer	  &mSoundPlayer;
	UIBundle			  &mUIBundle;

	sf::RectangleShape	  mBackground;
	sf::Text			  mDaylightText;
	sf::Text			  mHoursText;
	float				  mHourCount;

	ButtonPtr             mButton;
	trmb::ButtonContainer mContainer;
	bool                  mMouseOver;
	bool				  mUIBundleDisabled;
};

void	centerOrigin(DaylightUI &ui, bool centerXAxis = true, bool centerYAxis = true);

#endif
