#include "json.hpp"
#include <algorithm>
#include <iostream>
#include <map>
#include <random>
#include <sstream>
#include <string>
#include <thread>
#include <unistd.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#ifdef __linux__
void cls() { system("clear"); }
#elif win32
void cls() { system("cls"); }
#endif

const std::string DEF = "\033[0m";   // Default Font color
const std::string CF = "\033[1;36m"; // Cyan font
const std::string GF = "\033[32m";   // Green font
const std::string RF = "\033[31m";   // Red font
const std::string YF = "\033[33m";   // Yellow font

int rand_range(int num1, int num2) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dis(num1, num2);
  int number = dis(gen);
  return number;
}

void lower_case(std::string &command) {
  for (auto &letter : command) {
    letter = std::tolower(letter);
  }
}

// hpp files are included after #ifdefs, font colors, and rand_range so they
// can utilize their code.
#include "space_items.hpp"
#include "space_fight.hpp"
#include "character_file.hpp"
#include "stories.hpp"

std::vector<std::string> commandTerminal(Character &player) {
  std::string command;
  std::vector<std::string> commandLine;
  std::string word;
  do {
    std::cout << YF + "[Health: " << RF + player.health_amount() + YF
              << "]:> " + DEF;
    std::getline(std::cin, command);
  } while (command.empty());
  std::cout << std::endl;
  lower_case(command);
  std::stringstream ss(command);
  while (ss >> word) {
    commandLine.emplace_back(word);
  }
  return commandLine;
}

std::string linguistics(std::string &command) {
  std::unordered_map<std::string, std::unordered_set<std::string>>
      equivalences = {{"go",    {"move", "walk", "run", "head"}},
                      {"look",  {"examine", "what"}},
                      {"get",   {"take", "pickup"}},
                      {"find",  {"search", "seek"}},
                      {"exit",  {"exits"}},
                      {"north", {"n", "no"}},
                      {"south", {"s", "so"}},
                      {"east",  {"e", "ea"}},
                      {"west",  {"w", "we"}},
                      {"help",  {"?"}}};
  for (auto &actions : equivalences) {
    for (auto &synonym : actions.second) {
      if (command == synonym) {
        command = actions.first;
        return command;
      }
    }
  }
  return command;
}

Character move(std::vector<std::string> &com_line, Character &player,
               std::vector<std::shared_ptr<SpaceRoom>> &world_map) {
  enum class Action { north, south, east, west };
  std::map<std::string, Action> doMove = {{"north", Action::north},
                                          {"south", Action::south},
                                          {"east", Action::east},
                                          {"west", Action::west}};
  const std::string direction = linguistics(com_line[1]);
  std::shared_ptr<SpaceRoom> room = player.location;
  if (room->doors.contains(direction)) {
    if (room->doors[direction]->closed()) {
      std::cout << "\nThe door is closed.\n" << std::endl;
      return player;
    }
  }
  if (com_line.size() != 1 && player.location->exits.contains(direction)) {
    using enum Action;
    switch (doMove[direction]) {
    case north: {
      int next_room = player.location->exits[direction];
      player.location = world_map[next_room];
      break;
    }
    case south: {
      int next_room = player.location->exits[direction];
      player.location = world_map[next_room];
      break;
    }
    case east: {
      int next_room = player.location->exits[direction];
      player.location = world_map[next_room];
      break;
    }
    case west: {
      int next_room = player.location->exits[direction];
      player.location = world_map[next_room];
      break;
    }
    default: {
      std::cout << "Not a direction.\n" << std::endl;
    }
    }
  } else {
    std::cout << "\nYou can't move that way." << std::endl;
  }
  return player;
}

void drop_mech(std::vector<std::string> &comLineCharacter, Character &player) {
  if (player.heldItem != nullptr) {
    std::shared_ptr<ItemData> temp_item = player.heldItem;
    std::cout << "Dropped " << temp_item->item_name << "." << std::endl;
    player.heldItem.reset();
    player.location->room_items.emplace_back(temp_item);
  } else {
    std::cout << "Nothing to drop." << std::endl;
  }
  return;
}

