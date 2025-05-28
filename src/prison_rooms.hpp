#pragma once
#include "space_npc.hpp"
#include <unordered_map>

class RoomDoor {
private:
  bool opened = true;
  bool locked = false;
  bool windowed = false;
  int key_code;

public:

  RoomDoor() {}
  RoomDoor(bool open, bool lock)
	  : opened(open)
	  , locked(lock) {}
  RoomDoor(bool open, bool lock, bool window)
	  : opened(open)
	  , locked(lock)
	  , windowed(window) {}
  RoomDoor(bool open, bool lock, int key)
	  : opened(open)
	  , locked(lock)
	  , key_code(key) {}
/*
  nlohmann::json to_json() const {
	return {{"opened",   opened},
			{"locked",   locked},
			{"key_code", key_code}};
  }

  static RoomDoor from_json(const nlohmann::json &j) {
	return {j.at("opened").get<bool>(),
			j.at("locked").get<bool>(),
			j.at("key_code").get<int>()};
  }
*/
  bool closed() {
	if (!opened) {
	  return true;
	} else {
	  return false;
	}
  }

  bool window() {
	if (!windowed) {
	  return false;
	} else {
	  return true;
	}
  }

  void open() {
	if (!opened && locked == false) {
	  opened = true;
	}
  }
  void close() {
	if (opened) {
	  opened = false;
	}
  }

  void lock_door() {
	if (opened == false && locked == false) {
	  locked = true;
	} else if (opened == true) {
	  std::cout << "\nThe door is open.\n" << std::endl;
	} else if (locked == true) {
	  std::cout << "\nThe door is already locked.\n" << std::endl;
	}
  }

  void unlock_door() {
	if (opened == false && locked == true) {
	  locked = false;
	} else if (opened == true) {
	  std::cout << "\nThis door is already unlocked.\n" << std::endl;
	} else if (locked == false) {
	  std::cout << "\nThis door is already unlocked.\n" << std::endl;
	}
  }
};

class SpaceRoom {
private:
  

public:  
  std::string room_name;
  std::string description;
  std::string details;
  std::string near_room_info;
  std::map<std::string, std::shared_ptr<RoomDoor>> doors;
  std::map<std::string, int> exits;
  std::vector<std::shared_ptr<ItemData>> room_items;
  std::unordered_map<std::string, Npc> npc_players;

  SpaceRoom() {}
  SpaceRoom(std::string e_name, 
			std::string e_desc, 
			std::string e_details,
			std::string e_near)
			  :room_name(e_name) 
			  ,description(e_desc) 
			  ,details(e_details)
			  ,near_room_info(e_near) {}

  /*   nlohmann::json to_json() {
	  nlohmann::json doorsJson;
	  for (const auto &[key, door] : doors) {
		if (door) {
		  doorsJson[key] = door->to_json();
		} else {
		  doorsJson[key] = nullptr;
		}
	  }
	  nlohmann::json exitsJson;
	  for (const auto &[key, exit] : exits) {
		if (exit) {
		  exitsJson[key] = exit;
		} else {
		  exitsJson[key] = nullptr;
		}
	  }
	  nlohmann::json roomItemsJson = nlohmann::json::array();
	  for (const auto& item : room_items) {
		roomItemsJson = item.to_json();
	  }
	  return {{"room_name", room_name}, {"description", description},
			  {"details", details},     {"near_room_info", near_room_info},
			  {"doors", doorsJson},     {"exits", exitsJson},
			  {"look_info", look_info}, {"room_items", room_items}};
	}

	SpaceRoom from_json(nlohmann::json j) {
	  std::map<std::string, RoomDoor*> doors;
	  for (const auto &[key, door] : j.at("doors").items()) {
		if (!door.is_null()) {
		  doors[key] = new RoomDoor(RoomDoor::from_json(door));
		} else {
		  doors[key] = nullptr;
		}
	  }
	  std::map<std::string, int> exits;
	  for (const auto &[key, exit] : j.at("exits").items()) {
		if (!exit.is_null()) {
		  exits[key] = exit;
		}

	  return {
		  j.at("room_name").get<std::string>(),
		  j.at("description").get<std::string>(),
		  j.at("details").get<std::string>(),
		  j.at("near_room_info").get<std::string>(),
		  j.at("doors").get<std::map<std::string, RoomDoor*>>(),
		  j.at("exits").get<std::map<std::string, int>>(),
		  j.at("look_info").get<std::string>(),
		  j.at("room_items").get<std::vector<std::string>>()
	  };
	} */

