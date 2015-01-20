#include "objectGroups.h"

#include <cassert>
#include <vector>
#include <stdexcept>


ObjectGroups::ObjectGroups(const std::string &filename)
{
	read(filename);
}

const InteractiveGroup & ObjectGroups::getInteractiveGroup() const
{
	return mInteractiveGroup;
}

void ObjectGroups::read(const std::string &filename)
{
	tinyxml2::XMLDocument config;
	loadFile(filename, config);
	readInteractiveGroup(config);
}

void ObjectGroups::loadFile(const std::string &filename, tinyxml2::XMLDocument &config)
{
	tinyxml2::XMLError eResult = config.LoadFile(filename.c_str());

	if (eResult != tinyxml2::XML_NO_ERROR)
	{
		throw std::runtime_error("ALW - Runtime Error: Failed to load " + filename);
	}
}

void ObjectGroups::readInteractiveGroup(tinyxml2::XMLDocument &config)
{
	tinyxml2::XMLElement *element = config.FirstChildElement("map");
	if (element == nullptr)
	{
		throw std::runtime_error("ALW - Runtime Error: Failed to read map element.");
	}

	// ALW - Find the Interactive objectgroup
	element = element->FirstChildElement("objectgroup");
	if (element == nullptr)
	{
		throw std::runtime_error("ALW - Runtime Error: There are no objectgroup elements.");
	}

	std::string objectGroupName = "Interactive";
	while (objectGroupName != element->Attribute("name"))
	{
		element = element->NextSiblingElement("objectgroup");
		if (element == nullptr)
		{
			throw std::runtime_error("ALW - Runtime Error: Failed to read objectgroup element.");
		}
	}

	// ALW - Read Interactive objectgroup's attributes
	mInteractiveGroup.setName(static_cast<std::string>(element->Attribute("name")));

	int width;
	tinyxml2::XMLError eResult = element->QueryIntAttribute("width", &width);
	if (eResult != tinyxml2::XML_NO_ERROR)
	{
		throw std::runtime_error("ALW - Runtime Error: Unable to convert width attribute");
	}
	mInteractiveGroup.setWidth(width);

	int height;
	eResult = element->QueryIntAttribute("height", &height);
	if (eResult != tinyxml2::XML_NO_ERROR)
	{
		throw std::runtime_error("ALW - Runtime Error: Unable to convert height attribute");
	}
	mInteractiveGroup.setHeight(height);

	// ALW - Read Interactive objectgroup's objects
	std::vector<InteractiveObject> interactiveObjects;

	element = element->FirstChildElement("object");
	if (element == nullptr)
	{
		throw std::runtime_error("ALW - Runtime Error: There are no object elements.");
	}

	for (; element != nullptr; element = element->NextSiblingElement("object"))
	{
		interactiveObjects.push_back(readInteractiveObject(element));
	}

	mInteractiveGroup.setInteractiveObjects(interactiveObjects);
}

InteractiveObject ObjectGroups::readInteractiveObject(tinyxml2::XMLElement * element) const
{
	// ALW - Read each object's attributes
	std::string name = readInteractiveObjectName(element);
	std::string type = readInteractiveObjectType(element);
	float x          = readInteractiveObjectXCoord(element);
	float y          = readInteractiveObjectYCoord(element);
	float width      = readInteractiveObjectWidth(element);
	float height     = readInteractiveObjectHeight(element);

	// ALW - Read each object's properties
	element = element->FirstChildElement("properties");
	if (element == nullptr)
	{
		throw std::runtime_error("ALW - Runtime Error: There are no properties elements.");
	}

	float alternateXCoord1 = readInteractiveObjectAlternateXCoord1(element);
	float alternateXCoord2 = readInteractiveObjectAlternateXCoord2(element);
	float alternateXCoord3 = readInteractiveObjectAlternateXCoord3(element);
	float alternateYCoord1 = readInteractiveObjectAlternateYCoord1(element);
	float alternateYCoord2 = readInteractiveObjectAlternateYCoord2(element);
	float alternateYCoord3 = readInteractiveObjectAlternateYCoord3(element);
	std::string attachedTo = readInteractiveObjectAttachedTo(element);
	int beds = readInteractiveObjectBeds(element);
	float collisionBoxHeight = readInteractiveObjectCollisionBoxHeight(element);
	float collisionBoxWidth = readInteractiveObjectCollisionBoxWidth(element);
	float collisionBoxXCoord = readInteractiveObjectCollisionBoxXCoord(element);
	float collisionBoxYCoord = readInteractiveObjectCollisionBoxYCoord(element);
	std::string color = readInteractiveObjectColor(element);
	int nets = readInteractiveObjectNets(element);
	int residents = readInteractiveObjectResidents(element);
	std::string style = readInteractiveObjectStyle(element);

	return InteractiveObject(name, type, x, y, width, height, alternateXCoord1, alternateXCoord2, alternateXCoord3
		, alternateYCoord1, alternateYCoord2, alternateYCoord3, attachedTo, beds, collisionBoxHeight, collisionBoxWidth
		, collisionBoxXCoord, collisionBoxYCoord, color, nets, residents, style);
}