Character get_mech(std::vector<std::string> &com_line, Character &player,
                   std::vector<std::shared_ptr<SpaceRoom>> &world_map) {
  if (com_line[1].empty() || player.location->room_items.empty()) {
    std::cout << "\nGet What?" << std::endl;
  } else if (!player.location->room_items.empty()) {
    int itemNum = 0;
    if (!player.location->room_items.empty()) {
      for (auto item = player.location->room_items.begin();
           item < player.location->room_items.end(); item++) {
        if (player.location->room_items.at(itemNum)->item_key == com_line[1] &&
            player.heldItem != nullptr) {
          std::shared_ptr<ItemData> tempItem = player.heldItem;
          std::cout << "You dropped " << tempItem->item_name << std::endl;
          player.heldItem = player.location->room_items[itemNum];
          std::cout << "You picked up " << player.heldItem->item_name
                    << std::endl;
          player.location->room_items.erase(item);
          player.location->room_items.insert(item, tempItem);
          /* tempItem = nullptr;
          delete tempItem; */
          return player;
        } else if (player.location->room_items.at(itemNum)->item_key ==
                   com_line[1]) {
          player.heldItem = player.location->room_items[itemNum];
          std::cout << "You picked up " << player.heldItem->item_name
                    << std::endl;
          player.location->room_items.erase(item);
          return player;
        }
        itemNum++;
      }
      std::cout << "There is no " << com_line[1] << " here." << std::endl;
    } else {
      std::cout << "There is nothing here to grab." << std::endl;
    }
  }
  return player;
}

void find_mech(std::vector<std::string> &com_line, Character &player) {
  if (com_line.size() == 1) {
    std::cout << "Searching around.";
    std::cout.flush();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    int search_number = rand_range(0, (player.location->room_items.size() * 2));
    if ((player.location->room_items.size() - 1) >= search_number &&
        !player.location->room_items[search_number]->visible) {
      std::cout << "\nYou found: "
                << player.location->room_items[search_number]->item_name
                << std::endl;
      std::cout << "        "
                << player.location->room_items[search_number]->item_descrip
                << std::endl;
      std::cout << "[press enter]";
      std::cin.get();
      player.location->room_items[search_number]->visible = true;
    } else {
      std::cout << "\nYou found: nothing.." << std::endl;
      std::cout << "[press enter]";
      std::cin.get();
    }
  } else if (com_line.size() > 1) {
    std::string object = com_line[1];
    std::cout << "Seaching for " << object << "..\n";
    sleep(0);
    enum class Target { exit, object };
    std::map<std::string, Target> findTarget = {{"exit", Target::exit},
                                                {object, Target::object}};
    switch (findTarget[com_line[1]]) {
    case Target::exit:
      std::cout << "\tAs far as you can tell there are "
                << player.location->exits.size() << " here.\n";
      for (const auto &exit : player.location->exits) {
        std::cout << "\t -" << exit.first << "\n";
      }
      std::cout << "\t--------------\n";
      std::cout << "[press enter]";
      std::cin.get();
      break;

    case Target::object:
      // std::this_thread::sleep_for(std::chrono::seconds(1));
      for (const auto &item : player.location->room_items) {
        std::cout << ".";
        if (item->item_key == object) {
          std::cout << "\nYou found: " << item->item_name << std::endl;
          std::cin.get();
          break;
        } else {
          std::cout << "\nYou found nothing." << std::endl;
        }
      }
      break;

    default:
      break;
    }

  } else {
    std::cout << "\tYou found nothing.\n" << std::endl;
  }
}

Character open_mech(std::string &direction, Character &player) {
  if (player.location->doors.contains(direction)) {
    player.location->doors[direction]->open();
    std::cout << "opened door" << std::endl;
  }
  return player;
}

