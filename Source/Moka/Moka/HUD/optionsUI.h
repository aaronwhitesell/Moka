#ifndef OPTIONS_UI_H
#define OPTIONS_UI_H

#include "Trambo/HUD/gameTab.h"
#include "Trambo/HUD/incDec.h"
#include "Trambo/HUD/tabContainer.h"
#include "Trambo/Resources/resourceHolder.h"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include <string>


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

class OptionsUI : public sf::Transformable, public sf::Drawable
{
public:
	using Callback = trmb::GameTab::Callback;
	using EventGuid = unsigned long;


public:
							OptionsUI(Fonts::ID font, trmb::FontHolder &fonts, SoundEffects::ID soundEffect
								, trmb::SoundPlayer &soundPlayer, EventGuid leftClickPress, EventGuid leftClickRelease);
							OptionsUI(const OptionsUI &) = delete;
	OptionsUI&				operator=(const OptionsUI &) = delete;

	sf::Vector2f			getSize() const;
	sf::FloatRect			getRect() const;

	void					setTabSize(sf::Vector2f size);
	void					setLHSTabText(std::string string);
	void					setRHSTabText(std::string string);
	void					setCharacterSize(unsigned int size);

	void					handler(const sf::RenderWindow &window, const sf::View &view, const sf::Transform &transform);
	void					updateIncDecCallbacks(Callback incPurchaseCallback, Callback decPurchaseCallback
								, Callback incRepairCallback, Callback decRepairCallback);
	void					reset();
	void					enable();
	void					disable();


private:
	using TabPtr = trmb::TabContainer::Ptr;


private:
	virtual void			draw(sf::RenderTarget &target, sf::RenderStates states) const override final;
	void					standardizeCharacterSize();
	void					buildUI();
	void					lhsTab();
	void					rhsTab();


private:
	const sf::Vector2f		mFrameBuffer;
	const float				mOutLineThickness;
	sf::Vector2f			mTabSize;
	int						mTabCount;

	trmb::TabContainer		mTabs;
	trmb::IncDec			mIncDecUI;
	float					mHorizontalBuffer;
	float					mVerticalBuffer;
	bool					mDrawIncDecUI;

	sf::RectangleShape		mBackground;
	sf::Vector2f            mBackgroundDefaultSize;
	sf::Vector2f            mBackgroundExpandedSize;

	TabPtr					mLHSTab;
	TabPtr					mRHSTab;

	Callback                mIncPurchaseCallback;
	Callback                mDecPurchaseCallback;
	Callback                mIncRepairCallback;
	Callback                mDecRepairCallback;
};

void	centerOrigin(OptionsUI &button, bool centerXAxis = true, bool centerYAxis = true);

#endif
