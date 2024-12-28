#include <iostream>
#include <algorithm>
#include <map>
#include <random>
#include <sstream>
#include <string>
#include <unistd.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#ifdef __linux__
void cls() { system("clear"); }

#elif win32
void cls() { system("cls"); }

#endif

const std::string DEF = "\033[0m"; // Default Font color
const std::string CF = "\033[36m"; // Cyan font
const std::string GF = "\033[32m"; // Green font
const std::string RF = "\033[31m"; // Red font
const std::string YF = "\033[33m"; // Yellow font

// prison_rooms.h is included after #ifdefs and font colors so it
// can utilize their code.
#include "prison_rooms.h"

class Character {
private:
public:
  std::string name;
  std::string origin;
  std::string home_smell;
  ItemData heldItem;
  SpaceRoom* location;

  Character() {};
  Character(std::string player, std::string starsystem, std::string smell,
            SpaceRoom* place)
      : name(player), origin(starsystem), home_smell(smell), location(place) {}

  void setHeldItem(ItemData &pickedUp) { heldItem = pickedUp; }

  void key_test(std::string &player_name, ItemData &secret_key) {
    if (player_name == CF + "Kevin" + DEF) {
      heldItem = secret_key;
      std::cout << "\tYou recieved " << secret_key.item_name << "\n";
    }
  }

  void lookDescribe() {
    std::cout << "\nYou look around:\n"
              << location->details
              << std::endl; // *make this look at room.description
    std::cout << "\n[press enter]\n";
    std::cin.get();
    std::cout << std::endl;
  }
  void display() {
    using namespace std;
    cout << "\nPlayer Name:  " << name << endl;
    cout << "----------------------------" << endl;
    cout << "Star system:  " << origin << endl;
    cout << "People say everywhere in " << origin << " smells like "
         << home_smell << endl;
    cout << "Current Location:  " << location->room_name
         << endl; // *this should be the room.name
    cout << "\nBut you feel gritty and grimy.\nMaybe find a place to take a "
            "bath."
         << endl;
    cout << "Held items: " << heldItem.item_name << "\n";
    cout << "[press enter]";
    cin.get();
    cout << endl;
  }
};

#include "stories.h"

int randomRange(int num1, int num2) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dis(num1, num2);
  int number = dis(gen);
  return number;
}

Character createPlayer(SpaceRoom& room, ItemData& item) {
  std::cin.ignore();
  std::string temp;
  std::vector starSystems = {"Snargleblort-5",    "Gloopulon Beta",
                             "Sporkalon Prime",   "Flibberjax 12",
                             "Wobbletron-9000",   "Blipnar's Cradle",
                             "Quasar McDoodle",   "Fizzleflux Nebula",
                             "Zapwobble Cluster", "Zonkulous Prime Rib"};

  std::string starsystem = starSystems[randomRange(0, 9)];
  std::vector smells = {"old, dead...fish",
                        "cotton soaked in...slog blisster",
                        "the under scum...of wet dogs",
                        "sewage drippings...from long dead roach saliva",
                        "the remains of a thousand bone leviathans",
                        "some rando been sitting in dog pee all day",
                        "a bag of dead penguins",
                        "fish vomit",
                        "the rotted juice-chunks of garbage bags"};
  std::string homeSmell = smells[randomRange(0, 8)];
  std::string name;
  do {
    cls();
    std::cout << "You enter Prisoner Intake office." << std::endl;
    std::cout << "\nWhat will your character be called?" << std::endl;
    std::cout << "Name:  ";
    getline(std::cin, name);
  } while (name == "");
  cls();
  std::string answer;
  std::cout << CF + "\n              The Intake Office" + DEF << std::endl;
  std::cout << "        -------------------------\n" << std::endl;
  std::cout << "       Hello prisoner... em.. it says here\n      your name "
               "here zuh.. "
            << CF + name + DEF << "? " << std::endl;
  std::cout << "      ..And I see you're from... damn" << std::endl;
  std::string home = starsystem;
  std::cout << "      So.. you're from the star system " << YF + home + DEF
            << "?\n      Disgusting place.\n"
            << std::endl;
  std::cout
      << "      My wife and I visited there last year, \n\tit smells like "
      << YF + homeSmell + DEF << std::endl;
  Character player(CF + name + DEF, YF + home + DEF, YF + homeSmell + DEF,
                   &room);
  std::cout << "      Is that all correct?" << std::endl;
  std::cout << "      Yes or no?: ";
  std::getline(std::cin, answer);
  std::cout << "\n       -- he smiles. Well, too bad uh.. " << player.name
            << " is it, the paperwork's already done." << std::endl;
  std::cout << "\n      Just follow that guard right there to your new home."
            << std::endl;
  std::cout << "\n      [press enter]";
  player.key_test(player.name, item);
  std::cin.get();
  std::cout
      << "        --he's smiling as you're lead away...you hear him laughing."
      << std::endl;
  std::cout << "\n      [press enter]";
  std::cin.get();
  cls();
  std::cout << "\n\n      As you enter the cramped, little, dirty cell,"
            << std::endl;
  std::cout << "      the guard slams the door locked behind you." << std::endl;
  std::cout << "      Enjoy --he says walking back down the hall." << std::endl;
  std::cout << "\n\n      You're alone.\n\n" << std::endl;
  std::cout << "\n      [press enter]";

  std::cin.get();
  cls();
  std::cout << YF + "\n      type 'help' and press enter for commands\n\n" + DEF
            << std::endl;
  return player;
}