  void set_exits(std::string direction, int next_room) {
	exits[direction] = next_room;
  }

  void set_room_items(std::shared_ptr<ItemData> item) {
	room_items.emplace_back(item);
  }

  void set_npc(Npc &being) {
	npc_players.insert(std::make_pair(being.type, being));
  }

  std::shared_ptr<SpaceRoom> get_location(std::string &map_area,
	  		  std::shared_ptr<SpaceRoom> &location,
					std::map<std::string, std::vector<std::shared_ptr<SpaceRoom>>> &world_map) {
    for (auto &area_rooms : world_map) {
      int room_number = 0;
      for (auto &room : area_rooms.second) {
        if (room->room_name == location->room_name) {
          return world_map[map_area].at(room_number);
        }
        room_number++;
      }
    }
    return location;
  }

  void display() {
	using namespace std;
	cout << endl;
	cout << "        " << CF + room_name + DEF << endl;
	cout << "      -------------------------" << endl;
	cout << " " << description << endl;
	if (!npc_players.empty()) {
	  for (auto &being : npc_players) {
		being.second.look_being();
	  }
	  std::cout << endl;
	}
	if (!room_items.empty()){
	  cout << "\nItems: \n";
	  for (auto item : room_items) {
      if (item->visible) {
        cout << "    - " << item->item_name << endl;
      }
	  }
	}

	cout << endl;
  }
};

