#ifndef UI_BUNDLE_H
#define UI_BUNDLE_H


class OptionsUI;
class UndoUI;

class UIBundle
{
public:
						UIBundle(UndoUI &barrelUI, UndoUI &doorUI, UndoUI &windowUI, OptionsUI &clinicUI, OptionsUI &houseUI);
						UIBundle(const UIBundle &) = delete;
	UIBundle &			operator=(const UIBundle &) = delete;

	UndoUI &			getBarrelUI() const;
	UndoUI &			getDoorUI() const;
	UndoUI &			getWindowUI() const;
	OptionsUI &			getClinicUI() const;
	OptionsUI &			getHouseUI() const;


private:
	UndoUI				&mBarrelUI;
	UndoUI				&mDoorUI;
	UndoUI				&mWindowUI;
	OptionsUI			&mClinicUI;
	OptionsUI			&mHouseUI;
};

#endif
