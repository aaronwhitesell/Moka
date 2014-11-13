#include "houseUI.h"
#include "../Resources/resourceIdentifiers.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <cassert>
#include <functional>


HouseUI::HouseUI(Fonts::ID font, trmb::FontHolder &fonts, SoundEffects::ID soundEffect, trmb::SoundPlayer &soundPlayer,
	EventGuid leftClickPress, EventGuid leftClickRelease)
: mOptionTabs(leftClickPress)
, mIncDecUI(font, fonts, soundEffect, soundPlayer, leftClickPress, leftClickRelease, sf::Vector2f(20.0f, 20.0f))
, mHorizontalBuffer(1.0f)
, mVerticalBuffer(1.0f)
, mDrawIncDecUI(false)
{
	// ALW - This UI has one one intended purpose. It is not reuseable, so I have hardcoded values here for simplicity of use.
	const float outLineThickness = 1.0f; // ALW - Ignored when calculating sizes
	const sf::Vector2f optionTabSize = sf::Vector2f(75.0f, 20.0f);
	const sf::Vector2f frameBuffer = sf::Vector2f(7.0f, 7.0f);

	// ALW - Set up buttons before background, because the background size depends on the number of tabs in mOptionsTab.
	mPurchaseTab = std::make_shared<trmb::GameTab>(Fonts::ID::Main, fonts, soundEffect, soundPlayer, optionTabSize);
	mPurchaseTab->setPosition(frameBuffer.x + 0.0f, frameBuffer.y + 0.0f);
	mPurchaseTab->setText("Purchase");
	mPurchaseTab->setCallback(std::bind(&HouseUI::purchase, this));

	mRepairTab = std::make_shared<trmb::GameTab>(Fonts::ID::Main, fonts, soundEffect, soundPlayer, optionTabSize);
	mRepairTab->setPosition(frameBuffer.x + mPurchaseTab->getSize().x + mHorizontalBuffer, frameBuffer.y + 0.0f);
	mRepairTab->setText("Repair");
	mRepairTab->setCallback(std::bind(&HouseUI::repair, this));

	mOptionTabs.pack(mPurchaseTab);
	mOptionTabs.pack(mRepairTab);
	mOptionTabs.setPosition(0.0f, 0.0f);
	mOptionTabs.setVisualScheme(sf::Color(112u, 128u, 144u, 255u), sf::Color(255u, 255u, 255u, 255u), sf::Color(0u, 0u, 0u, 255u)
		, sf::Color(95u, 158u, 160u, 255u), sf::Color(255u, 255u, 255u, 255u), sf::Color(0u, 0u, 0u, 255u)        // Depress
		, sf::Color(160u, 160u, 160u, 255u), sf::Color(224u, 224u, 224u, 255u), sf::Color(128u, 128u, 128u, 255u) // Disable
		, outLineThickness);

	// ALW - Setup background before inc/dec UI, because the position of the in/dec UI depends on the size of the background.
	const int tabCount = mOptionTabs.getSize();
//	const float backgroundWidth = optionTabSize.x * tabCount + mHorizontalBuffer * (tabCount - 1) + frameBuffer.x * 2.0f;
//	const float backgroundHeight = optionTabSize.y + frameBuffer.y * 2.0f;
	mBackgroundDefaultSize = sf::Vector2f(optionTabSize.x * tabCount + mHorizontalBuffer * (tabCount - 1) + frameBuffer.x * 2.0f
		, optionTabSize.y + frameBuffer.y * 2.0f);
	mBackgroundExpandedSize = sf::Vector2f(mBackgroundDefaultSize.x, mBackgroundDefaultSize.y + mVerticalBuffer + mIncDecUI.getSize().y);

	mBackground.setSize(mBackgroundDefaultSize);
	mBackground.setPosition(0.0f, 0.0f);
	mBackground.setFillColor(sf::Color(0u, 0u, 0u, 200u));
	mBackground.setOutlineColor(sf::Color(0u, 0u, 0u, 255u));
	mBackground.setOutlineThickness(outLineThickness);

	trmb::centerOrigin(mIncDecUI, true, false);
	assert(("ALW - Logic Error: The increment button's height is not the same as the decrement button's height!"
		, mPurchaseTab->getSize().y == mRepairTab->getSize().y));
	mIncDecUI.setPosition(mBackground.getSize().x / 2.0f, frameBuffer.y + mPurchaseTab->getSize().y + mVerticalBuffer);
	mIncDecUI.setVisualScheme(sf::Color(95u, 158u, 160u, 255u), sf::Color(255u, 255u, 255u, 255u), sf::Color(0u, 0u, 0u, 255u)
		, sf::Color(162u, 181u, 205u, 255u), sf::Color(255u, 255u, 255u, 255u), sf::Color(0u, 0u, 0u, 255u)       // Hover
		, sf::Color(88u, 146u, 148u, 255u), sf::Color(255u, 255u, 255u, 255u), sf::Color(0u, 0u, 0u, 255u)        // Depress
		, sf::Color(160u, 160u, 160u, 255u), sf::Color(224u, 224u, 224u, 255u), sf::Color(128u, 128u, 128u, 255u) //Disablle
		, outLineThickness);
}

