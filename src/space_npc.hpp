#pragma once

class Npc {
private:
    std::shared_ptr<ItemData> heldItem;
    int location;
public:
    std::string name;
    std::string type;
    int health = 5;

    Npc() {}
    Npc(std::shared_ptr<ItemData> eHeld, 
        int&                      eLocation,
        std::string&              eName,        
        std::string&              eType
        )
        :heldItem(eHeld)
        ,location(eLocation)
        ,name(eName)
        ,type(eType) {}
    
    void look_being() {
        std::cout << "    - A " << YF + type + DEF << " person is standing here.";
    }

    void look_details() {
        std::cout << "      You see a " << YF + type + DEF << " :" << std::endl;
        std::cout << "  Who's name is: " << GF + name + DEF << ".. " << std::endl;
        std::cout << "  Health: " << health << std::endl;
        std::cout << "  and they're standing here, holding a " << heldItem->item_name << "." << std::endl;
        std::cout << std::endl;
    }

};

typedef std::shared_ptr<std::map<std::string, Npc>> p_MapNpc;
p_MapNpc loading_npc() {
    std::vector<std::shared_ptr<ItemData>> world_items = load_items();
    p_MapNpc npc_beings(std::make_shared<std::map<std::string, Npc>>());

    std::string name = "Draig";
    std::string type = "blipian";
    int location = 2;    
    Npc draig(world_items.at(0), location, name, type);
    npc_beings->insert(std::make_pair(draig.type, draig));

    return npc_beings;
}