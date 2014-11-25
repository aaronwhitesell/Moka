#include "uiBundle.h"


UIBundle::UIBundle(UndoUI &barrelUI, UndoUI &windowUI, OptionsUI &clinicUI, OptionsUI &houseUI)
: mBarrelUI(barrelUI)
, mWindowUI(windowUI)
, mClinicUI(clinicUI)
, mHouseUI(houseUI)
{
}

UndoUI & UIBundle::getBarrelUI() const
{
	return mBarrelUI;
}

UndoUI & UIBundle::getWindowUI() const
{
	return mWindowUI;
}

OptionsUI & UIBundle::getClinicUI() const
{
	return mClinicUI;
}

OptionsUI & UIBundle::getHouseUI() const
{
	return mHouseUI;
}