sf::Vector2f HouseUI::getSize() const
{
	return mBackground.getSize();
}

sf::FloatRect HouseUI::getRect() const
{
	const sf::Vector2f backgroundPosition = mBackground.getPosition();
	const sf::Vector2f backgroundSize = mBackground.getSize();
	sf::FloatRect rect = sf::FloatRect(backgroundPosition.x, backgroundPosition.y, backgroundSize.x, backgroundSize.y);
	rect = getTransform().transformRect(rect);

	return rect;
}

void HouseUI::setPurchaseCallback(Callback callback)
{
	mPurchaseTab->setCallback(callback);
}

void HouseUI::setRepairCallback(Callback callback)
{
	mPurchaseTab->setCallback(callback);
}

void HouseUI::handler(const sf::RenderWindow &window, const sf::View &view, const sf::Transform &transform)
{
	sf::Transform combinedTransform = getTransform() * transform;

	mOptionTabs.handler(window, view, combinedTransform);
	mIncDecUI.handler(window, view, combinedTransform);
}

void HouseUI::updateIncDecCallbacks(Callback incPurchaseCallback, Callback decPurchaseCallback
	, Callback incRepairCallback, Callback decRepairCallback)
{
	mIncPurchaseCallback = incPurchaseCallback;
	mDecPurchaseCallback = decPurchaseCallback;
	mIncRepairCallback = incRepairCallback;
	mDecRepairCallback = decRepairCallback;
}

void HouseUI::deactivate()
{
	// ALW - Resize background
	mBackground.setSize(mBackgroundDefaultSize);

	// ALW - Deactivate tabs
	mOptionTabs.deactivate();

	// ALW - Deactivate IncDec UI
	mDrawIncDecUI = false;
	mIncDecUI.setCallbacks(nullptr, nullptr);
}

void HouseUI::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(mBackground, states);
	target.draw(mOptionTabs, states);
	if (mDrawIncDecUI)
		target.draw(mIncDecUI, states);
}

void HouseUI::purchase()
{
	// ALW - Resize background
	mBackground.setSize(mBackgroundExpandedSize);

	// ALW - Activate IncDec UI
	mDrawIncDecUI = true;
	mIncDecUI.setCallbacks(mIncPurchaseCallback, mDecPurchaseCallback);
}

void HouseUI::repair()
{
	// ALW - Resize background
	mBackground.setSize(mBackgroundExpandedSize);

	// ALW - Activate IncDec UI
	mDrawIncDecUI = true;
	mIncDecUI.setCallbacks(mIncRepairCallback, mDecRepairCallback);
}

void centerOrigin(HouseUI &button, bool centerXAxis, bool centerYAxis)
{
	sf::Vector2f size = button.getSize();
	float xAxis = 0.0f;
	float yAxis = 0.0f;

	if (centerXAxis)
		xAxis = std::floor(size.x / 2.0f);

	if (centerYAxis)
		yAxis = std::floor(size.y / 2.0f);

	button.setOrigin(xAxis, yAxis);
}