float ObjectGroups::readInteractiveObjectXCoord(tinyxml2::XMLElement * const element) const
{
	float x; // ALW - Read int as float (reduces number of casts when used).
	tinyxml2::XMLError eResult = element->QueryFloatAttribute("x", &x);
	if (eResult != tinyxml2::XML_NO_ERROR)
	{
		throw std::runtime_error("ALW - Runtime Error: Unable to convert x attribute");
	}

	return x;
}

float ObjectGroups::readInteractiveObjectYCoord(tinyxml2::XMLElement * const element) const
{
	float y; // ALW - Read int as float (reduces number of casts when used).
	tinyxml2::XMLError eResult = element->QueryFloatAttribute("y", &y);
	if (eResult != tinyxml2::XML_NO_ERROR)
	{
		throw std::runtime_error("ALW - Runtime Error: Unable to convert y attribute");
	}

	return y;
}

float ObjectGroups::readInteractiveObjectWidth(tinyxml2::XMLElement * const element) const
{
	float width;
	tinyxml2::XMLError eResult = element->QueryFloatAttribute("width", &width);
	if (eResult != tinyxml2::XML_NO_ERROR)
	{
		throw std::runtime_error("ALW - Runtime Error: Unable to convert width attribute");
	}

	return width;
}

float ObjectGroups::readInteractiveObjectHeight(tinyxml2::XMLElement * const element) const
{
	float height;
	tinyxml2::XMLError eResult = element->QueryFloatAttribute("height", &height);
	if (eResult != tinyxml2::XML_NO_ERROR)
	{
		throw std::runtime_error("ALW - Runtime Error: Unable to convert height attribute");
	}

	return height;
}

std::string ObjectGroups::readInteractiveObjectName(tinyxml2::XMLElement * const element) const
{
	return element->Attribute("name");
}

std::string ObjectGroups::readInteractiveObjectType(tinyxml2::XMLElement * const element) const
{
	return element->Attribute("type");
}

int ObjectGroups::readInteractiveObjectBeds(tinyxml2::XMLElement * element) const
{
	// ALW - Property Beds
	element = element->FirstChildElement("property");
	if (element == nullptr)
	{
		throw std::runtime_error("ALW - Runtime Error: There are no property elements.");
	}

	std::string bedsName = "Beds";
	while (bedsName != element->Attribute("name"))
	{
		element = element->NextSiblingElement("property");
		if (element == nullptr)
		{
			throw std::runtime_error("ALW - Runtime Error: Failed to find Beds property element.");
		}
	}

	int bedsValue;
	tinyxml2::XMLError eResult = element->QueryIntAttribute("value", &bedsValue);
	if (eResult != tinyxml2::XML_NO_ERROR)
	{
		throw std::runtime_error("ALW - Runtime Error: Unable to convert Beds attribute.");
	}

	return bedsValue;
}

int ObjectGroups::readInteractiveObjectNets(tinyxml2::XMLElement * element) const
{
	// ALW - Property Nets
	element = element->FirstChildElement("property");
	if (element == nullptr)
	{
		throw std::runtime_error("ALW - Runtime Error: There are no property elements.");
	}

	std::string netsName = "Nets";
	while (netsName != element->Attribute("name"))
	{
		element = element->NextSiblingElement("property");
		if (element == nullptr)
		{
			throw std::runtime_error("ALW - Runtime Error: Failed to find Nets property element.");
		}
	}

	int netsValue;
	tinyxml2::XMLError eResult = element->QueryIntAttribute("value", &netsValue);
	if (eResult != tinyxml2::XML_NO_ERROR)
	{
		throw std::runtime_error("ALW - Runtime Error: Unable to convert Nets attribute.");
	}

	return netsValue;
}

