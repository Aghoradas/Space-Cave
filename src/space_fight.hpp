#pragma once
#include "character_file.hpp"
#include "space_npc.hpp"
#include <iostream>

void fight_mech(auto &aggressor, Npc &defender,
                std::vector<std::string> &com_line) {
  if (!player.location->npc_players.contains(com_line[1])) {
    std::cout << "\nFight who?" << std::endl;
    return;
  }
  Npc opponent = player.location->npc_players[com_line[1]];

  // random roll from 6
  int p_roll = rand_range(1, 6);
  int o_roll = rand_range(1, 6);
  bool fight = true;

  std::string damages[4] = {"wow", "yikes", "holy shit", "damn that sucks"};

  do {
    // aggressor hits defender
    defender.health = defender.health - 1; // 1 is in place of attack points
    if ()

  } while (fight);
}
