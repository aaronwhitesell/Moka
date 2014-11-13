#ifndef HOUSE_UI_H
#define HOUSE_UI_H

#include "Trambo/HUD/gameTab.h"
#include "Trambo/HUD/incDec.h"
#include "Trambo/HUD/tabContainer.h"
#include "Trambo/Resources/resourceHolder.h"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>


namespace sf
{
	class RenderStates;
	class RenderTarget;
	class RenderWindow;
	class View;
}

namespace trmb
{
	class SoundPlayer;
}

class HouseUI : public sf::Transformable, public sf::Drawable
{
public:
	using Callback = trmb::GameTab::Callback;
	using EventGuid = unsigned long;


public:
							HouseUI(Fonts::ID font, trmb::FontHolder &fonts, SoundEffects::ID soundEffect
								, trmb::SoundPlayer &soundPlayer, EventGuid leftClickPress, EventGuid leftClickRelease);
							HouseUI(const HouseUI &) = delete;
	HouseUI&				operator=(const HouseUI &) = delete;

	sf::Vector2f			getSize() const;
	sf::FloatRect			getRect() const;

	void					setPurchaseCallback(Callback callback);
	void					setRepairCallback(Callback callback);

	void					handler(const sf::RenderWindow &window, const sf::View &view, const sf::Transform &transform);
	void					updateIncDecCallbacks(Callback incPurchaseCallback, Callback decPurchaseCallback
								, Callback incRepairCallback, Callback decRepairCallback);
	void					deactivate();


private:
	using TabPtr = trmb::TabContainer::Ptr;


private:
	virtual void			draw(sf::RenderTarget &target, sf::RenderStates states) const override final;
	void					purchase();
	void					repair();


private:
	trmb::TabContainer		mOptionTabs;
	trmb::IncDec			mIncDecUI;
	float					mHorizontalBuffer;
	float					mVerticalBuffer;
	bool					mDrawIncDecUI;

	sf::RectangleShape		mBackground;
	sf::Vector2f            mBackgroundDefaultSize;
	sf::Vector2f            mBackgroundExpandedSize;

	TabPtr					mPurchaseTab;
	TabPtr					mRepairTab;

	Callback                mIncPurchaseCallback;
	Callback                mDecPurchaseCallback;
	Callback                mIncRepairCallback;
	Callback                mDecRepairCallback;
};

void	centerOrigin(HouseUI &button, bool centerXAxis = true, bool centerYAxis = true);

#endif
