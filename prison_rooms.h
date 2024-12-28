#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include "space_items.h"


class SpaceRoom {
private:
    /* data */
public:
    std::string room_name;
    std::string description;
    std::string details;
    std::string near_room_info;
    std::map<std::string, int> exits;
    std::string look_info;
    std::vector<ItemData> room_items;

    void set_exits(std::string direction, int next_room){
        exits.insert(std::make_pair(direction, next_room));
    }

    void set_room_items(ItemData item){
        room_items.emplace_back(item);
    }

    void getItem(ItemData item){
    }

    void display(){
        using namespace std;
        cout << endl;
        cout << "        " << CF+room_name+DEF << endl;
        cout << "      -------------------------" << endl;
        cout << " " << description << endl;
        for (auto item : room_items) {
            cout << "\t- " << item.item_name << endl;
        }
        cout << endl;
    }

};

std::vector<SpaceRoom> loadrooms() {
    std::shared_ptr<vItem_t> world_items = load_items();
    std::vector<SpaceRoom> roomMap;
    SpaceRoom room0;
    room0.room_name = "Cell";
    room0.description = "\n\tThis is a very small prison cell. It is smelly\n"
                        "\tand empty, with the exception of one small cot.";
    room0.details = "\n\tOh its horrible.\nAnd there's no toilet in here.";
    room0.near_room_info = "\n\tYou see a dirty prison cell.";
    room0.set_exits("south", 1);    
    room0.set_room_items(*world_items->at(1));
    
    SpaceRoom room1;
    room1.room_name = "Hallway";
    room1.description = "\n\tThis hallway leads to freedom, or wherever you came from.\n"
                        "\tOkay okay, it leads outside at least.";
    room1.details = "\n\tThe walls here have fresh paint on them, but "+RF+"blood"+DEF+" stains on the\n"
                    "\tconcrete floor leading to the intake office. "+RF+"It"+DEF+" seems as though\n"
                    "\tthere is only one cell in this place.      "+RF+"___'_"+DEF;
    room1.near_room_info = "\n\tYou see a hallway, wit"+RF+"h bloodst"+DEF+"ains on the floor.";    
    room1.set_exits("north", 0);
    room1.set_exits("east", 2);

    SpaceRoom room2;
    room2.room_name = "The Intake Office";
    room2.description = "\n\tThis is where you first arrived. Its cold and office-like,\n"
                        "\tthere are some file cabinets, a desk,and a window. Another\n"
                        "\tdoor leads out the front, to the "+GF+"north"+DEF+". This is a strange\n"
                        "\tplace indeed.";
    room2.details = "\n\tNot sure where everyone went to, you see the front door, facing\n"
                    "\tnorth of here. It doesn't even look like this place was used at all.\n";
    room2.near_room_info = "\n\tYou see the intake office. Strange.. you don't notice anyone\n"
                            "\tin there.";
    room2.set_exits("west", 1);
    room2.set_exits("north", 3);
    
    SpaceRoom room3;
    room3.room_name = "Outside..";
    room3.description = "\n\tYou're outside, or so you thought. For miles all around,\n"
                        "\tas far as you can see, there are small prison facilities\n"
                        "\texactly like the one you're standing in. Each one is\n"
                        "\tsurrounded by what looks like some faintly visible "+GF+"forcefield"+DEF+".\n\n"
                        "\tThe door behind you has shut.\n"
                        "\tYou hear it lock.";
    room3.details = "\n\tIts soul crushing. In the yard next to yours there is a skeleton\n"
                    "\thuddled and leaning against the office door.";
    room3.near_room_info = "\n\tYou look through window.. You see a skyline dotted with offices.";
    //room3.set_room_items(*world_items->at(2));

    SpaceRoom room4;
    room4.room_name = "School";
    room4.description = "\n\t'Death stalks our every breath' was the whisper you were\n"
                        "\thearing when you arrived here. It is dark in this place.\n"
                        "\tIts so dark that it seems to consume all the light that you\n"
                        "\timagine would even dare to be here.";
    room4.details = "\n\tIts as if the very darkness around you even consumes sound, because\n"
                    "\tthere is nothing to be heard. Nothing more than the phantom feelings\n"
                    "\tof moving yourself around. No light. No sound. Nothing.";
    room4.near_room_info = "\n\tFrom here it looks like a very tiny point of light.";
    
    roomMap.emplace_back(room0);
    roomMap.emplace_back(room1);
    roomMap.emplace_back(room2);
    roomMap.emplace_back(room3);
    roomMap.emplace_back(room4);
    return roomMap;
}
