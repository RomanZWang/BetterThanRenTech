#include <iostream>
#include <fstream>
#include <ctime>

#include <cpr/cpr.h>
#include <json.hpp>
#include "hash-library/sha256.h"

void calcAndOutput(const std::string& src){
	std::cout << "src : \"" << src << "\"\n";
}

long int unix_timestamp()
{
    time_t t = std::time(0);
    long int now = static_cast<long int> (t);
    return now;
}

int main(int argc, char** argv) {
		// Base URL variable initialization
    std::string burl = "https://api.binance.com";
    std::string ep1 = "/api/v1/ticker/allPrices";
    std::string ep2 = "/api/v3/account";
		std::string unix_epoch_time_now = std::to_string(unix_timestamp());
    std::string data_query_string = "recvWindow=20000&timestamp=";

		// Test out the API GET request to binance
    auto response = cpr::Get(cpr::Url{"api.binance.com/api/v1/ticker/allPrices"});
    auto json = nlohmann::json::parse(response.text);
    std::cout << json.dump(4) << std::endl;

		// Get keys from hidden key files
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

		// Print out preliminary variables
		std::cout << "Public Key: " << public_key << std::endl;
		std::cout << "Hash of Secret Key: " << secret_key_hash << std::endl;
		std::cout << "Full request url: " << request_url << std::endl;

		// Calculate SHA256 key
		SHA256 sha256object;
		std::string secret_key_hash = sha256object(secret_key);
		std::stringstream russ;

		// Create URL request string
		russ << burl << ep2 << "?" << data_query_string << "&signature=" << secret_key_hash;
		std::string request_url = russ.str();

		return 0;
}