int ObjectGroups::readInteractiveObjectResidents(tinyxml2::XMLElement * element) const
{
	// ALW - Property Residents
	element = element->FirstChildElement("property");
	if (element == nullptr)
	{
		throw std::runtime_error("ALW - Runtime Error: There are no property elements.");
	}

	std::string residentsName = "Residents";
	while (residentsName != element->Attribute("name"))
	{
		element = element->NextSiblingElement("property");
		if (element == nullptr)
		{
			throw std::runtime_error("ALW - Runtime Error: Failed to find Residents property element.");
		}
	}

	int residentsValue;
	tinyxml2::XMLError eResult = element->QueryIntAttribute("value", &residentsValue);
	if (eResult != tinyxml2::XML_NO_ERROR)
	{
		throw std::runtime_error("ALW - Runtime Error: Unable to convert Residents attribute.");
	}

	return residentsValue;
}

float ObjectGroups::readInteractiveObjectAlternateXCoord1(tinyxml2::XMLElement * element) const
{
	// ALW - Property AlternateXCoord1
	element = element->FirstChildElement("property");
	if (element == nullptr)
	{
		throw std::runtime_error("ALW - Runtime Error: There are no property elements.");
	}

	std::string alternateXCoord1Name = "AlternateXCoord1";
	while (alternateXCoord1Name != element->Attribute("name"))
	{
		element = element->NextSiblingElement("property");
		if (element == nullptr)
		{
			throw std::runtime_error("ALW - Runtime Error: Failed to find AlternateXCoord1 property element.");
		}
	}

	float alternateXCoord1Value;
	tinyxml2::XMLError eResult = element->QueryFloatAttribute("value", &alternateXCoord1Value);
	if (eResult != tinyxml2::XML_NO_ERROR)
	{
		throw std::runtime_error("ALW - Runtime Error: Unable to convert AlternateXCoord1 attribute.");
	}

	return alternateXCoord1Value;
}

float ObjectGroups::readInteractiveObjectAlternateYCoord1(tinyxml2::XMLElement * element) const
{
	// ALW - Property AlternateYCoord1
	element = element->FirstChildElement("property");
	if (element == nullptr)
	{
		throw std::runtime_error("ALW - Runtime Error: There are no property elements.");
	}

	std::string alternateYCoord1Name = "AlternateYCoord1";
	while (alternateYCoord1Name != element->Attribute("name"))
	{
		element = element->NextSiblingElement("property");
		if (element == nullptr)
		{
			throw std::runtime_error("ALW - Runtime Error: Failed to find AlternateYCoord1 property element.");
		}
	}

	float alternateYCoord1Value;
	tinyxml2::XMLError eResult = element->QueryFloatAttribute("value", &alternateYCoord1Value);
	if (eResult != tinyxml2::XML_NO_ERROR)
	{
		throw std::runtime_error("ALW - Runtime Error: Unable to convert AlternateYCoord1 attribute.");
	}

	return alternateYCoord1Value;
}

float ObjectGroups::readInteractiveObjectAlternateXCoord2(tinyxml2::XMLElement * element) const
{
	// ALW - Property AlternateXCoord2
	element = element->FirstChildElement("property");
	if (element == nullptr)
	{
		throw std::runtime_error("ALW - Runtime Error: There are no property elements.");
	}

	std::string alternateXCoord2Name = "AlternateXCoord2";
	while (alternateXCoord2Name != element->Attribute("name"))
	{
		element = element->NextSiblingElement("property");
		if (element == nullptr)
		{
			throw std::runtime_error("ALW - Runtime Error: Failed to find AlternateXCoord2 property element.");
		}
	}

	float alternateXCoord2Value;
	tinyxml2::XMLError eResult = element->QueryFloatAttribute("value", &alternateXCoord2Value);
	if (eResult != tinyxml2::XML_NO_ERROR)
	{
		throw std::runtime_error("ALW - Runtime Error: Unable to convert AlternateXCoord2 attribute.");
	}

	return alternateXCoord2Value;
}

