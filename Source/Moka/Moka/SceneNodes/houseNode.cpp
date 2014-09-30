#include "HouseNode.h"
#include "../GameObjects/interactiveObject.h"
#include "../Resources/resourceIdentifiers.h"

#include "Trambo/Events/event.h"
#include "Trambo/Sounds/soundPlayer.h"

#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


HouseNode::HouseNode(sf::RenderWindow &window, const sf::View &view, const InteractiveObject &interactiveObject
	, std::vector<sf::IntRect> attachedRects, trmb::SoundPlayer &soundPlayer)
: mWindow(window)
, mView(view)
, mInteractiveObject(interactiveObject)
, mAttachedRects(attachedRects)
, mSoundPlayer(soundPlayer)
{
	mHightlight.setSize(static_cast<sf::Vector2f>(sf::Vector2i(mInteractiveObject.getWidth(), mInteractiveObject.getHeight())));
	mHightlight.setPosition(static_cast<sf::Vector2f>(sf::Vector2i(mInteractiveObject.getX(), mInteractiveObject.getY())));
}

void HouseNode::updateSelection()
{
	const sf::Vector2i relativeToWindow = sf::Mouse::getPosition(mWindow);
	const sf::Vector2f relativeToWorld = mWindow.mapPixelToCoords(relativeToWindow, mView);
	const sf::Vector2f MousePosition(relativeToWorld);
	const sf::FloatRect interactiveObjRect(static_cast<sf::FloatRect>(sf::IntRect(mInteractiveObject.getX(),
		mInteractiveObject.getY(), mInteractiveObject.getWidth(), mInteractiveObject.getHeight())));

	// ALW - If the cursor click is on a house select it
	if (interactiveObjRect.contains(MousePosition))
		mIsSelected = true;
	else
		mIsSelected = false;

	std::vector<sf::IntRect>::const_iterator iter    = begin(mAttachedRects);
	std::vector<sf::IntRect>::const_iterator iterEnd = end(mAttachedRects);

	// ALW - If the cursor click is on an object attached to the house then the house is unselected
	for (; iter != iterEnd; ++iter)
	{
		if (static_cast<sf::FloatRect>(*iter).contains(MousePosition))
			mIsSelected = false;
	}

	if (mIsSelected)
		mSoundPlayer.play(SoundEffects::ID::Button);
}
