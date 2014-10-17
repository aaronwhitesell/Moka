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
	if (config.LoadFile(filename.c_str()) != tinyxml2::XML_NO_ERROR)
	{
		throw std::runtime_error("TinyXML2 - Failed to load " + filename);
	}
}

void ObjectGroups::readInteractiveGroup(tinyxml2::XMLDocument &config)
{
	tinyxml2::XMLElement *map = config.FirstChildElement("map");
	assert(("ALW - Logic Error: There is no map to read! This must be a incorrectly formatted TMX file.", map));

	tinyxml2::XMLElement *objectGroup = map->FirstChildElement("objectgroup");
	assert(("ALW - Logic Error: There must be at least one object group!", objectGroup));

	std::string objectGroupName = "Interactive";
	while (objectGroupName != objectGroup->Attribute("name"))
	{
		objectGroup = objectGroup->NextSiblingElement("objectgroup");
		assert(("ALW - Logic Error: There must be an object group named Interactive!", objectGroup));
	}
	
	assert(("ALW - Logic Error : There is no name attribute!", objectGroup->Attribute("name")));
	mInteractiveGroup.setName(static_cast<std::string>(objectGroup->Attribute("name")));
	assert(("ALW - Logic Error : There is no width attribute!", objectGroup->Attribute("width")));
	mInteractiveGroup.setWidth(objectGroup->IntAttribute("width"));
	assert(("ALW - Logic Error : There is no height attribute!", objectGroup->Attribute("height")));
	mInteractiveGroup.setHeight(objectGroup->IntAttribute("height"));

	std::vector<InteractiveObject> interactiveObjects;
	tinyxml2::XMLElement *object = objectGroup->FirstChildElement("object");
	assert(("ALW - Logic Error: The Interactive object group is empty!", objectGroup));
	for (; object != nullptr; object = object->NextSiblingElement("object"))
	{
		assert(("ALW - Logic Error : There is no name attribute!", object->Attribute("name")));
		std::string name = object->Attribute("name");
		assert(("ALW - Logic Error : There is no type attribute!", object->Attribute("type")));
		std::string type = object->Attribute("type");
		assert(("ALW - Logic Error : There is no x attribute!", object->IntAttribute("x")));
		float x = object->FloatAttribute("x");
		assert(("ALW - Logic Error : There is no x attribute!", object->IntAttribute("y")));
		float y = object->FloatAttribute("y");
		assert(("ALW - Logic Error : There is no x attribute!", object->IntAttribute("width")));
		float width = object->FloatAttribute("width");
		assert(("ALW - Logic Error : There is no x attribute!", object->IntAttribute("height")));
		float height = object->FloatAttribute("height");

		tinyxml2::XMLElement *properties = object->FirstChildElement("properties");
		assert(("ALW - Logic Error: There are no properties!", properties));
		tinyxml2::XMLElement *property = properties->FirstChildElement("property");
		assert(("ALW - Logic Error: There is no property!", property));
		assert(("ALW - Logic Error : There is no name attribute for the property!", property->Attribute("name")));
		std::string propertyName = property->Attribute("name");

		std::string attachedTo;
		if (propertyName == "AttachedTo")
		{
			assert(("ALW - Logic Error : There is no value for the attachedTo property!", property->Attribute("value")));
			attachedTo = property->Attribute("value");
		}
		else
			assert(("ALW - Logic Error : There was no attachedTo property!", false));

		interactiveObjects.push_back(InteractiveObject(name, type, attachedTo, x, y, width, height));
	}

	mInteractiveGroup.setInteractiveObjects(interactiveObjects);
}