float ObjectGroups::readInteractiveObjectAlternateYCoord2(tinyxml2::XMLElement * element) const
{
	// ALW - Property AlternateYCoord2
	element = element->FirstChildElement("property");
	if (element == nullptr)
	{
		throw std::runtime_error("ALW - Runtime Error: There are no property elements.");
	}

	std::string alternateYCoord2Name = "AlternateYCoord2";
	while (alternateYCoord2Name != element->Attribute("name"))
	{
		element = element->NextSiblingElement("property");
		if (element == nullptr)
		{
			throw std::runtime_error("ALW - Runtime Error: Failed to find 1PosY property element.");
		}
	}

	float alternateYCoord2Value;
	tinyxml2::XMLError eResult = element->QueryFloatAttribute("value", &alternateYCoord2Value);
	if (eResult != tinyxml2::XML_NO_ERROR)
	{
		throw std::runtime_error("ALW - Runtime Error: Unable to convert AlternateYCoord2 attribute.");
	}

	return alternateYCoord2Value;
}

float ObjectGroups::readInteractiveObjectAlternateXCoord3(tinyxml2::XMLElement * element) const
{
	// ALW - Property AlternateXCoord3
	element = element->FirstChildElement("property");
	if (element == nullptr)
	{
		throw std::runtime_error("ALW - Runtime Error: There are no property elements.");
	}

	std::string alternateXCoord3Name = "AlternateXCoord3";
	while (alternateXCoord3Name != element->Attribute("name"))
	{
		element = element->NextSiblingElement("property");
		if (element == nullptr)
		{
			throw std::runtime_error("ALW - Runtime Error: Failed to find AlternateXCoord3 property element.");
		}
	}

	float alternateXCoord3Value;
	tinyxml2::XMLError eResult = element->QueryFloatAttribute("value", &alternateXCoord3Value);
	if (eResult != tinyxml2::XML_NO_ERROR)
	{
		throw std::runtime_error("ALW - Runtime Error: Unable to convert AlternateXCoord3 attribute.");
	}

	return alternateXCoord3Value;
}

float ObjectGroups::readInteractiveObjectAlternateYCoord3(tinyxml2::XMLElement * element) const
{
	// ALW - Property AlternateYCoord3
	element = element->FirstChildElement("property");
	if (element == nullptr)
	{
		throw std::runtime_error("ALW - Runtime Error: There are no property elements.");
	}

	std::string alternateYCoord3Name = "AlternateYCoord3";
	while (alternateYCoord3Name != element->Attribute("name"))
	{
		element = element->NextSiblingElement("property");
		if (element == nullptr)
		{
			throw std::runtime_error("ALW - Runtime Error: Failed to find AlternateYCoord3 property element.");
		}
	}

	float alternateYCoord3Value;
	tinyxml2::XMLError eResult = element->QueryFloatAttribute("value", &alternateYCoord3Value);
	if (eResult != tinyxml2::XML_NO_ERROR)
	{
		throw std::runtime_error("ALW - Runtime Error: Unable to convert AlternateYCoord3 attribute.");
	}

	return alternateYCoord3Value;
}

float ObjectGroups::readInteractiveObjectCollisionBoxXCoord(tinyxml2::XMLElement * element) const
{
	// ALW - Property CollisionBoxXCoord
	element = element->FirstChildElement("property");
	if (element == nullptr)
	{
		throw std::runtime_error("ALW - Runtime Error: There are no property elements.");
	}

	std::string collisionBoxXCoordName = "CollisionBoxXCoord";
	while (collisionBoxXCoordName != element->Attribute("name"))
	{
		element = element->NextSiblingElement("property");
		if (element == nullptr)
		{
			throw std::runtime_error("ALW - Runtime Error: Failed to find CollisionBoxXCoord property element.");
		}
	}

	float collisionBoxXCoordValue;
	tinyxml2::XMLError eResult = element->QueryFloatAttribute("value", &collisionBoxXCoordValue);
	if (eResult != tinyxml2::XML_NO_ERROR)
	{
		throw std::runtime_error("ALW - Runtime Error: Unable to convert CollisionBoxXCoord attribute.");
	}

	return collisionBoxXCoordValue;
}

float ObjectGroups::readInteractiveObjectCollisionBoxYCoord(tinyxml2::XMLElement * element) const
{
	// ALW - Property CollisionBoxYCoord
	element = element->FirstChildElement("property");
	if (element == nullptr)
	{
		throw std::runtime_error("ALW - Runtime Error: There are no property elements.");
	}

	std::string collisionBoxYCoordName = "CollisionBoxYCoord";
	while (collisionBoxYCoordName != element->Attribute("name"))
	{
		element = element->NextSiblingElement("property");
		if (element == nullptr)
		{
			throw std::runtime_error("ALW - Runtime Error: Failed to find CollisionBoxYCoord property element.");
		}
	}

	float collisionBoxYCoordValue;
	tinyxml2::XMLError eResult = element->QueryFloatAttribute("value", &collisionBoxYCoordValue);
	if (eResult != tinyxml2::XML_NO_ERROR)
	{
		throw std::runtime_error("ALW - Runtime Error: Unable to convert CollisionBoxYCoord attribute.");
	}

	return collisionBoxYCoordValue;
}

