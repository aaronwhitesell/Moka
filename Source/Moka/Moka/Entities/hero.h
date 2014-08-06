#ifndef HERO_H
#define HERO_H

#include "Trambo/Events/eventHandler.h"
#include "Trambo/Resources/resourceHolder.h"
#include "Trambo/SceneNodes/entity.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Time.hpp>

#include <string>


namespace sf
{
	class RenderStates;
	class RenderTarget;
}

namespace trmb
{
	class Event;
}

class Hero : public trmb::Entity, public trmb::EventHandler // inherits SceneNode indirectly
{
public:
	enum class Type
	{
		Priest,
		Warrior,
		Wizard,
		TypeCount
	};


public:
							Hero(Type type, const trmb::TextureHolder &textures, const trmb::FontHolder &fonts, sf::FloatRect worldBounds);
							Hero(Hero &) = delete;
	Hero &					operator=(Hero &) = delete;

	virtual void			handleEvent(const trmb::Event &gameEvent);
	virtual void			updateCurrent(sf::Time dt);
	virtual sf::FloatRect	getBoundingRect() const;

	bool					isAllied() const;
	float					getMaxSpeed() const;


private:
	virtual void			drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;

	void					correctPosition();
	void					correctDiagonalVelocity();

	std::string				toString(Type type) const;
	Textures::ID			toTexture(const std::string &str) const;
	void					readXML(Type type);


private:
	struct Data
	{
		int								hitpoints;
		float							speed;
		Textures::ID					texture;
		sf::IntRect						textureRect;
		sf::Time						fireInterval;
	};


private:
	Type					mType;
	sf::Sprite				mSprite;
	sf::FloatRect			mWorldBounds;
	Data					mData;
};

#endif
