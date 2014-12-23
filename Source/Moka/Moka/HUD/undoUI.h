#ifndef UNDO_UI_H
#define UNDO_UI_H

#include "Trambo/HUD/undoUIElem.h"
#include "Trambo/Resources/resourceHolder.h"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include <deque>
#include <memory>
#include <utility>
#include <vector>


namespace sf
{
	class RenderStates;
	class RenderTarget;
	class RenderWindow;
	class Transform;
	class View;
}

namespace trmb
{
	class SoundPlayer;
}

class UndoUI : public sf::Transformable, public sf::Drawable
{
public:
	using Callback = trmb::UndoUIElem::Callback;
	using CallbackPair = std::pair<Callback, Callback>;
	using EventGuid = unsigned long;


public:
							UndoUI(Fonts::ID font, trmb::FontHolder &fonts, SoundEffects::ID soundEffect
								, trmb::SoundPlayer &soundPlayer, EventGuid leftClickPress, EventGuid leftClickRelease);
							UndoUI(const UndoUI &) = delete;
	UndoUI&					operator=(const UndoUI &) = delete;

	bool					isHidden() const;

	sf::Vector2f			getSize() const;
	sf::FloatRect			getRect() const;

	void					setSize(sf::Vector2f size);
	void					setCharacterSize(unsigned int size);
	void					setUIElemState(const std::deque<bool> &flags);
	void					setCallbacks(const std::vector<CallbackPair> &callback);

	void					handler(const sf::RenderWindow &window, const sf::View &view, const sf::Transform &transform);
	void					addUIElem(std::string doString, std::string undoString);
	void					enable();
	void					disable(bool useDisableColorScheme);
	void					unhide();
	void					hide();



private:
	using UIElemPtr = std::unique_ptr<trmb::UndoUIElem>;
	using UIElemVec = std::vector<UIElemPtr>;


private:
	virtual void			draw(sf::RenderTarget &target, sf::RenderStates states) const override final;
	void					buildUI();


private:
	const EventGuid			mLeftClickPress;	// ALW - Matches the GUID in the Controller class.
	const EventGuid			mLeftClickRelease;	// ALW - Matches the GUID in the Controller class.
	Fonts::ID				mFont;
	trmb::FontHolder&		mFonts;
	SoundEffects::ID		mSoundEffect;
	trmb::SoundPlayer&		mSoundPlayer;

	sf::Vector2f			mUIElemSize;
	const float				mOutLineThickness;
	const sf::Vector2f		mFrameBuffer;
	float					mHorizontalBuffer;

	bool					mDisable;
	bool					mHide;

	sf::Vector2f			mRestoreBackgroundSize;

	UIElemVec				mUIElems;
	sf::RectangleShape		mBackground;
};

void	centerOrigin(UndoUI &undoUI, bool centerXAxis = true, bool centerYAxis = true);

#endif