float ObjectGroups::readInteractiveObjectCollisionBoxWidth(tinyxml2::XMLElement * element) const
{
	// ALW - Property CollisionBoxWidth
	element = element->FirstChildElement("property");
	if (element == nullptr)
	{
		throw std::runtime_error("ALW - Runtime Error: There are no property elements.");
	}

	std::string collisionBoxWidthName = "CollisionBoxWidth";
	while (collisionBoxWidthName != element->Attribute("name"))
	{
		element = element->NextSiblingElement("property");
		if (element == nullptr)
		{
			throw std::runtime_error("ALW - Runtime Error: Failed to find CollisionBoxWidth property element.");
		}
	}

	float collisionBoxWidthValue;
	tinyxml2::XMLError eResult = element->QueryFloatAttribute("value", &collisionBoxWidthValue);
	if (eResult != tinyxml2::XML_NO_ERROR)
	{
		throw std::runtime_error("ALW - Runtime Error: Unable to convert CollisionBoxWidth attribute.");
	}

	return collisionBoxWidthValue;
}

float ObjectGroups::readInteractiveObjectCollisionBoxHeight(tinyxml2::XMLElement * element) const
{
	// ALW - Property CollisionBoxHeight
	element = element->FirstChildElement("property");
	if (element == nullptr)
	{
		throw std::runtime_error("ALW - Runtime Error: There are no property elements.");
	}

	std::string collisionBoxHeightName = "CollisionBoxHeight";
	while (collisionBoxHeightName != element->Attribute("name"))
	{
		element = element->NextSiblingElement("property");
		if (element == nullptr)
		{
			throw std::runtime_error("ALW - Runtime Error: Failed to find CollisionBoxHeight property element.");
		}
	}

	float collisionBoxHeightValue;
	tinyxml2::XMLError eResult = element->QueryFloatAttribute("value", &collisionBoxHeightValue);
	if (eResult != tinyxml2::XML_NO_ERROR)
	{
		throw std::runtime_error("ALW - Runtime Error: Unable to convert CollisionBoxHeight attribute.");
	}

	return collisionBoxHeightValue;
}

std::string ObjectGroups::readInteractiveObjectAttachedTo(tinyxml2::XMLElement * element) const
{
	// ALW - Property AttachedTo
	element = element->FirstChildElement("property");
	if (element == nullptr)
	{
		throw std::runtime_error("ALW - Runtime Error: There are no property elements.");
	}

	std::string attachedToName = "AttachedTo";
	while (attachedToName != element->Attribute("name"))
	{
		element = element->NextSiblingElement("property");
		if (element == nullptr)
		{
			throw std::runtime_error("ALW - Runtime Error: Failed to find AttachedTo property element.");
		}
	}

	return element->Attribute("value");
}

std::string ObjectGroups::readInteractiveObjectColor(tinyxml2::XMLElement * element) const
{
	// ALW - Property Color
	element = element->FirstChildElement("property");
	if (element == nullptr)
	{
		throw std::runtime_error("ALW - Runtime Error: There are no property elements.");
	}

	std::string colorName = "Color";
	while (colorName != element->Attribute("name"))
	{
		element = element->NextSiblingElement("property");
		if (element == nullptr)
		{
			throw std::runtime_error("ALW - Runtime Error: Failed to find Color property element.");
		}
	}

	return element->Attribute("value");
}

std::string ObjectGroups::readInteractiveObjectStyle(tinyxml2::XMLElement * element) const
{
	// ALW - Property Style
	element = element->FirstChildElement("property");
	if (element == nullptr)
	{
		throw std::runtime_error("ALW - Runtime Error: There are no property elements.");
	}

	std::string styleName = "Style";
	while (styleName != element->Attribute("name"))
	{
		element = element->NextSiblingElement("property");
		if (element == nullptr)
		{
			throw std::runtime_error("ALW - Runtime Error: Failed to find Style property element.");
		}
	}

	return element->Attribute("value");
}
