#ifndef SCOREBOARD_UI_H
#define SCOREBOARD_UI_H

#include "../Resources/resourceIdentifiers.h"

#include "Trambo/Events/eventHandler.h"
#include "Trambo/HUD/buttonContainer.h"
#include "Trambo/HUD/gameButton.h"
#include "Trambo/Resources/resourceHolder.h"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include <string>


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

class ScoreboardUI : public sf::Transformable, public sf::Drawable, public trmb::EventHandler
{
public:
	using                   EventGuid = unsigned long;


public:
							ScoreboardUI(const sf::RenderWindow &window, trmb::Camera &camera, trmb::FontHolder &fonts
								, SoundEffects::ID soundEffect, trmb::SoundPlayer &soundPlayer , EventGuid leftClickPress
								, EventGuid leftClickRelease);
							ScoreboardUI(const ScoreboardUI &) = delete;
	ScoreboardUI			operator=(const ScoreboardUI &) = delete;

	void					initialize(int totalResidents);

	bool					isFinished() const;

	sf::Vector2f		    getSize() const;

	void					addMosquitoSpawn();
	void					addDoorDeflection();
	void					addWindowDeflection();
	void					addNetDeflection();
	void					addCuredResident();
	void					addInfectedResident();

	void				    handler();
	virtual void		    handleEvent(const trmb::Event &gameEvent) final;

	void			        done();


private:
	using                   ButtonPtr = trmb::ButtonContainer::Ptr;


private:
	virtual void		    draw(sf::RenderTarget &target, sf::RenderStates states) const final;
	float					getMaxResultTextLength();
	float					getMaxDescTextLength();
	void					leftJustify(sf::Text &text);

	void					enable();
	void					disable();
	void				    unhide();
	void				    hide();

	void				    repositionGUI();


private:
	const EventGuid		    mFullscreen;		   // ALW - Matches the GUID in the ToggleFullscreen class.
	const EventGuid		    mWindowed;		       // ALW - Matches the GUID in the ToggleFullscreen class.
	const EventGuid			mBeginScoreboardEvent; // ALW - Matches the GUID in the World class.

	const sf::RenderWindow	&mWindow;
	trmb::Camera			&mCamera;
	trmb::FontHolder		&mFonts;
	trmb::SoundPlayer	    &mSoundPlayer;

	sf::RectangleShape	    mBackground;
	sf::Text				mResultText;
	sf::RectangleShape	    mStatsBackground;
	sf::Text				mBarrelDescText;
	sf::Text				mDoorDescText;
	sf::Text				mWindowDescText;
	sf::Text				mNetDescText;
	sf::Text				mCuredDescText;
	sf::Text				mMalariaDescText;

	sf::Text				mBarrelResultText;
	sf::Text				mDoorResultText;
	sf::Text				mWindowResultText;
	sf::Text				mNetResultText;
	sf::Text				mCuredResultText;
	sf::Text				mMalariaResultText;

	int						mSpawnedMosquitoes;
	int                     mDoorDeflections;
	int                     mWindowDeflections;
	int                     mNetDeflections;
	int                     mCuredResident;
	int						mInfectedResident;

	ButtonPtr               mButton;
	trmb::ButtonContainer   mContainer;

	bool                    mHide;
	bool					mDisable;
	bool					mFinished;
};

void	centerOrigin(ScoreboardUI &ui, bool centerXAxis = true, bool centerYAxis = true);

#endif
