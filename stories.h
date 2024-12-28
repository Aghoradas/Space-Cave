#pragma once
#include <vector>
#include "prison_rooms.h"



Character firstStory(Character& player, std::vector<SpaceRoom>& roomMap) {
    cls();
    std::cout << RF + "Well..\nYou've managed to escape. Haha...\n" + DEF;
    std::cout << "Many do not make it this far.\n"
                 "There are some people how never make it out alive.\n"
                 "Of course, how alive are you... really?" << std::endl;
    std::cout << "\n[press enter]";
    std::cin.get();
    cls();
    player.location = &roomMap[4];
    return player;
}