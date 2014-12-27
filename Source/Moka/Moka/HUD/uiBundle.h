#ifndef UI_BUNDLE_H
#define UI_BUNDLE_H


class ChatBoxUI;
class DaylightUI;
class OptionsUI;
class UndoUI;

class UIBundle
{
public:
						UIBundle(ChatBoxUI &chatboxUI, DaylightUI &daylightUI, UndoUI &barrelUI, UndoUI &doorUI, UndoUI &windowUI
							, OptionsUI &clinicUI, OptionsUI &houseUI);
						UIBundle(const UIBundle &) = delete;
	UIBundle &			operator=(const UIBundle &) = delete;

	ChatBoxUI &         getChatBoxUI() const;
	DaylightUI &        getDaylightUI() const;
	UndoUI &			getBarrelUI() const;
	UndoUI &			getDoorUI() const;
	UndoUI &			getWindowUI() const;
	OptionsUI &			getClinicUI() const;
	OptionsUI &			getHouseUI() const;


private:
	ChatBoxUI			&mChatBoxUI;
	DaylightUI			&mDaylightUI;
	UndoUI				&mBarrelUI;
	UndoUI				&mDoorUI;
	UndoUI				&mWindowUI;
	OptionsUI			&mClinicUI;
	OptionsUI			&mHouseUI;
};

#endif
