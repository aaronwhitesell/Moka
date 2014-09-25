#include "preventionGroup.h"


PreventionGroup::PreventionGroup()
{
}

const std::vector<PreventionObject> & PreventionGroup::getPreventionObjects() const
{
	return mPreventionObjects;
}

void PreventionGroup::setName(const std::string &name)
{
	mName = name;
}

void PreventionGroup::setWidth(int width)
{
	mWidth = width;
}

void PreventionGroup::setHeight(int height)
{
	mHeight = height;
}

void PreventionGroup::setPreventionObjects(const std::vector<PreventionObject> &preventionObjects)
{
	mPreventionObjects = preventionObjects;
}
