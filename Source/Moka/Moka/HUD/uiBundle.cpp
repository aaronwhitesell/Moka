#include "uiBundle.h"


UIBundle::UIBundle(DaylightUI &daylightUI, UndoUI &barrelUI, UndoUI &doorUI, UndoUI &windowUI, OptionsUI &clinicUI, OptionsUI &houseUI)
: mDaylightUI(daylightUI)
, mBarrelUI(barrelUI)
, mDoorUI(doorUI)
, mWindowUI(windowUI)
, mClinicUI(clinicUI)
, mHouseUI(houseUI)
{
}

DaylightUI & UIBundle::getDaylightUI() const
{
	return mDaylightUI;
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
