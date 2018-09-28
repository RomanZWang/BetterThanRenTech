#include <iostream>
#include <fstream>
#include <chrono>

#include <cpr/cpr.h>
#include <json.hpp>
#include "hash-library/sha256.h"

void calcAndOutput(const std::string& src){
	std::cout << "src : \"" << src << "\"\n";
}

int main(int argc, char** argv) {
    std::string burl = "https://api.binance.com/api";
    std::string ep1 = "/api/v1/ticker/allPrices";
    std::string ep2 = "/api/v3/account";
    std::string data_query_string = "recvWindow=20000&timestamp=1519211811670";

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

		SHA256 sha256object;

    std::cout << "Public Key: " << public_key << std::endl << "Secret Key: " << secret_key << std::endl;
		std::cout << "Hash of Secret Key: " << sha256object(secret_key) << std::endl;
		return 0;
}
