#ifndef UI_BUNDLE_H
#define UI_BUNDLE_H


class OptionsUI;
class UndoUI;

class UIBundle
{
public:
						UIBundle(UndoUI &barrelUI, OptionsUI &clinicUI, OptionsUI &houseUI);
						UIBundle(const UIBundle &) = delete;
	UIBundle &			operator=(const UIBundle &) = delete;

	UndoUI &			getBarrelUI() const;
	OptionsUI &			getClinicUI() const;
	OptionsUI &			getHouseUI() const;


private:
	UndoUI				&mBarrelUI;
	OptionsUI			&mClinicUI;
	OptionsUI			&mHouseUI;
};

#endif