Character close_mech(std::string &direction, Character &player,
                     std::vector<std::shared_ptr<SpaceRoom>> world_map) {
  player.location->doors[direction]->close();
  std::cout << "closed door" << std::endl;
  return player;
}

void look_mech(std::vector<std::string> &com_line, Character &player,
               std::vector<std::shared_ptr<SpaceRoom>> &rooms_map) {
  switch (com_line.size()) {
  case 1: {
    player.lookDescribe();
    break;
  }
  case 2: {
    if (com_line[1] == "self") {
      player.display();
      break;
    } else if (com_line[1] == "forcefield" &&
               player.location->room_name == "Outside..") {
      firstStory(player, rooms_map);
    } else if (player.location->exits.contains(com_line[1])) {
      std::string selected = com_line[1];
      if (player.location->doors.contains(selected)) {
        if (player.location->doors[selected]->closed() &&
            !player.location->doors[selected]->window()) {
          std::cout << "\nYou see nothing. The door is closed.\n" << std::endl;
          return;
        } else if (!player.location->doors[selected]->closed()) {
          int direction = player.location->exits[com_line[1]];
          std::cout << "\nYou look: " << com_line[1] << std::endl;
          std::cout << "\n"
                    << rooms_map[direction]->near_room_info << std::endl;
        } else if (!player.location->doors[selected]->closed() &&
                   player.location->doors[selected]->window()) {
          int direction = player.location->exits[com_line[1]];
          std::cout << "\nThrough bars you look: " << com_line[1] << std::endl;
          std::cout << "\n"
                    << rooms_map[direction]->near_room_info << std::endl;
        }
      } else {
        int direction = player.location->exits[com_line[1]];
        std::cout << "\nYou look: " << com_line[1] << std::endl;
        std::cout << "\n" << rooms_map[direction]->near_room_info << std::endl;
      }
      std::cout << "\n[press enter]";
      std::cin.get();

    } else if (com_line[1] == player.heldItem->item_key) {
      player.heldItem->look_item();
    } else if (player.location->npc_players.contains(com_line[1])) {
      std::string other = com_line[1];
      player.location->npc_players[other].look_details();
      std::cout << "\n[press enter]\n";
      std::cin.get();
    } else {
      for (auto &item : player.location->room_items) {
        if (item->item_key == com_line[1]) {
          item->look_item();
          return;
        }
      }
    }
  }
  default: {
    break;
  }
  }
}

void help_info() {
  std::cout << RF + "\tHelp" + DEF << std::endl;
  std::cout << " ----------------------------------" << std::endl;
  std::cout
      << "  go [direction]    - moves you to the room in a given direction"
      << std::endl;
  std::cout << "  look              - displays details about surroungings"
            << std::endl;
  std::cout << "  look [object]     - displays details about object"
            << std::endl;
  std::cout << "  get [object]      - picks object up" << std::endl;
  std::cout << "  open [direction]  - opens door in that direction"
            << std::endl;
  std::cout << "  close [direction] - close door in that direction"
            << std::endl;
  std::cout << " -------------------------------------" << std::endl;
  std::cout << "\n[press enter]\n";
  std::cin.get();
}

