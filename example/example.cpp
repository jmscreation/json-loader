/*
    Author:     jmscreator
    License:   Always free to use forever (See MIT License)

    This is an example program which demonstrates how to use the rapidjson wrapper

*/

#include "../jsonloader.h" // rapidjson parsing library wrapper

#include <fstream> // load from the file
#include <iostream> // cout + string
#include <vector>


// Simple struct to load the json data into
struct Person {
    std::string name;
    float age;

    struct Hobby { // hobby struct
        std::string name;
        uint32_t rating;
    };

    std::vector<Hobby> hobbies;
    std::vector<std::string> inventory;
};

std::vector<Person> people; // list of people loaded into memory


bool LoadPerson(const std::string& path) {
    std::ifstream file(path, std::ios::binary | std::ios::in); // input file

    if(!file.is_open()){
        std::cout << "failed to load \"" << path << "\"\n";
        return false;
    }

    std::string rawData; // where to store raw json data

    do {
        char chunk[1024]; // 1kb chunk
        std::streamsize len = file.readsome(chunk, sizeof(chunk)); // load a chunk of data

        if(len > 0){
            rawData.append(chunk, len); // copy-append data onto rawData
            continue;
        }

        break; // end of file
    } while(file.good());

    file.close();

    if(!json::parseData(rawData.data(), rawData.size())){ // parse the raw json data
        std::cout << "failed to parse json data\n";
        return false;
    }
    
    Person person;

    if(!json::loadProperty("name", person.name)) std::cout << "failed to load person's name\n";
    if(!json::loadProperty("age", person.age)) std::cout << "failed to load person's age\n";

    json::Object hobbies;
    if(json::loadProperty("hobbies", hobbies)){
        for(auto it = hobbies.MemberBegin(); it != hobbies.MemberEnd(); ++it){ // iterate over hobbies
            if(!it->name.IsString()) continue; // skip non-string hobbies
            
            Person::Hobby hobby;
            hobby.name = it->name.GetString(); // load hobby key
            if(json::loadProperty(hobbies, hobby.name.data(), hobby.rating)){ // load hobby rating value (using json::loadProperty instead of hobby.value because of the type)
                person.hobbies.emplace_back(hobby); // add hobby to person's hobby list
            }
        }
    } else {
        std::cout << "failed to load person's hobbies\n";
    }

    json::Array inventory;
    if(json::loadPropertyArray("inventory", inventory)){
        for(auto it = inventory.Begin(); it != inventory.End(); ++it){ // iterate over inventory array
            std::string item; // where to store the string
            if(json::loadValue(*it, item)){ // load string from array element
                person.inventory.push_back(item); // add inventory item to person's inventory list
            }
        }
    } else {
        std::cout << "failed to load person's inventory\n";
    }

    people.emplace_back(person); // push the person onto the list of people

    return true;
}

int main() {
    json::displayErrors = true;
    
    // load people files
    LoadPerson("person1.json");
    LoadPerson("person2.json");

    // print the information for each person
    for(Person& person : people){
        std::cout << person.name << ":\n"
                << "\tage: " << person.age << "\n"
                << "\thobbies:" << "\n";

        for(Person::Hobby& hobby : person.hobbies){
            std::cout << "\t\t" << hobby.name << " -> " << hobby.rating << " / 10\n";
        }
        
        std::cout << "\tinventory:\n";
        for(std::string& item : person.inventory){
            std::cout << "\t\t" << item << "\n";
        }
    }
    
    return 0;
}