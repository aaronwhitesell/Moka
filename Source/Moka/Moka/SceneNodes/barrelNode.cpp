#include "barrelNode.h"
#include "../GameObjects/interactiveObject.h"
#include "../HUD/chatBox.h"
#include "../Resources/resourceIdentifiers.h"

#include "Trambo/Localize/localize.h"
#include "Trambo/Sounds/soundPlayer.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


BarrelNode::BarrelNode(const InteractiveObject &interactiveObject, const sf::RenderWindow &window, const sf::View &view, UndoUI &undoUI
	, trmb::SoundPlayer &soundPlayer, ChatBox &chatBox)
: PreventionNode(interactiveObject, window, view, undoUI)
, mSoundPlayer(soundPlayer)
, mChatBox(chatBox)
{
	mCallbackPairs.emplace_back(CallbackPair(std::bind(&BarrelNode::lidClick, this), std::bind(&BarrelNode::undoClick, this)));
	mUIElemStates.emplace_back(true);
}

void BarrelNode::activate()
{
	updateUndoUI();	
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
	mChatBox.updateText(trmb::Localize::getInstance().getString("inspectBarrel"));
	mSelected = true;
}

void BarrelNode::updateUndoUI()
{
	const float verticalBuffer = 10.0f;
	mUndoUI.setPosition(sf::Vector2f(mInteractiveObject.getX() + mInteractiveObject.getWidth() / 2.0f
		, mInteractiveObject.getY() + mInteractiveObject.getHeight() + verticalBuffer));

	mUndoUI.setCallbacks(mCallbackPairs);

	mUndoUI.setUIElemState(mUIElemStates);
}

void BarrelNode::lidClick()
{
	mUIElemStates.front() = false;
	// ALW - TODO - Add lid
}

void BarrelNode::undoClick()
{
	mUIElemStates.front() = true;
	// ALW - TODO - Remove lid
}
