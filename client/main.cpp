/**
 * @file	main.cpp
 * @brief	Client application (that uses the graphics engine)
 *
 * @authors Luca Fantò (C) SUPSI [luca.fanto@student.supsi.ch]
 *          Mattia Cainarca (C) SUPSI [mattia.cainarca@student.supsi.ch]
 *          Antonio Marroffino (C) SUPSI [antonio.marroffino@student.supsi.ch]
 */

//////////////
// #INCLUDE //
//////////////

// Project-specific includes
#include "gameManager.h"

// Standard libraries
#include <iostream>

//////////
// MAIN //
//////////

/**
 * Application entry point.
 * @param argc number of command-line arguments passed
 * @param argv array containing up to argc passed arguments
 * @return error code (0 on success, error code otherwise)
 */
int main(int argc, char *argv[])
{
	std::cout << "\n[application start]" << std::endl;
	GameManager::getInstance().startGame();
    std::cout << "\n[application terminated]" << std::endl;
    return 0;
}
