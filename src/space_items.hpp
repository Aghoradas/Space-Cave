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
    bool        visible = false;
    bool        wearable = false;
    bool        onOff = false;
    int         field_key = 0000;


    ItemData(){};

    ItemData(std::string name, 
             std::string descrip, 
             std::string symbol)
                :item_name(name)
                ,item_descrip(descrip)
                ,item_key(symbol) {}

    ItemData(std::string name, 
             std::string descrip, 
             std::string symbol, 
             bool        worn, 
             bool        on, 
             int         key)
                :item_name(name)
                ,item_descrip(descrip)
                ,item_key(symbol)
                ,wearable(worn)
                ,onOff(on)
                ,field_key(key) {}

    nlohmann::json to_json() const {
        return {
            {"item_name", item_name},
            {"item_descrip", item_descrip},
            {"item_key", item_key},
            {"wearable", wearable},
            {"onOff", onOff},
            {"field_key", field_key}
        };
    }

    ItemData from_json(const nlohmann::json& j) {
        return {
            j.at("item_name").get<std::string>(),
            j.at("item_descrip").get<std::string>(),
            j.at("item_key").get<std::string>(),
            j.at("wearable").get<bool>(),
            j.at("onOff").get<bool>(),
            j.at("field_key").get<int>()
        };
    }

    void look_item() {
        std::cout << "\n\n\tLooking at the item..." << std::endl;
        std::cout << item_descrip << std::endl;
        std::cout << "[press enter]";
        std::cin.get();
    }
};


std::vector<std::shared_ptr<ItemData>> load_items(){
    std::vector<std::shared_ptr<ItemData>> worldItems;
    std::shared_ptr<ItemData> spoon(std::make_shared<ItemData>(
        "spoon",
        "a crude spoon",
        "spoon"));
        spoon->visible = true;
    worldItems.emplace_back(spoon);

    std::shared_ptr<ItemData> cot(std::make_shared<ItemData>(
        "A small "+GF+"cot"+DEF,
        "Its a dingy white cot. Its probably as old as this prison.",
        "cot"));
        cot->visible = true;
    worldItems.emplace_back(cot);

    std::shared_ptr<ItemData> forcefield(std::make_shared<ItemData>(
        "a forcefield",
        "This thing surrounds this small prison yard. Its so powerful that can you hear it.",
        "forcefield"));
    worldItems.emplace_back(forcefield);
    
    std::shared_ptr<ItemData> cyan_key(std::make_shared<ItemData>(
        "a key-"+CF+"card"+DEF,
        "This card has a cyan glow. It will probably come in handy later.",
        "key"));
    //cyan_key->field_key = 1001;
    worldItems.emplace_back(cyan_key);
    return worldItems;
}
