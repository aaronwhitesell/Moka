#ifndef OBJECTS_GROUPS_H
#define OBJECTS_GROUPS_H

#include "preventionGroup.h"

#include "../../../3rdParty/TinyXML2/tinyxml2.h"

#include <string>


class ObjectGroups
{
public:
	explicit							ObjectGroups(const std::string &filename);
										ObjectGroups(const ObjectGroups &) = delete;
	ObjectGroups &						operator=(const ObjectGroups &) = delete;

	const PreventionGroup &				getPreventionGroup() const;


private:
	void								read(const std::string &filename);
	void								loadFile(const std::string &filename, tinyxml2::XMLDocument &config);
	void								readPreventionGroup(tinyxml2::XMLDocument &config);


private:
	PreventionGroup						mPreventionGroup;
};

#endif
