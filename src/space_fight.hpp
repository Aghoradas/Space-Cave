#pragma once
#include <iostream>
#include "character_file.hpp"
#include "space_npc.hpp"

Character fight_mech(Character &aggressor, Npc &defender, std::vector<std::string> &com_line) {

  int agg_attack;
  int def_attack;
  bool fight = true;

  std::string damages[4] = {"wow", "yikes", "holy shit", "damn that sucks"};
  std::cout << "\nFighting: " << defender.name << std::endl;
  std::cout << "---------------------------\n" << std::endl;
  std::cout << "[press enter]";

  do {
    // aggressor hits defender
    std::getchar();
    agg_attack = rand_range(1, 6);
    defender.health = defender.health - agg_attack;
    std::cout << "\n" << aggressor.name << " hit " << defender.name << " for " << agg_attack << " damage.";
    if (defender.health < 1 || aggressor.health < 1) {
      std::cout << "\n\n Fight is over!\n" << std::endl;
      fight = false;
      break;
    }
    def_attack = rand_range(1, 6);
    aggressor.health = aggressor.health - def_attack;
    std::cout << "\n" << defender.name << " hit " << aggressor.name << " for " << def_attack << " damage.";
    if (aggressor.health < 1 || defender.health < 1) {
      std::cout << "\n\n Fight is over!\n" << std::endl;
      fight = false;
      break;
    }

  } while (fight);
  return aggressor;
}

