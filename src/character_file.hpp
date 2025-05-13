#pragma once
#include "json.hpp"
#include "space_items.hpp"
#include "prison_rooms.hpp"
#include <string>

class Character {
private:
public:
  std::string                            name;
  std::string                            origin;
  std::string                            home_smell;
  int                                    health = 5;
  std::shared_ptr<ItemData>              heldItem;
  std::vector<std::shared_ptr<ItemData>> inventory;
  std::string                            map_area = "prison_map";
  std::shared_ptr<SpaceRoom>             location;

  Character() {};

  Character(std::string                player, 
			std::string                starsystem, 
			std::string                smell,
			std::shared_ptr<SpaceRoom> place)
			: name(player)
			, origin(starsystem)
			, home_smell(smell)
			, location(place) {}

  Character(std::string                player, 
			std::string                starsystem, 
			std::string                smell,
			std::shared_ptr<ItemData>  item,
			std::string                area,
			std::shared_ptr<SpaceRoom> place)
			: name(player)
			, origin(starsystem)
			, home_smell(smell)
			, heldItem(item)
			, map_area(area)
			, location(place) {}

/*   void savePlayer() {
	nlohmann::json j;
	j = nlohmann::json {
	  {"name", name},
	  {"origin", origin},
	  {"home_smell", home_smell},
	  {"heldItem", heldItem.to_json()},
	  {"location", location->to_json()}
	};
  }
 */
  void set_location(std::string area, int room) {
	
  }

  void setHeldItem(std::shared_ptr<ItemData> pickedUp) { heldItem = pickedUp; }

  void key_test(std::string &player_name, std::shared_ptr<ItemData> secret_key) {
	if (player_name == CF + "Kevin" + DEF) {
	  heldItem = secret_key;
	  std::cout << "\tYou recieved " << secret_key->item_name << "\n";
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

  std::string health_amount() {
    std::string draw_health;
    if (health != 0) {
      for (int h = 0; h < health; h++) {
      draw_health = draw_health + "#";
      } 
    } else {
      cls();
      std::cout << "Game Over!" << std::endl;
      std::cout << "\n\n[press enter]";
      std::cin.get();
    }
    return draw_health;
  }

  void display() {
	using namespace std;
	cout << "\nPlayer Name:  " << name << endl;
	cout << "----------------------------" << endl;
	cout << "Star system:  " << origin << endl;
	cout << "People say everywhere in " << origin << " smells like "
		 << home_smell << endl;
	cout << "Current Location:  " << location->room_name
		 << endl;
	cout << "Health: " << RF + health_amount() + DEF << endl;
	cout << "\nBut you feel gritty and grimy.\nMaybe find a place to take a "
			"bath."
		 << endl;
	if (heldItem != nullptr) {
	  cout << "Held items: " << heldItem->item_name << "\n";
	} else {
	  cout << "Held items: \n";
	}
	cout << "\n[press enter]" << endl;
	cin.get();
	cout << endl;
  }
};


Character createPlayer(std::shared_ptr<SpaceRoom> room, std::shared_ptr<ItemData> item) {
  std::cin.ignore();
  std::string temp;
  std::vector starSystems = {"Snargleblort-5",    "Gloopulon Beta",
							 "Sporkalon Prime",   "Flibberjax 12",
							 "Wobbletron-9000",   "Blipnar's Cradle",
							 "Quasar McDoodle",   "Fizzleflux Nebula",
							 "Zapwobble Cluster", "Zonkulous Prime Rib"};

  std::string starsystem = starSystems[rand_range(0, 9)];
  std::vector smells = {"old, dead...fish",
						"cotton soaked in...slog blisster",
						"the under scum...of wet dogs",
						"sewage drippings...from long dead roach saliva",
						"the remains of a thousand bone leviathans",
						"some rando been sitting in dog pee all day",
						"a bag of dead penguins",
						"fish vomit",
						"the rotted juice-chunks of garbage bags"};
  std::string homeSmell = smells[rand_range(0, 8)];
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
  std::cout << "       Hello prisoner... em.. it says here\n"
			   "      your name here zuh.. " << CF + name + DEF << "? " << std::endl;
  std::cout << "      ..And I see you're from... damn" << std::endl;
  std::string home = starsystem;
  std::cout << "      So.. you're from the star system " << YF + home + DEF
			<< "?\n      Disgusting place.\n"
			<< std::endl;
  std::cout
			<< "      My wife and I visited there last year,\n"
			<< "      it smells like " << YF + homeSmell + DEF << std::endl;
  Character player(CF + name + DEF, YF + home + DEF, YF + homeSmell + DEF, room);
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
