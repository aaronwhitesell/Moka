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
		throw std::runtime_error("ALW - Logic Error: Failed to load " + filename);
	}
}

void ObjectGroups::readInteractiveGroup(tinyxml2::XMLDocument &config)
{
	tinyxml2::XMLElement *element = config.FirstChildElement("map");
	if (element == nullptr)
	{
		throw std::runtime_error("ALW - Logic Error: Failed to read map element.");
	}

	// ALW - Find the Interactive objectgroup
	element = element->FirstChildElement("objectgroup");
	if (element == nullptr)
	{
		throw std::runtime_error("ALW - Logic Error: There are no objectgroup elements.");
	}

	std::string objectGroupName = "Interactive";
	while (objectGroupName != element->Attribute("name"))
	{
		element = element->NextSiblingElement("objectgroup");
		if (element == nullptr)
		{
			throw std::runtime_error("ALW - Logic Error: Failed to read objectgroup element.");
		}
	}

	// ALW - Read Interactive objectgroup's attributes
	mInteractiveGroup.setName(static_cast<std::string>(element->Attribute("name")));

	int width;
	tinyxml2::XMLError eResult = element->QueryIntAttribute("width", &width);
	if (eResult != tinyxml2::XML_NO_ERROR)
	{
		throw std::runtime_error("ALW - Logic Error: Unable to convert width attribute");
	}
	mInteractiveGroup.setWidth(width);

	int height;
	eResult = element->QueryIntAttribute("height", &height);
	if (eResult != tinyxml2::XML_NO_ERROR)
	{
		throw std::runtime_error("ALW - Logic Error: Unable to convert height attribute");
	}
	mInteractiveGroup.setHeight(height);

	// ALW - Read Interactive objectgroup's objects
	std::vector<InteractiveObject> interactiveObjects;

	element = element->FirstChildElement("object");
	if (element == nullptr)
	{
		throw std::runtime_error("ALW - Logic Error: There are no object elements.");
	}

	for (; element != nullptr; element = element->NextSiblingElement("object"))
	{
		// ALW - Don't reassign element. It is needed for the loop.
		tinyxml2::XMLElement *elem = element;

		std::string name = elem->Attribute("name");
		std::string type = elem->Attribute("type");

		float x; // ALW - Read int as float (reduces number of casts when used).
		eResult = elem->QueryFloatAttribute("x", &x);
		if (eResult != tinyxml2::XML_NO_ERROR)
		{
			throw std::runtime_error("ALW - Logic Error: Unable to convert x attribute");
		}

		float y; // ALW - Read int as float (reduces number of casts when used).
		eResult = elem->QueryFloatAttribute("y", &y);
		if (eResult != tinyxml2::XML_NO_ERROR)
		{
			throw std::runtime_error("ALW - Logic Error: Unable to convert y attribute");
		}

		float width;
		eResult = elem->QueryFloatAttribute("width", &width);
		if (eResult != tinyxml2::XML_NO_ERROR)
		{
			throw std::runtime_error("ALW - Logic Error: Unable to convert width attribute");
		}

		float height;
		eResult = elem->QueryFloatAttribute("height", &height);
		if (eResult != tinyxml2::XML_NO_ERROR)
		{
			throw std::runtime_error("ALW - Logic Error: Unable to convert height attribute");
		}

		// ALW - Read each object's properties
		tinyxml2::XMLElement *properties = elem->FirstChildElement("properties");
		if (properties == nullptr)
		{
			throw std::runtime_error("ALW - Logic Error: There are no properties elements.");
		}

		// ALW - Property 0PosX
		elem = properties->FirstChildElement("property");
		if (elem == nullptr)
		{
			throw std::runtime_error("ALW - Logic Error: There are no property elements.");
		}

		std::string posX0Name = "0PosX";
		while (posX0Name != elem->Attribute("name"))
		{
			elem = elem->NextSiblingElement("property");
			if (elem == nullptr)
			{
				throw std::runtime_error("ALW - Logic Error: Failed to find 0PosX property element.");
			}
		}

		float posX0Value;
		eResult = elem->QueryFloatAttribute("value", &posX0Value);
		if (eResult != tinyxml2::XML_NO_ERROR)
		{
			throw std::runtime_error("ALW - Logic Error: Unable to convert 0PosX attribute.");
		}

		// ALW - Property 0PosY
		elem = properties->FirstChildElement("property");
		if (elem == nullptr)
		{
			throw std::runtime_error("ALW - Logic Error: There are no property elements.");
		}

		std::string posY0Name = "0PosY";
		while (posY0Name != elem->Attribute("name"))
		{
			elem = elem->NextSiblingElement("property");
			if (elem == nullptr)
			{
				throw std::runtime_error("ALW - Logic Error: Failed to find 0PosY property element.");
			}
		}

		float posY0Value;
		eResult = elem->QueryFloatAttribute("value", &posY0Value);
		if (eResult != tinyxml2::XML_NO_ERROR)
		{
			throw std::runtime_error("ALW - Logic Error: Unable to convert 0PosY attribute.");
		}

		// ALW - Property 1PosX
		elem = properties->FirstChildElement("property");
		if (elem == nullptr)
		{
			throw std::runtime_error("ALW - Logic Error: There are no property elements.");
		}

		std::string posX1Name = "1PosX";
		while (posX1Name != elem->Attribute("name"))
		{
			elem = elem->NextSiblingElement("property");
			if (elem == nullptr)
			{
				throw std::runtime_error("ALW - Logic Error: Failed to find 1PosX property element.");
			}
		}

		float posX1Value;
		eResult = elem->QueryFloatAttribute("value", &posX1Value);
		if (eResult != tinyxml2::XML_NO_ERROR)
		{
			throw std::runtime_error("ALW - Logic Error: Unable to convert 1PosX attribute.");
		}

		// ALW - Property 1PosY
		elem = properties->FirstChildElement("property");
		if (elem == nullptr)
		{
			throw std::runtime_error("ALW - Logic Error: There are no property elements.");
		}

		std::string posY1Name = "1PosY";
		while (posY1Name != elem->Attribute("name"))
		{
			elem = elem->NextSiblingElement("property");
			if (elem == nullptr)
			{
				throw std::runtime_error("ALW - Logic Error: Failed to find 1PosY property element.");
			}
		}

		float posY1Value;
		eResult = elem->QueryFloatAttribute("value", &posY1Value);
		if (eResult != tinyxml2::XML_NO_ERROR)
		{
			throw std::runtime_error("ALW - Logic Error: Unable to convert 1PosY attribute.");
		}

		// ALW - Property AttachedTo
		elem = properties->FirstChildElement("property");
		if (elem == nullptr)
		{
			throw std::runtime_error("ALW - Logic Error: There are no property elements.");
		}

		std::string attachedToName = "AttachedTo";
		while (attachedToName != elem->Attribute("name"))
		{
			elem = elem->NextSiblingElement("property");
			if (elem == nullptr)
			{
				throw std::runtime_error("ALW - Logic Error: Failed to find AttachedTo property element.");
			}
		}

		std::string attachedToValue = elem->Attribute("value");

		// ALW - Property Color
		elem = properties->FirstChildElement("property");
		if (elem == nullptr)
		{
			throw std::runtime_error("ALW - Logic Error: There are no property elements.");
		}

		std::string colorName = "Color";
		while (colorName != elem->Attribute("name"))
		{
			elem = elem->NextSiblingElement("property");
			if (elem == nullptr)
			{
				throw std::runtime_error("ALW - Logic Error: Failed to find Color property element.");
			}
		}

		std::string colorValue = elem->Attribute("value");

		// ALW - Property Style
		elem = properties->FirstChildElement("property");
		if (elem == nullptr)
		{
			throw std::runtime_error("ALW - Logic Error: There are no property elements.");
		}

		std::string styleName = "Style";
		while (styleName != elem->Attribute("name"))
		{
			elem = elem->NextSiblingElement("property");
			if (elem == nullptr)
			{
				throw std::runtime_error("ALW - Logic Error: Failed to find Style property element.");
			}
		}

		std::string styleValue = elem->Attribute("value");

		interactiveObjects.push_back(InteractiveObject(name, type, x, y, width, height, posX0Value, posY0Value, posX1Value, posY1Value
			, attachedToValue, colorValue, styleValue));
	}

	mInteractiveGroup.setInteractiveObjects(interactiveObjects);
}