std::vector<std::string> commandTerminal(Character *player) {
  std::string command;
  std::vector<std::string> commandLine;
  std::string word;

  do {
    std::cout << YF + "[location- " << CF + player->location->room_name + YF
              << "]:> " + DEF;
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
      equivalences = {{"go", {"move", "walk", "run", "head"}},
                      {"look", {"examine", "what"}},
                      {"get", {"take", "pickup"}},
                      {"find", {"search", "seek"}},
                      {"north",{"n", "no"}}, 
                      {"south",{"s", "so"}},
                      {"east",{"e", "ea"}},
                      {"west",{"w", "we"}},
                      {"help", {"?"}}};
  for (auto &actions : equivalences) {
    for (auto &synonym : actions.second) {
      if (command == synonym) {
        command = actions.first;
      }
    }
  }
  return command;
}

Character move(std::vector<std::string> &comLine, Character &player,
               std::vector<SpaceRoom> &roomMap) {
  enum class Action { north, south, east, west };
  std::map<std::string, Action> doMove = {{"north", Action::north},
                                          {"south", Action::south},
                                          {"east", Action::east},
                                          {"west", Action::west}};
  using enum Action;
  
  if (comLine.size() != 1) {
    std::string direction = linguistics(comLine[1]);
    switch (doMove[direction]) {
      case north: {
        int next_room = player.location->exits["north"];
        player.location = &roomMap[next_room];
        break;
      }
      case south: {
        int next_room = player.location->exits["south"];
        player.location = &roomMap[next_room];
        break;
      }
      case east: {
        int next_room = player.location->exits["east"];
        player.location = &roomMap[next_room];
        break;
      }
      case west: {
        int next_room = player.location->exits["west"];
        player.location = &roomMap[next_room];
        break;
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
              std::vector<SpaceRoom>& roomMap) {
  if (comLine[1].empty() || player.location->room_items.empty()) {
    std::cout << "\nGet What?" << std::endl;
  } else if (!player.location->room_items.empty()){
    int itemNum = 0;
  
    if (!player.location->room_items.empty()) {
      for (auto item = player.location->room_items.begin(); item < player.location->room_items.end(); item++) {
        if (player.location->room_items.at(itemNum).item_key == comLine[1]) {
          ItemData tempItem = player.heldItem;
          std::cout << "You dropped " << tempItem.item_name << std::endl;
          player.heldItem = player.location->room_items[itemNum];
          std::cout << "You picked up " << player.heldItem.item_name << std::endl;
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

void look_mech(std::vector<std::string> &comLine, Character &player,
               std::vector<SpaceRoom> &roomMap) {
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
               player.location->room_name == "Outside.." &&
               player.heldItem.item_key == "key") {
      firstStory(player, roomMap);
    } else if (player.location->exits.contains(comLine[1])) {
      int direction = player.location->exits[comLine[1]];
      std::cout << "\nYou look " << comLine[1] << std::endl;
      std::cout << "\n" << roomMap[direction].near_room_info << std::endl;
      std::cout << "\n[press enter]";
      std::cin.get();
    } else if (comLine[1] == player.heldItem.item_key) {
      player.heldItem.look_item();
    } else {
      for (auto &item : player.location->room_items) {
        if (item.item_key == comLine[1]) {
          item.look_item();
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
                         std::vector<SpaceRoom> &roomMap) {
  enum class Action { go, help, look, get, find };
  std::map<std::string, Action> takeAction = {{"go", Action::go},
                                              {"help", Action::help},
                                              {"look", Action::look},
                                              {"get", Action::get},
                                              {"find", Action::find}};
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

void mainGame(Character player, std::vector<SpaceRoom> roomMap) {
  while (true) {
    player.location->display();
    std::vector<std::string> comLine;
    comLine = commandTerminal(&player);
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
  std::vector<SpaceRoom> roomMap = loadrooms();
  std::shared_ptr<vItem_t> worldItems = load_items();
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
                     "                           ============\n"
                     "                            1) Begin\n"
                     "                            2) " +
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
      Character player = createPlayer(roomMap[0], *worldItems->at(3));
      mainGame(player, roomMap);
    } else {
      std::cout << "\n\tPlease make a selection from the menu.\n";
      continue;
    }
  }
  return 0;
}
