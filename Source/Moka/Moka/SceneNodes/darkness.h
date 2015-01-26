#ifndef DARKNESS_H
#define DARKNESS_H

#include "Trambo/Events/eventHandler.h"
#include "Trambo/SceneNodes/sceneNode.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Time.hpp>


namespace sf
{
	class RenderStates;
	class RenderTarget;
	class RenderWindow;
}

namespace trambo
{
	class Event;
}

class Darkness : public trmb::SceneNode, trmb::EventHandler
{
public:
							Darkness(const sf::RenderWindow &window);
							Darkness(const Darkness &) = delete;
	Darkness &				operator=(const Darkness &) = delete;

	virtual void			updateCurrent(sf::Time dt) override final;
	virtual void			handleEvent(const trmb::Event &gameEvent) override final;


private:
	using					EventGuid = unsigned long;


private:
	virtual void			drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override;
	void				    reposition();


private:
	const EventGuid		    mFullscreen;		    // ALW - Matches the GUID in the ToggleFullscreen class.
	const EventGuid		    mWindowed;		        // ALW - Matches the GUID in the ToggleFullscreen class.
	const EventGuid         mBeginSimulationMode;   // ALW - Matches the GUID in the DaylightUI class.
	const sf::RenderWindow  &mWindow;
	sf::RectangleShape		mBackground;
	bool					mDarken;
	int						mCount;
	unsigned char           mAlpha;
	sf::Time				mUpdateTime;

};

#endif
