#include "uiBundle.h"


UIBundle::UIBundle(UndoUI &barrelUI, OptionsUI &clinicUI, OptionsUI &houseUI)
: mBarrelUI(barrelUI)
, mClinicUI(clinicUI)
, mHouseUI(houseUI)
{
}

UndoUI & UIBundle::getBarrelUI() const
{
	return mBarrelUI;
}

OptionsUI & UIBundle::getClinicUI() const
{
	return mClinicUI;
}

OptionsUI & UIBundle::getHouseUI() const
{
	return mHouseUI;
}
