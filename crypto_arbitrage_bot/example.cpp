#include <iostream>

#include <cpr/cpr.h>
#include <json.hpp>
#include "PicoSHA2/picosha2.h"

#include <fstream>



int main(int argc, char** argv) {
    std::string burl = "api.binance.com/api/";
    auto response = cpr::Get(cpr::Url{"api.binance.com/api/v1/ticker/allPrices"});
    auto json = nlohmann::json::parse(response.text);
    std::cout << json.dump(4) << std::endl;

    std::ifstream public_key_file("../key_store/public.txt");
    std::string public_key;
    if (public_key_file.good()){
      std::getline(public_key_file, public_key);
    }

    std::string secret_key;
    std::ifstream secret_key_file("../key_store/secret.txt");
    if (secret_key_file.good()){
      std::getline(secret_key_file, secret_key);
    }

    std::cout << public_key << std::endl << secret_key << std::endl;

}
