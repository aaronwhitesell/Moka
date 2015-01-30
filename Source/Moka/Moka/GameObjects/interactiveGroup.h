#ifndef INTERACTIVE_GROUP_H
#define INTERACTIVE_GROUP_H

#include "interactiveObject.h"

#include <string>
#include <vector>


class InteractiveGroup
{
public:
									InteractiveGroup();
									InteractiveGroup(const InteractiveGroup &) = delete;
	InteractiveGroup &				operator=(const InteractiveGroup &) = delete;

	const std::vector<InteractiveObject> &	getInteractiveObjects() const;

	int								getWidth() const;
	int								getHeight() const;

	void							setName(const std::string &name);
	void							setWidth(int width);
	void							setHeight(int height);
	void							setInteractiveObjects(const std::vector<InteractiveObject> &interactiveObjects);


private:
	std::string						mName;
	int								mWidth;
	int								mHeight;

	std::vector<InteractiveObject>	mInteractiveObjects;
};

#endif
