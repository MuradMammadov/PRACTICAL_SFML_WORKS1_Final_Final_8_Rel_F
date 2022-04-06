//Code of Button.h
#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
class Button {
public:
	Button(sf::Vector2f position, sf::Vector2f size, sf::String textureFile);
	Button(sf::Vector2f position, sf::Vector2f size, sf::Color color = sf::Color::Green, sf::String text = "");
	~Button() { delete texture; }
	bool isSelected(sf::RenderWindow& window);
	sf::Texture* const& getTexture()const { return texture; }
	sf::RectangleShape const &getRect()const { return rect; }
	sf::String const& getText()const { return buttonText; }
private:
	sf::Texture *texture = nullptr;
	sf::RectangleShape rect;
	sf::String buttonText = "";
};



Button::Button(sf::Vector2f position, sf::Vector2f size, sf::String textureFile){
	texture = new sf::Texture;
	texture->loadFromFile(textureFile);
	rect.setSize(size);
	rect.setPosition(position);
	rect.setTexture(texture);
}

inline Button::Button(sf::Vector2f position, sf::Vector2f size, sf::Color color, sf::String text){
	rect.setSize(size);
	rect.setPosition(position);
	rect.setOutlineColor(color);
	rect.setOutlineThickness(3.f);
	rect.setFillColor(sf::Color::Transparent);
	buttonText = text;
	
}

inline bool Button::isSelected(sf::RenderWindow& window)
{
	sf::Vector2i pos = sf::Mouse::getPosition(window);
	sf::Vector2i rPos = (sf::Vector2i)rect.getPosition();//position of the rectangle
	sf::Vector2i rSize = (sf::Vector2i)rect.getSize(); //size of the vector
	return pos.x >= rPos.x && pos.y >= rPos.y && pos.x <= rPos.x + rSize.x && pos.y <= rPos.y + rSize.y;
}
