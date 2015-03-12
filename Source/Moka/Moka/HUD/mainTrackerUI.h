#ifndef MAIN_TRACKER_UI_H
#define MAIN_TRACKER_UI_H

#include "../Resources/resourceIdentifiers.h"

#include "Trambo/Events/eventHandler.h"
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

class MainTrackerUI : public sf::Transformable, public sf::Drawable, public trmb::EventHandler
{
public:
							MainTrackerUI(const sf::RenderWindow &window, Fonts::ID font, const trmb::FontHolder &fonts
								, SoundEffects::ID soundEffect, trmb::SoundPlayer &soundPlayer);
							MainTrackerUI(const MainTrackerUI &) = delete;
	MainTrackerUI			operator=(const MainTrackerUI &) = delete;

	sf::Vector2f		    getSize() const;

	int						getMosquitoCount() const;

	void					setMosquitoCount(int count);
	void					setResidentCount(int count);

	void				    addMosquito();

	void				    addInfectedMosquito();
	void				    subtractInfectedMosquito();

	void				    addInfectedResident();
	void				    subtractInfectedResident();

	virtual void		    handleEvent(const trmb::Event &gameEvent) final;


private:
	using                   EventGuid = unsigned long;


private:
	virtual void		    draw(sf::RenderTarget &target, sf::RenderStates states) const final;
	void					buildMosquitoUI();
	void					buildResidentUI();
	void				    unhide();
	void				    hide();
	void				    repositionGUI();


private:
	const EventGuid			mBeginSimulationEvent; // ALW - Matches the GUID in the DaylightUI class.
	const EventGuid		    mFullscreen;		   // ALW - Matches the GUID in the ToggleFullscreen class.
	const EventGuid		    mWindowed;		       // ALW - Matches the GUID in the ToggleFullscreen class.

	const sf::RenderWindow	&mWindow;
	trmb::SoundPlayer	    &mSoundPlayer;

	sf::RectangleShape	    mBackground;

	// ALW - Mosquito UI
	sf::RectangleShape	    mMosquitoTextBackground;
	sf::Text			    mMosquitoText;
	sf::RectangleShape	    mInfectedMosquitoTextBackground;
	sf::RectangleShape	    mTotalMosquitoTextBackground;
	sf::Text			    mInfectedMosquitoText;
	sf::Text			    mTotalMosquitoText;
	sf::RectangleShape	    mInfectedMosquitoCountBackground;
	sf::RectangleShape	    mMosquitoCountBackground;
	sf::Text			    mInfectedMosquitoCountText;
	sf::Text			    mMosquitoCountText;
	int						mInfectedMosquitoCount;
	int						mMosquitoCount;
	
	// ALW - Resident UI
	sf::RectangleShape	    mResidentTextTextBackground;
	sf::Text			    mResidentText;
	sf::RectangleShape	    mInfectedResidentTextBackground;
	sf::RectangleShape	    mTotalResidentTextBackground;
	sf::Text			    mInfectedResidentText;
	sf::Text			    mTotalResidentText;
	sf::RectangleShape	    mInfectedResidentCountBackground;
	sf::RectangleShape	    mResidentCountBackground;
	sf::Text			    mInfectedResidentCountText;
	sf::Text			    mResidentCountText;
	int						mInfectedResidentCount;
	int						mResidentCount;

	bool                    mHide;
};

void	centerOrigin(MainTrackerUI &ui, bool centerXAxis = true, bool centerYAxis = true);

#endif
