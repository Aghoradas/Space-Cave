#pragma once
#include "space_items.hpp"

class Npc {
private:
    std::shared_ptr<ItemData> heldItem;
    int location;
public:
    std::string name;
    std::string type;

    Npc() {}
    Npc(std::string&              eName, 
        std::shared_ptr<ItemData> eHeld, 
        std::string&              eType, 
        int&                      eLocation)
        :name(eName)
        ,heldItem(eHeld)
        ,type(eType)
        ,location(eLocation) {}
    
    void look_being() {
        std::cout << "    - a " << type << " is standing here.";
    }

};

typedef std::shared_ptr<std::map<std::string, Npc>> p_MapNpc;
p_MapNpc loading_npc() {
    std::shared_ptr<vItem_t> world_items = load_items();
    p_MapNpc npc_beings(std::make_shared<std::map<std::string, Npc>>());

    std::string name = "Craig";
    std::string type = YF + "goblin" + DEF;
    int location = 2;    
    Npc goblin(name, world_items->at(0), type, location);

    npc_beings->insert(std::make_pair(name, goblin));

    return npc_beings;
}