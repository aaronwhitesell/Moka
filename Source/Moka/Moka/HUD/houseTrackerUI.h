#ifndef HOUSE_TRACKER_UI_H
#define HOUSE_TRACKER_UI_H

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
}

namespace trmb
{
	class Event;
	class SoundPlayer;
}

class HouseTrackerUI : public sf::Transformable, public sf::Drawable, public trmb::EventHandler
{
public:
							HouseTrackerUI(Fonts::ID font, trmb::FontHolder &fonts, SoundEffects::ID soundEffect, trmb::SoundPlayer &soundPlayer);
							HouseTrackerUI(const HouseTrackerUI &) = delete;
	HouseTrackerUI			operator=(const HouseTrackerUI &) = delete;

	sf::Vector2f		    getSize() const;

	void				    addInfectedMosquito();
	void				    subtractInfectedMosquito();

	void				    addMosquitoTotal();
	void				    subtractMosquitoTotal();

	virtual void		    handleEvent(const trmb::Event &gameEvent) final;


private:
	using                   EventGuid = unsigned long;


private:
	virtual void		    draw(sf::RenderTarget &target, sf::RenderStates states) const final;
	void				    unhide();
	void				    hide();


private:
	const EventGuid			mBeginSimulationEvent; // ALW - Matches the GUID in the DaylightUI class.

	trmb::SoundPlayer	    &mSoundPlayer;

	sf::RectangleShape	    mBackground;
	sf::RectangleShape	    mMosquitoTextBackground;
	sf::Text			    mMosquitoText;
	sf::RectangleShape	    mInfectedMosquitoCountBackground;
	sf::RectangleShape	    mMosquitoTotalBackground;
	sf::Text			    mInfectedMosquitoCountText;
	sf::Text			    mMosquitoTotalText;
	int						mInfectedMosquitoCount;
	int						mMosquitoTotal;

	bool                    mHide;
};

void	centerOrigin(HouseTrackerUI &ui, bool centerXAxis = true, bool centerYAxis = true);

#endif