Character action_commands(std::vector<std::string> &com_line, Character &player,
                          std::vector<std::shared_ptr<SpaceRoom>> &rooms_map) {
  enum class Action { go, help, look, get, drop, find, open, close, fight };
  std::map<std::string, Action> takeAction = {
      {"go", Action::go},      {"help", Action::help},
      {"look", Action::look},  {"drop", Action::drop},
      {"get", Action::get},    {"find", Action::find},
      {"open", Action::open},  {"close", Action::close},
      {"fight", Action::fight}};
  std::string processed_com = linguistics(com_line[0]);
  if (takeAction.contains(processed_com)) {
    using enum Action;
    switch (takeAction[processed_com]) {
    case go: {
      if (com_line.size() > 1) {
        return move(com_line, player, rooms_map);
      } else {
        std::cout << "\nGo where?\n" << std::endl;
      }
      break;
    }
    case look: {
      look_mech(com_line, player, rooms_map);
      break;
    }
    case help: {
      help_info();
      break;
    }
    case get: {
      if (com_line.size() > 1) {
        get_mech(com_line, player, rooms_map);
      } else {
        std::cout << "\nGet what?\n" << std::endl;
      }
      break;
    }
    case drop: {
      if (com_line.size() > 1) {
        drop_mech(com_line, player);
      } else {
        std::cout << "\nNothing to drop.\n" << std::endl;
      }
      break;
    }
    case find: {
      find_mech(com_line, player);
      break;
    }
    case open: {
      if (com_line.size() > 1) {
        std::string direction = linguistics(com_line[1]);
        if (!player.location->doors.empty()) {
          open_mech(direction, player);
        } else {
          std::cout << "\nThere is no door to be opened.\n" << std::endl;
        }
      } else {
        std::cout << "\nOpen what?\n" << std::endl;
      }
      break;
    }
    case close: {
      if (com_line.size() > 1) {
        std::string direction = linguistics(com_line[1]);
        if (player.location->doors.contains(direction)) {
          close_mech(direction, player, rooms_map);
        } else {
          std::cout << "\nThere is no door to be closed.\n" << std::endl;
        }
      } else {
        std::cout << "\nClose what?\n" << std::endl;
      }
      break;
    }
    case fight: {
      fight_mech(player, com_line);
      break;
    }
    default: {
      std::cout << " ..." << std::endl;
      break;
    }
    }
  } else {
    std::cout << "\nAh.. nothing.\n";
  }
  return player;
}

void mainGame(
    Character &player,
    std::map<std::string, std::vector<std::shared_ptr<SpaceRoom>>> &world_map) {
  while (true) {
    player.location = player.location->get_location(player.map_area,
                                                    player.location, world_map);
    player.location->display();
    std::vector<std::string> com_line;
    com_line = commandTerminal(player);
    if (com_line[0] == "quit" || com_line[0] == "exit") {
      break;
    } else if (com_line[0] == "clear") {
      cls();
    } else {
      player = action_commands(com_line, player, world_map[player.map_area]);
    }
  }
}

int main() {
  int menuAns;
  while (true) {
    menuAns = 0;
    cls();
    std::cout << CF +
                     "                 #####  ####  ##     #### #####    #####  ##  #   ##  #####  \n"
                     "                ##     ##  # ###   ##    ##       ##     ###  #  ##  ##\n"
                     "                ####  ####  ## #  ##    ####     #      ## #  # ##  ####\n"
                     "                   # ##    ##### ##    ##       ##     #####  ###  ##\n"
                     "              ##### ##    ##   # #### ######    ##### ##   #  ##  ######\n" +
                     DEF +
                     "                ======================================================\n"
                     "\n"
                     "                                      1 ) Begin\n"
                     "                                      2 ) " + RF + "Exit" + DEF +
                     "\n"
                     "                                   ------------------"
              << std::endl;
    std::cout << "                          Selection:  ";
    std::cin >> menuAns;
    std::cout << "\a";
    if (menuAns == 2) {
      cls();
      std::cout << "Good Bye..." << std::endl;
      std::cin.get();
      break;
    } else if (menuAns == 1) {
      std::vector<std::shared_ptr<ItemData>> worldItems = load_items();
      std::map<std::string, std::vector<std::shared_ptr<SpaceRoom>>> world_map =
          loadrooms(worldItems);
      std::shared_ptr<SpaceRoom> room = world_map["prison_map"].at(0);
      std::shared_ptr<ItemData> key = worldItems[3];
      Character player = createPlayer(room, key);
      mainGame(player, world_map);
    } else {
      std::cout << "\n\tPlease make a selection from the menu.\n";
      menuAns = 0;
      std::cin.ignore();
    }
  }
  return 0;
}
