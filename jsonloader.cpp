/*
    Author:     jmscreator
    License:   Always free to use forever (See MIT License)
*/

#include "jsonloader.h"


namespace json {
    /*
        JSON Wrapper Basic Variables
    */

    // public
    bool displayErrors = false;

    // private
    bool _ready = false;
    rapidjson::Document _jsonData;

    /*
        Load External Parameters From JSON Data
    */

   bool loadProperty(const Object& config, const char* name, Object& parameter) {
        if(!_ready){
            if(displayErrors){
                std::cout << "Must parse JSON data before loading property\n";
            }
            return false;
        }

        if(config.IsObject() && config.HasMember(name)){
            if(config[name].IsObject()){
                parameter.CopyFrom(config[name], _jsonData.GetAllocator());
                return true;
            } else {
                if(displayErrors) {
                    std::cout << name << " is not a valid object\n";
                }
            }
        }
        return false;
    }

    bool parseData(const char* data, size_t length) {
        rapidjson::Document& config = _jsonData;
        config.SetNull();
        config.Parse(data, length);
        if(config.GetParseError() != rapidjson::ParseErrorCode::kParseErrorNone){
            if(displayErrors){
                std::cout << "JSON Failed to parse - Error @" << config.GetErrorOffset()
                          << " -> " << rapidjson::GetParseError_En(config.GetParseError()) << "\n";
            }
            return false;
        }
        if(!config.IsObject()){
            if(displayErrors){
                std::cout << "Initial JSON must be an object\n";
            }
            return false;
        }
        _ready = true;
        return true;
    }

    void clearData() {
        _jsonData.Clear();
        _ready = false;
    }
}