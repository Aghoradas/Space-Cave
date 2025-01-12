#include <iostream>
#include <algorithm>
#include <thread>
#include <map>
#include <random>
#include <sstream>
#include <string>
#include <unistd.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "json.hpp"

#ifdef __linux__
void cls() { system("clear"); }

#elif win32
void cls() { system("cls"); }

#endif

const std::string DEF = "\033[0m"; // Default Font color
const std::string CF  = "\033[1;36m"; // Cyan font
const std::string GF  = "\033[32m"; // Green font
const std::string RF  = "\033[31m"; // Red font
const std::string YF  = "\033[33m"; // Yellow font

int randomRange(int num1, int num2) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dis(num1, num2);
  int number = dis(gen);
  return number;
}

// prison_rooms.hpp is included after #ifdefs and font colors so it
// can utilize their code.
#include "space_npc.hpp"
#include "prison_rooms.hpp"
#include "character_file.hpp"
#include "stories.hpp"

std::vector<std::string> commandTerminal(Character & player) {
  std::string command;
  std::vector<std::string> commandLine;
  std::string word;

  do {
    std::cout << "\n" + YF + "[Health: " << RF + player.health + YF << "]:> " + DEF;
    std::getline(std::cin, command);
  } while (command.empty());
  std::cout << std::endl;

  std::stringstream ss(command);
  while (ss >> word) {
    commandLine.emplace_back(word);
  }
  return commandLine;
}

