//Code of Mine.h
#pragma once
#include <SFML/Graphics.hpp>

class Mine
{
private:

public:
	Mine(sf::Vector2f size, sf::Vector2f position);

	void dig(float distance);
	bool endReached()const { return minedDistance >= length; }
	void restart();
	
	sf::RectangleShape getEarthShape()const { return earthShape; }
	sf::Texture getEarthTexture()const { return earthTexture; }
	sf::RectangleShape getMined()const { return minedArea; }
	float getMinedDistance()const { return minedDistance; }
	float getRemainingDistance()const { return length - minedDistance; }
private://parameters
	float minedDistance = 0.f;
	sf::RectangleShape earthShape;
	sf::Texture earthTexture;
	sf::RectangleShape minedArea;
public:
	static constexpr float  length = 100.f;
};

Mine::Mine(sf::Vector2f size, sf::Vector2f position):
	earthShape(size),
	minedArea({ size.x, size.y / 12 })
{
	earthShape.setPosition(position);
	earthShape.setFillColor(sf::Color(140, 40, 40));
	earthTexture.loadFromFile("images\\Earth.jpg");

	minedArea.setFillColor(sf::Color(20, 10, 1));
	minedArea.setScale(0.f, 1.f);
	minedArea.setPosition(position.x, position.y + size.y * 0.7f);
}

inline void Mine::dig(float distance){
	minedDistance += distance;
	minedArea.setScale(minedDistance / length, 1.f);
}

inline void Mine::restart(){
	minedDistance = 0;
	minedArea.setScale(0.f, 1.f);
}
