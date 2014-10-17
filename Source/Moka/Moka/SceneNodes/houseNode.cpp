#include "HouseNode.h"
#include "../GameObjects/interactiveObject.h"
#include "../HUD/chatBox.h"
#include "../Resources/resourceIdentifiers.h"

#include "Trambo/Events/event.h"
#include "Trambo/Localize/localize.h"
#include "Trambo/Sounds/soundPlayer.h"

#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


HouseNode::HouseNode(sf::RenderWindow &window, const sf::View &view, const InteractiveObject &interactiveObject
	, std::vector<sf::FloatRect> attachedRects, trmb::SoundPlayer &soundPlayer, ChatBox &chatBox)
: mWindow(window)
, mView(view)
, mInteractiveObject(interactiveObject)
, mAttachedRects(attachedRects)
, mSoundPlayer(soundPlayer)
, mChatBox(chatBox)
{
	mHightlight.setSize(sf::Vector2f(mInteractiveObject.getWidth(), mInteractiveObject.getHeight()));
	mHightlight.setPosition(sf::Vector2f(mInteractiveObject.getX(), mInteractiveObject.getY()));
}

void HouseNode::updateSelection()
{
	const sf::Vector2i relativeToWindow = sf::Mouse::getPosition(mWindow);
	const sf::Vector2f relativeToWorld = mWindow.mapPixelToCoords(relativeToWindow, mView);
	const sf::Vector2f mousePosition = relativeToWorld;
	const sf::FloatRect interactiveObjRect = sf::FloatRect(mInteractiveObject.getX(),
		mInteractiveObject.getY(), mInteractiveObject.getWidth(), mInteractiveObject.getHeight());

	// ALW - If the cursor click is on a house select it
	if (interactiveObjRect.contains(mousePosition))
		mIsSelected = true;
	else
		mIsSelected = false;

	std::vector<sf::FloatRect>::const_iterator iter = begin(mAttachedRects);
	for (; iter != end(mAttachedRects); ++iter)
	{
		// ALW - If the cursor click is on an object attached to the house then the house is unselected
		if (iter->contains(mousePosition))
			mIsSelected = false;
	}

	if (mIsSelected)
	{
		mSoundPlayer.play(SoundEffects::ID::Button);
		// ALW - ChatBox::UpdateText() can generate a mCreatePrompt event when an interactive object
		// ALW - is selected. This asynchronous event will force InteractiveNode classes to ignore
		// ALW - left and right click events. Then if <enter> is pressed an mEnter event will be
		// ALW - generated allowing InteractiveNode classes to handle left and right click events.
		// ALW - However, if another interactive object is selected that occurs before the currently
		// ALW - selected interactive object in the SceneNode then this newly selected interactive
		// ALW - object will cause ChatBox::UpdateText() to be called which generates a mCreatePrompt.
		// ALW - InteractiveNodes are then forced to ignore left and right click events, so the original
		// ALW - interactive object will be left selected. To remedy this all InteractiveNodes deselect
		// ALW - themselves when a mCreatePrompt is generated. Immediately afterwards the InteractiveNode
		// ALW - that generated the mCreatePrompt is reselected.
		mChatBox.updateText(trmb::Localize::getInstance().getString("inspectHouse"));
		mIsSelected = true;
	}
}
