#pragma once
#include <vector>
#include "prison_rooms.hpp"



Character firstStory(Character& player, std::vector<std::shared_ptr<SpaceRoom>> roomMap) {
    if (player.heldItem->item_key == "key") {
        cls();
        std::cout << RF + "Well..\nYou've managed to escape. Haha...\n" + DEF;
        std::cout << "Many do not make it this far.\n"
                    "There are some people how never make it out alive.\n"
                    "Of course, how alive are you... really?" << std::endl;
        std::cout << "\n[press enter]";
        std::cin.get();
        cls();
        player.location = roomMap[4];
    } else {
        cls();
        std::cout << RF + "You have no " + CF + "key" + DEF + ".\n";
        std::cout << "\n[press enter]";
        std::cin.get();
        cls();
        player.location = roomMap[0];
        std::cout << "\nYou notice this strange electric static shivering through your bones"
                     "\nas you stand so close to the forcefield.\n"
                     "\nThe world around comes back into view.";
        std::cout << "\n[press enter]";        
        std::cin.get();
    }
    return player;
}