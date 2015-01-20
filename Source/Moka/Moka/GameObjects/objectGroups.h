#ifndef OBJECTS_GROUPS_H
#define OBJECTS_GROUPS_H

#include "interactiveGroup.h"

#include "../../../3rdParty/TinyXML2/tinyxml2.h"

#include <string>


class ObjectGroups
{
public:
	explicit							ObjectGroups(const std::string &filename);
										ObjectGroups(const ObjectGroups &) = delete;
	ObjectGroups &						operator=(const ObjectGroups &) = delete;

	const InteractiveGroup &			getInteractiveGroup() const;


private:
	void								read(const std::string &filename);
	void								loadFile(const std::string &filename, tinyxml2::XMLDocument &config);
	void								readInteractiveGroup(tinyxml2::XMLDocument &config);
	InteractiveObject                   readInteractiveObject(tinyxml2::XMLElement *element) const;

	// ALW - Object attributes
	float							    readInteractiveObjectXCoord(tinyxml2::XMLElement * const element) const;
	float   							readInteractiveObjectYCoord(tinyxml2::XMLElement * const element) const;
	float    							readInteractiveObjectWidth(tinyxml2::XMLElement * const element) const;
	float							    readInteractiveObjectHeight(tinyxml2::XMLElement * const element) const;

	std::string							readInteractiveObjectName(tinyxml2::XMLElement * const element) const;
	std::string							readInteractiveObjectType(tinyxml2::XMLElement * const element) const;

	// ALW - Objects properties
	int									readInteractiveObjectBeds(tinyxml2::XMLElement * element) const;
	int     							readInteractiveObjectNets(tinyxml2::XMLElement * element) const;
	int									readInteractiveObjectResidents(tinyxml2::XMLElement * element) const;

	float							    readInteractiveObjectAlternateXCoord1(tinyxml2::XMLElement * element) const;
	float							    readInteractiveObjectAlternateYCoord1(tinyxml2::XMLElement * element) const;
	float							    readInteractiveObjectAlternateXCoord2(tinyxml2::XMLElement * element) const;
	float							    readInteractiveObjectAlternateYCoord2(tinyxml2::XMLElement * element) const;
	float							    readInteractiveObjectAlternateXCoord3(tinyxml2::XMLElement * element) const;
	float							    readInteractiveObjectAlternateYCoord3(tinyxml2::XMLElement * element) const;
	float							    readInteractiveObjectCollisionBoxXCoord(tinyxml2::XMLElement * element) const;
	float   							readInteractiveObjectCollisionBoxYCoord(tinyxml2::XMLElement * element) const;
	float    							readInteractiveObjectCollisionBoxWidth(tinyxml2::XMLElement * element) const;
	float							    readInteractiveObjectCollisionBoxHeight(tinyxml2::XMLElement * element) const;

	std::string							readInteractiveObjectAttachedTo(tinyxml2::XMLElement * element) const;
	std::string							readInteractiveObjectColor(tinyxml2::XMLElement * element) const;
	std::string							readInteractiveObjectStyle(tinyxml2::XMLElement * element) const;


private:
	InteractiveGroup					mInteractiveGroup;
};

#endif
