//Code of Displayer.h
#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Button.h"
#include "Mine.h"

class Displayer {
public:
	Displayer(sf::RenderWindow& window) : window(window) {}
	void draw(Mine& mine);
	void draw(Button& button);

	void setFont(std::string fontFile) { font.loadFromFile(fontFile); }
	void displayText(sf::Vector2f position, sf::String text, int size = 30, sf::Color color = sf::Color::White)const;
	void displayMined(int minedDistance);
	void displayTurn(int turn);
private:
	sf::RenderWindow& window;
	sf::Font font;
};


void Displayer::draw(Mine& mine) {
	//Setting the texture of the earth and drawing it
	sf::Texture earthTexture = mine.getEarthTexture();
	sf::RectangleShape earthShape = mine.getEarthShape();
	earthShape.setTexture(&earthTexture);
	window.draw(earthShape);

	sf::RectangleShape uiBorder({ (float)window.getSize().x, 5.f });
	uiBorder.setFillColor(sf::Color::White);
	
	//Displaying border between user interface and the mine from top and bottom
	uiBorder.setPosition(0.f, earthShape.getPosition().y);
	window.draw(uiBorder);
	uiBorder.setPosition(0.f, earthShape.getPosition().y + earthShape.getSize().y);
	window.draw(uiBorder);

	//Displaying the area that has been mined
	window.draw(mine.getMined());
}

void Displayer::draw(Button& button){
	sf::RectangleShape rect = button.getRect();
	sf::Texture* texture = button.getTexture();
	if(texture != nullptr)
		rect.setTexture(texture);
	else {
		displayText({ rect.getPosition().x + 23.f, rect.getPosition().y + 5.f}, button.getText(), 30, rect.getOutlineColor());
	}
	window.draw(rect);
}

void Displayer::displayText(sf::Vector2f position, sf::String textInp, int size, sf::Color color)const {
	sf::Text text(textInp, font, size);
	std::string str = textInp;
	text.setFillColor(color);
	text.setPosition(position);
	window.draw(text);
}

void Displayer::displayMined(int minedDistance){
	sf::String text;
	if (minedDistance >= 100) 
		text = "The mining is over.";
	else
		text = std::to_string(minedDistance) + " meters have been mined";

	displayText({50.f, 80.f }, text);
}

inline void Displayer::displayTurn(int turn){
	displayText({ 500.f, 80.f }, "Player " + std::to_string(2 - turn % 2) + "'s turn");
}
