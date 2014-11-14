#include "clinicNode.h"
#include "../GameObjects/interactiveObject.h"
#include "../HUD/chatBox.h"
#include "../HUD/optionsUI.h"
#include "../Resources/resourceIdentifiers.h"

#include "Trambo/Localize/localize.h"
#include "Trambo/Sounds/soundPlayer.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


ClinicNode::ClinicNode(const sf::RenderWindow &window, const sf::View &view, const InteractiveObject &interactiveObject
	, std::vector<sf::FloatRect> attachedRects, trmb::SoundPlayer &soundPlayer, ChatBox &chatBox, OptionsUI &optionsUI)
: BuildingNode(window, view, interactiveObject, attachedRects, optionsUI)
, mSoundPlayer(soundPlayer)
, mChatBox(chatBox)
{
}

void ClinicNode::activate()
{
	updateOptionsUI();
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
	mChatBox.updateText(trmb::Localize::getInstance().getString("inspectClinic"));
	mSelected = true;
}

void ClinicNode::updateOptionsUI()
{
	const float verticalBuffer = 10.0f;
	mOptionsUI.setPosition(sf::Vector2f(mInteractiveObject.getX() + mInteractiveObject.getWidth() / 2.0f
		, mInteractiveObject.getY() + mInteractiveObject.getHeight() + verticalBuffer));

	mOptionsUI.updateIncDecCallbacks(std::bind(&ClinicNode::incrementPurchaseRDTClick, this)
		, std::bind(&ClinicNode::decrementPurchaseRDTClick, this)
		, std::bind(&ClinicNode::incrementPurchaseACTClick, this)
		, std::bind(&ClinicNode::decrementPurchaseACTClick, this));
}

void ClinicNode::incrementPurchaseRDTClick()
{
	// ALW - TODO - Increment resource
}

void ClinicNode::decrementPurchaseRDTClick()
{
	// ALW - TODO - Decrement resource
}

void ClinicNode::incrementPurchaseACTClick()
{
	// ALW - TODO - Increment resource
}

void ClinicNode::decrementPurchaseACTClick()
{
	// ALW - TODO - Decrement resource
}
