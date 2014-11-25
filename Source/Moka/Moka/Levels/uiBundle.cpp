#include "uiBundle.h"


UIBundle::UIBundle(UndoUI &barrelUI, UndoUI &doorUI, UndoUI &windowUI, OptionsUI &clinicUI, OptionsUI &houseUI)
: mBarrelUI(barrelUI)
, mDoorUI(doorUI)
, mWindowUI(windowUI)
, mClinicUI(clinicUI)
, mHouseUI(houseUI)
{
}

UndoUI & UIBundle::getBarrelUI() const
{
	return mBarrelUI;
}

UndoUI & UIBundle::getDoorUI() const
{
	return mDoorUI;
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
