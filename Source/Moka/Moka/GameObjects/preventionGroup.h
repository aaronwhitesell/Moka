#ifndef PREVENTION_GROUP_H
#define PREVENTION_GROUP_H

#include "preventionObject.h"

#include <string>
#include <vector>


class PreventionGroup
{
public:
									PreventionGroup();
									PreventionGroup(const PreventionGroup &) = delete;
	PreventionGroup &				operator=(const PreventionGroup &) = delete;

	const std::vector<PreventionObject> &	getPreventionObjects() const;

	void							setName(const std::string &name);
	void							setWidth(int width);
	void							setHeight(int height);
	void							setPreventionObjects(const std::vector<PreventionObject> &preventionObjects);


private:
	std::string						mName;
	int								mWidth;
	int								mHeight;

	std::vector<PreventionObject>	mPreventionObjects;
};

#endif