std::string linguistics(std::string &command) {
  std::unordered_map<std::string, std::unordered_set<std::string>>
      equivalences = {{"go",   {"move", "walk", "run", "head"}},
                      {"look", {"examine", "what"}},
                      {"get",  {"take", "pickup"}},
                      {"find", {"search", "seek"}},
                      {"north",{"n", "no"}},
                      {"south",{"s", "so"}},
                      {"east", {"e", "ea"}},
                      {"west", {"w", "we"}},
                      {"help", {"?"}}};
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

Character move(std::vector<std::string> &comLine, Character &player,
               std::vector<std::shared_ptr<SpaceRoom>> roomMap) {
  enum class Action { north, south, east, west };
  std::map<std::string, Action> doMove = {{"north", Action::north},
                                          {"south", Action::south},
                                          {"east", Action::east},
                                          {"west", Action::west}};
  const std::string direction = linguistics(comLine[1]);
  std::shared_ptr<SpaceRoom> room = player.location;
  if (room->doors.contains(comLine[1])) {
    if (room->doors[comLine[1]]->closed()) {
      std::cout << "\nThe door is closed.\n" << std::endl;
      return player;
    }
  }
  if (comLine.size() != 1 && player.location->exits.contains(direction)) {
    using enum Action;
    switch (doMove[direction]) {
      case north: {
        int next_room = player.location->exits[direction];
        player.location = roomMap[next_room];
        break;
      }
      case south: {
        int next_room = player.location->exits[direction];
        player.location = roomMap[next_room];
        break;
      }
      case east: {
        int next_room = player.location->exits[direction];
        player.location = roomMap[next_room];
        break;
      }
      case west: {
        int next_room = player.location->exits[direction];
        player.location = roomMap[next_room];
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

/* Character dropItem(std::vector<ItemData> item) {
  if (!item.empty()) {
    for ()
  }
} */


Character get_mech(std::vector<std::string>& comLine, Character& player,
              std::vector<std::shared_ptr<SpaceRoom>> roomMap) {
  if (comLine[1].empty() || player.location->room_items.empty()) {
    std::cout << "\nGet What?" << std::endl;
  } else if (!player.location->room_items.empty()){
    int itemNum = 0;

    if (!player.location->room_items.empty()) {
      for (auto item = player.location->room_items.begin(); item < player.location->room_items.end(); item++) {
        if (player.location->room_items.at(itemNum)->item_key == comLine[1]) {
          std::shared_ptr<ItemData> tempItem = player.heldItem;
          std::cout << "You dropped " << tempItem->item_name << std::endl;
          player.heldItem = player.location->room_items[itemNum];
          std::cout << "You picked up " << player.heldItem->item_name << std::endl;
          item = player.location->room_items.erase(item);
          player.location->room_items.insert(item,tempItem);
          /* tempItem = nullptr;
          delete tempItem; */
          break;
        }
        itemNum++;
      }
    } else {
      std::cout << "There is nothing here to grab." << std::endl;
    }
  }
  return player;
}

void find_mech(std::vector<std::string> &comLine, Character &player) {
  enum class Target { exit };
  std::map<std::string, Target> findTarget = {{"exit", Target::exit}};
  if (comLine.size() > 1) {
    switch (findTarget[comLine[1]]) {
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

    default:
      break;
    }

  } else {
    std::cout << "\tYou found nothing.\n" << std::endl;
  }
}

Character open_mech(std::string &direction, Character &player) {
  player.location->doors[direction]->open();
  std::cout << "opened door" << std::endl;
  return player;

}

Character close_mech(std::string &direction, Character &player, std::vector<std::shared_ptr<SpaceRoom>> roomMap) {
  player.location->doors[direction]->close();
  std::cout << "closed door" << std::endl;
  return player;
}

void look_mech(std::vector<std::string> &comLine, Character &player, std::vector<std::shared_ptr<SpaceRoom>> roomMap) {
  switch (comLine.size()) {
  case 1: {
    player.lookDescribe();
    break;
  }
  case 2: {
    if (comLine[1] == "self") {
      player.display();
      break;
    } else if (comLine[1] == "forcefield" &&
               player.location->room_name == "Outside..") {
      firstStory(player, roomMap);
    } else if (player.location->exits.contains(comLine[1])) {
      std::string ifDoor = comLine[1];
      if (player.location->doors.contains(ifDoor)) {
        if (player.location->doors[ifDoor]->closed()) {
          int direction = player.location->exits[comLine[1]];
          std::cout << "\nYou look: " << comLine[1] << std::endl;
          std::cout << "\n" << roomMap[direction]->near_room_info << std::endl;
        } else if (player.location->doors[ifDoor]->window()) {
          int direction = player.location->exits[comLine[1]];
          std::cout << "\nThrough bars you look: " << comLine[1] << std::endl;
          std::cout << "\n" << roomMap[direction]->near_room_info << std::endl;
        } 
        else {
          std::cout << "\nYou see nothing. The door is closed.\n" << std::endl;
        }

      }
      std::cout << "\n[press enter]";
      std::cin.get();

    } else if (comLine[1] == player.heldItem->item_key) {
      player.heldItem->look_item();
    } else {
      for (auto &item : player.location->room_items) {
        if (item->item_key == comLine[1]) {
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
  std::cout << " -------------------" << std::endl;
  std::cout << "  go [direction] - moves you to the room in a given direction"
            << std::endl;
  std::cout << "  look - displays details about surroungings" << std::endl;
  std::cout << "  look [object] - displays details about object" << std::endl;
  std::cout << "  get [object] - picks object up" << std::endl;
  std::cout << " ------------------------" << std::endl;
  std::cout << "\n[press enter]\n";
  std::cin.get();
}

Character actionCommands(std::vector<std::string> &comLine, Character &player,
                         std::vector<std::shared_ptr<SpaceRoom>> roomMap) {
  enum class Action { go, help, look, get, find, open, close };
  std::map<std::string, Action> takeAction = {{"go", Action::go},
                                              {"help", Action::help},
                                              {"look", Action::look},
                                              {"get", Action::get},
                                              {"find", Action::find},
                                              {"open", Action::open},
                                              {"close", Action::close}};
  std::string processedCom = linguistics(comLine[0]);
  if (takeAction.contains(processedCom)) {
    using enum Action;
    switch (takeAction[processedCom]) {
    case go: {
      return move(comLine, player, roomMap);
    }
    case look: {
      look_mech(comLine, player, roomMap);
      break;
    }
    case help: {
      help_info();
      break;
    }
    case get: {
      get_mech(comLine, player, roomMap);
      break;
    }
    case find: {
      find_mech(comLine, player);
      break;
    }
    case open: {
      std::string direction = linguistics(comLine[1]);
      if (player.location->doors.contains(direction)) {
        open_mech(direction, player);
      } else {
        std::cout << "\nThere is no door to be opened.\n" << std::endl;
      }
      break;
    }
    case close: {
      std::string direction = linguistics(comLine[1]);
      if (player.location->doors.contains(direction)) {
        close_mech(direction, player, roomMap);
      } else {
        std::cout << "\nThere is no door to be closed.\n" << std::endl;
      }
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



void mainGame(Character &player, std::vector<std::shared_ptr<SpaceRoom>> roomMap) {
  while (true) {
    player.location->display();
    std::vector<std::string> comLine;
    comLine = commandTerminal(player);
    if (comLine[0] == "quit") {
      break;
    } else if (comLine[0] == "clear") {
      cls();
    } else {
      player = actionCommands(comLine, player, roomMap);
    }
  }
}

int main() {
  int menuAns;
  while (true) {
    menuAns = 0;
    cls();
    std::cout << CF +
                     "                      #####  ####  ##     #### #####\n"
                     "                     ##     ##  # ###   ##    ##\n"
                     "                     ####  ####  ## #  ##    ####\n"
                     "                        # ##    ##### ##    ##\n"
                     "                   ##### ##    ##   # #### ######\n" +
                     DEF +
                     "                           =============\n"
                     "                             1 ) Begin\n"
                     "                             2 ) " +
                     RF + "Exit" + DEF +
                     "\n"
                     "                         -----------------"
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
      std::vector<std::shared_ptr<SpaceRoom>> roomMap = loadrooms();
      std::shared_ptr<vItem_t> worldItems = load_items();
      Character player = createPlayer(roomMap[0], worldItems->at(3));
      mainGame(player, roomMap);
    } else {
      std::cout << "\n\tPlease make a selection from the menu.\n";
      menuAns = 0;
      std::cin.ignore();
    }
  }
  return 0;
}