std::map<std::string, std::vector<std::shared_ptr<SpaceRoom>>> loadrooms(
	  std::vector<std::shared_ptr<ItemData>> worldItems) {
  
  std::map<std::string, std::vector<std::shared_ptr<SpaceRoom>>> world_map;
  std::vector<std::shared_ptr<SpaceRoom>> prison_map;
  p_MapNpc npc_beings = loading_npc();  // p_MapNpc is a defined type: std::shared_ptr<std::map<std::string, Npc>>

  std::shared_ptr<SpaceRoom> cell1(std::make_shared<SpaceRoom>(
	  "Cell",
	  "\n\tThis is a very small prison cell. It is smelly\n"
	  "\tand empty, with the exception of one small cot.",
	  "\n\tOh its horrible.\nAnd there's no toilet in here.",
	  "\n\tYou see a dirty prison cell."));
  cell1->set_exits("south", 1);
  cell1->set_room_items(worldItems.at(1));

  std::shared_ptr<RoomDoor> cellroom1_hallway(
	  std::make_shared<RoomDoor>(false, false, true));
  cell1->doors["south"] = cellroom1_hallway;

  std::shared_ptr<SpaceRoom> hallway(std::make_shared<SpaceRoom>(
	  "Hallway",
	  "\n\tThis hallway leads to freedom, or wherever you came from.\n"
	  "\tOkay okay, it leads outside at least.",
	  "\n\tThe walls here have fresh paint on them, but " + RF + "blood" + DEF +
		  " stains on the\n"
		  "\tconcrete floor leading to the intake office. " +
		  RF + "It" + DEF +
		  " seems as though\n"
		  "\tthere is only one cell in this place.      " +
		  RF + "___'_" + DEF,
	  "\n\tYou see a hallway, wit" + RF + "h bloodst" + DEF +
		  "ains on the floor.\n" + RF + "\t                           _'__\n" +
		  DEF));
  hallway->doors["north"] = cellroom1_hallway;
  hallway->set_exits("north", 0);
  hallway->set_exits("south", 2);
  hallway->set_exits("east", 3);

  std::shared_ptr<RoomDoor> cellroom2_hallway(
	  std::make_shared<RoomDoor>(false, false, true));
  hallway->doors["south"] = cellroom2_hallway;

  std::shared_ptr<SpaceRoom> cell2(std::make_shared<SpaceRoom>(
	  "Other Cell",
	  "\n\tThis is a very small prison cell, just like yours.\n"
	  "\tSmelly and empty, with the exception of that one damn cot.",
	  "\n\tIts so much worse in here.\nStill, no toilet.",
	  "\n\tYou see a dirty prison cell."));
  cell2->set_npc(npc_beings->at("blipian"));
  cell2->set_exits("north", 1);
  cell2->set_room_items(worldItems.at(1));
  cell2->set_room_items(worldItems.at(3));

  cell2->doors["north"] = cellroom2_hallway;

  std::shared_ptr<SpaceRoom> intake(std::make_shared<SpaceRoom>(
	  "The Intake Office",
	  "\n\tThis is where you first arrived. Its cold and office-like,\n"
	  "\tthere are some file cabinets, a desk,and a window. Another\n"
	  "\tdoor leads out the front, to the " +
		  GF + "north" + DEF +
		  ". This is a strange\n"
		  "\tplace indeed.",
	  "\n\tNot sure where everyone went to, you see the front door, facing\n"
	  "\tnorth of here. It doesn't even look like this place was used at "
	  "all.\n",
	  "\n\tYou see the intake office. Strange.. you don't notice anyone\n"
	  "\tin there."));

  intake->set_exits("west", 1);
  intake->set_exits("north", 4);

  std::shared_ptr<SpaceRoom> outside(std::make_shared<SpaceRoom>(
	  "Outside..",
	  "\n\tYou're outside, or so you thought. For miles all around,\n"
	  "\tas far as you can see, there are small prison facilities\n"
	  "\texactly like the one you're standing in. Each one is\n"
	  "\tsurrounded by what looks like some faintly visible " +
		  GF + "forcefield" + DEF +
		  ".\n\n"
		  "\tThe door behind you has shut.\n"
		  "\tYou hear it lock.",
	  "\n\tIts soul crushing. In the yard next to yours there is a skeleton\n"
	  "\thuddled and leaning against the office door.",
	  "\n\tYou look through window.. You see a skyline dotted with offices."));

  prison_map.emplace_back(cell1);
  prison_map.emplace_back(hallway);
  prison_map.emplace_back(cell2);
  prison_map.emplace_back(intake);
  prison_map.emplace_back(outside);
  world_map["prison_map"] = prison_map;

  std::vector<std::shared_ptr<SpaceRoom>> school_map;
  std::shared_ptr<SpaceRoom> death_school(std::make_shared<SpaceRoom>(
	  "School",
	  "\n\t'Death stalks our every breath' was the whisper you were\n"
	  "\thearing when you arrived here. It is dark in this place.\n"
	  "\tIts so dark that it seems to consume all the light that you\n"
	  "\timagine would even dare to be here.",
	  "\n\tIts as if the very darkness around you even consumes sound, "
	  "because\n"
	  "\tthere is nothing to be heard. Nothing more than the phantom feelings\n"
	  "\tof moving yourself around. No light. No sound. Nothing.",
	  "\n\tFrom here it looks like a very tiny point of light."));
	death_school->set_exits("south", 1);
  std::shared_ptr<SpaceRoom> library(std::make_shared<SpaceRoom>());
	library->room_name = "Library";
	library->description = "\n\tAlibrary with no books. Convenient.";
	library->set_exits("north", 0);
	library->set_exits("east", 2);
	library->set_exits("west", 3);
	library->set_exits("south", 4);
  std::shared_ptr<SpaceRoom> side_east(std::make_shared<SpaceRoom>());
	side_east->room_name = "East Room";
	side_east->description = "\n\tSome eastern room.";
	side_east->set_exits("west", 1);
  std::shared_ptr<SpaceRoom> side_west(std::make_shared<SpaceRoom>());
	side_west->room_name = "West Room";
	side_west->description = "\n\tSome western type of room.";
	side_west->set_exits("east", 1);
  std::shared_ptr<SpaceRoom> main_room(std::make_shared<SpaceRoom>());
	main_room->room_name = "Main Room";
	main_room->description = "\n\tWow.. this must be the main room.";
	main_room->set_exits("north", 1);

  school_map.emplace_back(death_school);
  school_map.emplace_back(library);
  school_map.emplace_back(side_east);
  school_map.emplace_back(side_west);
  school_map.emplace_back(main_room);
  world_map["school_map"] = school_map;

  return world_map;
}
