#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include <thread>
#include <chrono>    
#include "Mine.h"
#include <fstream>
#include <string>
#include <iostream>
#include <map>

class Bot {
public:
	Bot(){
		map = {};
		std::random_device rd;
		eng.seed(rd());
	}
	//Function to generate a game tree and store it in file
	void generateGameTree(std::ofstream &tree, int playerNum = 1, int remainingDistance = Mine::length, int depth = 0);//Calculates number of total possible ways of winning for player 1 from any position
	//minMax algorithm to calculate winning and losing paths, starting from a specified remaining distance
	int minMax(Mine& mine, int remainingDistance = Mine::length, bool isBot = true,  int depth = 0);
	//easy mode version, the AI that chooses moves at random
	void chooseRandom(Mine& mine);
private:
	std::mt19937 eng;
	std::map<std::pair<int, int>, int > map;//Map to use dynamic programming and optimize min-max algorith
};

void Bot::generateGameTree(std::ofstream &tree, int playerNum, int remaining, int depth) {
	if (depth == 0)
		map = {};//buffer Map can be reused for minMaxing. But not for generating game tree (bacause I said so)

	if (playerNum > 2)
		playerNum = 2 - playerNum % 2;

	if (map.count({ remaining, depth }) != 0 && remaining > 0) { //check If we have already had an identical game state
		//If the game is already over. This won't optimize much and its better to just say who won
			tree << "But this same scenario has happened already.\n";
		return;
	}
	
	if (remaining > 0)
		tree << remaining << " meters remain.\n";
	else
		tree << "Player " << 2 - (playerNum + 1) % 2 << " wins\n";
	
	if (remaining <= 0) {
		if (playerNum == 1)
		{
			map[{remaining, depth}] = 0;
			return;
		}
		else {
			map[{remaining, depth}] = 0;
			return;
		}
	}

	//Add padding to the tree so we can better see what is happening
	for (int i = 0; i < depth; i++)
		tree << "|";

	tree << "Player " << playerNum << " uses pickaxe. ";
	generateGameTree(tree, playerNum + 1, remaining - 5, depth + 1);

	for (int i = 0; i < depth; i++)
		tree << "|";
	tree << "Player " << playerNum << " uses drill. ";
	generateGameTree(tree, playerNum + 1, remaining - 10, depth + 1);

	for (int i = 0; i < depth; i++)
		tree << "|";
	tree << "Player " << playerNum << " uses nuke. ";
	generateGameTree(tree, playerNum + 1, remaining - 30, depth + 1);

	map[{remaining, depth}] = 0;

	if (depth == 0)
		map = {};//The map is a buffer. So we empty it after each use
}

int Bot::minMax(Mine& mine, int remaining, bool isBot, int depth) {


	//if (map.count({ remaining, depth }) != 0) { //Check f we have already had an identical game state
	//	return map[{remaining, depth}];
	//}



	if (remaining <= 0) {//If the end of mine has already been reached, the previous player has won
		if (!isBot)//If the current turn is not bot's, then previous turn was, so bot won
			return map[{remaining, (int)isBot}] = 1;
		else//The bot lost in this case
			return map[{remaining, (int)isBot}] = 0;
	}

	//Calculating if these moves lead to winning or losing

	int pickaxe = minMax(mine,  remaining - 5, !isBot, depth + 1);
	if (pickaxe == 1 && isBot == true && depth > 0) //Min-max algorithm alpha-beta pruning 
		return 1;
	if (pickaxe == 0 && isBot == false && depth > 0)//Min-max algorithm alpha-beta pruning
		return 0;
	int drill = minMax(mine, remaining - 10, !isBot, depth + 1);
	if (drill == 1 && isBot == true && depth > 0)//Min-max algorithm alpha-beta pruning
		return 1;
	if (drill == 0 && isBot == false && depth > 0)//Min-max algorithm alpha-beta pruning
		return 0;
	int nuke = minMax(mine, remaining - 30, !isBot, depth + 1);
	if (drill == 1 && isBot == true && depth > 0)//Min-max algorithm alpha-beta pruning
		return 1;
	if (drill == 0 && isBot == false && depth > 0)//Min-max algorithm alpha-beta pruning
		return 0;

	if (depth == 0) {//Once everyhing has been executed, the bot makes the move. The best option
		if (pickaxe > drill && pickaxe > nuke) {
			mine.dig(5);
			std::cout << "Pickaxe was chosen: " << pickaxe << " points\n";
		}
		else if (drill > nuke) {
			mine.dig(10);
			std::cout << "Drill was chosen: " << drill << " points\n";
		}
		else {
			mine.dig(30);
			std::cout << "nuke was chosen: " << nuke << " points\n";
		}
	}
	
	//Min-max algorithm
	if (isBot)
		return std::max(pickaxe, std::max(drill, nuke));
	else
		return std::min(pickaxe, std::min(drill, nuke));
}

void Bot::chooseRandom(Mine& mine){
	std::uniform_int_distribution<int> distr(1, 3);
	int rand = distr(eng);
	switch (rand) {
	case 1:
		mine.dig(5);
		break;
	case 2:
		mine.dig(10);
		break;
	case 3:
		mine.dig(30);
		break;
	}
}