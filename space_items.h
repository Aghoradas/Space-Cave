// Class and associated data about items.
// Update - version:2 (expanding item classifications- wearables)

#pragma once
#include <iostream>
#include <memory>
#include <vector>


class ItemData
{
public:
    std::string item_name;
    std::string item_descrip;
    std::string item_key;
    bool wearable = false;
    bool onOff = false;
    int field_key = 0000;

    ItemData(){};
    ItemData(std::string name, std::string descrip, std::string symbol){
        item_name = name;
        item_descrip = descrip;
        item_key = symbol;
    };
    void look_item() {
        std::cout << "\n\n\tLooking at the item..." << std::endl;
        std::cout << item_descrip << std::endl;
    }

    void flipSwitch(){
        if (onOff == false){
            onOff = true;
            std::cout << "\n\tOh... now that is bright." << std::endl;
        }
        else{
            onOff = false;
            std::cout << "\n\tIt got a little darker in here." << std::endl;
        }
    }

};

class Key : ItemData {
public:
    std::string key_name;
    int field_key;
    Key(){};
    Key(std::string name, std::string descrip, std::string symbol) {
        item_name = name;
        item_descrip = descrip;
        item_key = symbol;
    };
};

typedef std::vector<std::shared_ptr<ItemData>> vItem_t;

std::shared_ptr<vItem_t> load_items(){
    std::shared_ptr<vItem_t> worldItems(new vItem_t);
    
    std::shared_ptr<ItemData> nothing(std::make_shared<ItemData>(
        "nothing",
        "There's nothing in your hands.",
        "nothing"));
    worldItems->emplace_back(nothing);

    std::shared_ptr<ItemData> cot(std::make_shared<ItemData>(
        "A small "+GF+"cot"+DEF,
        "Its a dingy white cot. Its probably as old as this prison.",
        "cot"));
    worldItems->emplace_back(cot);

    std::shared_ptr<ItemData> forcefield(std::make_shared<ItemData>(
        "a forcefield", 
        "This thing surrounds this small prison yard. Its so powerful that can you hear it.", 
        "forcefield"));
    worldItems->emplace_back(forcefield);
    std::shared_ptr<ItemData> cyan_key(std::make_shared<ItemData>(
        "a key-"+CF+"card"+DEF,
        "This card has a cyan glow. It will probably come in handy later.",
        "key"));
    cyan_key->field_key = 1001;
    worldItems->emplace_back(cyan_key);
    return worldItems;
}
