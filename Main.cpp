//code of Main.cpp
#include <SFML/Graphics.hpp>
#include "Displayer.h"
#include "Mine.h"
#include "Button.h"
#include "Bot.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "mine-race 0.1", sf::Style::Close | sf::Style::Titlebar);
   
    std::ofstream gameTree("Game Tree.txt");
    enum class GameState { menu, ongoing, over }gameState = GameState::menu;
    sf::Vector2f winSize = (sf::Vector2f)window.getSize();
    int turnCounter = 1;
    int playerNum = 1; //By default you are player 1
    Bot bot;

    Displayer displayer(window);
    displayer.setFont("fonts\\comic.ttf");
    
    Mine mine({ winSize.x - 50, winSize.y * 0.6f }, { 50, winSize.y * 0.15f });


    const sf::Vector2f buttonSize = { 180.f, 180.f };
    Button SelectPlayer1({ 200.f, winSize.y / 2 }, { 150.f, 50.f }, sf::Color::Red, "Player 1");
    Button SelectPlayer2({ 450.f, winSize.y / 2 }, { 150.f, 50.f }, sf::Color::Blue, "Player 2");
    Button robot({ 300.f, 250.f }, { 200.f, 200.f }, "images\\robot.png");

    Button pickaxe({ 10.f, winSize.y - (buttonSize.y + 5.f) }, buttonSize, "images\\pickaxe.png");
    Button drill({ (winSize.x - buttonSize.x) / 2.f, winSize.y - (buttonSize.y + 5.f) }, buttonSize, "images\\drill.jpg");
    Button nuke({ winSize.x - 10.f - buttonSize.x, winSize.y - (buttonSize.y + 5.f) }, buttonSize, "images\\nuke.jpg");
    
    Button restart({ winSize.x / 2 - 120.f, 400.f }, { 240.f, 100.f }, "images\\restart.png");
    std::map< std::pair<int, int>, int> map = {};
    bot.generateGameTree(gameTree);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
            case sf::Event::MouseButtonPressed:
                switch (gameState)
                {
                case GameState::menu:
                    if (SelectPlayer1.isSelected(window)) {
                        playerNum = 1;
                        gameState = GameState::ongoing;
                    }
                    if (SelectPlayer2.isSelected(window)) {
                        playerNum = 2;
                        gameState = GameState::ongoing;
                    }
                    break;
                case GameState::ongoing:
                    //If it is players turn
                    if (turnCounter == playerNum) {
                        if (pickaxe.isSelected(window)) {
                            mine.dig(5);
                            ++turnCounter;
                        }
                        else if (drill.isSelected(window)) {
                            mine.dig(10);
                            ++turnCounter;
                        }
                        else if (nuke.isSelected(window)) {
                            mine.dig(30);
                            ++turnCounter;
                        }
                    }
                    // If it is bots turn
                    else {
                        if (!robot.isSelected(window)) continue;

                        bot.minMax(mine, mine.getRemainingDistance());
                        while (window.pollEvent(event));
                        ++turnCounter;
                    }
                    if (mine.endReached()) {
                        gameState = GameState::over;
                        ++turnCounter;
                    }
                    break;
                case GameState::over:
                    if (restart.isSelected(window)) {
                        turnCounter = 1;
                        mine.restart();
                        gameState = GameState::menu;
                    }
                    break;
                }
                break;
            }
        }

        if (turnCounter > 2) turnCounter = 2 - turnCounter % 2;

        window.clear();
        
        switch (gameState) {
        case GameState::menu:
            displayer.draw(SelectPlayer1);
            displayer.draw(SelectPlayer2);
            displayer.displayText({ 100.f, 300.f }, "Are you player 1 or player 2?", 40, sf::Color::Green);
            break;
        case GameState::ongoing:
            displayer.draw(mine);
            displayer.draw(pickaxe);
            displayer.draw(drill);
            displayer.draw(nuke);
            displayer.displayMined((int)mine.getMinedDistance());
            displayer.draw(robot);
            displayer.displayTurn(turnCounter);
            break;
        case GameState::over:
            displayer.draw(mine);
            sf::String resultMessage;
            if (turnCounter == playerNum)
                displayer.displayText({ 235.f, 300.f }, "YOU WON!", 70, sf::Color::Yellow);
            else
                displayer.displayText({ 260.f, 300.f }, "you lost", 70, sf::Color::Yellow);
            
            displayer.draw(restart);
            break;
        }
        
        window.display();
    }

    return 0;
}