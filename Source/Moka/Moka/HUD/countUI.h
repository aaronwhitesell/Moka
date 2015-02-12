#ifndef COUNT_UI_H
#define COUNT_UI_H

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

class CountUI : public sf::Transformable, public sf::Drawable, public trmb::EventHandler
{
public:
							CountUI(Fonts::ID font, trmb::FontHolder &fonts, SoundEffects::ID soundEffect, trmb::SoundPlayer &soundPlayer);
							CountUI(const CountUI &) = delete;
	CountUI					operator=(const CountUI &) = delete;

	sf::Vector2f		    getSize() const;

	virtual void		    handleEvent(const trmb::Event &gameEvent) final;

	void				    addLeft();
	void				    addRight();
	void				    subtractLeft();
	void				    subtractRight();


private:
	using                   EventGuid = unsigned long;


private:
	virtual void		    draw(sf::RenderTarget &target, sf::RenderStates states) const final;
	void				    unhide();
	void				    hide();


private:
	const EventGuid			mBeginSimulationEvent; // ALW - Matches the GUID in the DaylightUI class.

	trmb::FontHolder	    &mFonts;
	trmb::SoundPlayer	    &mSoundPlayer;

	sf::RectangleShape	    mBackground;
	sf::RectangleShape	    mCountBackgroundLeft;
	sf::RectangleShape	    mCountBackgroundRight;
	sf::Text			    mCountTextLeft;
	sf::Text			    mCountTextRight;
	int						mCountLeft;
	int						mCountRight;

	bool                    mHide;
};

void	centerOrigin(CountUI &ui, bool centerXAxis = true, bool centerYAxis = true);

#endif
