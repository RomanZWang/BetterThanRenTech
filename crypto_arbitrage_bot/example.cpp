#include <iostream>
#include <fstream>
#include <ctime>
#include <chrono>

#include <cpr/cpr.h>
#include <json.hpp>
#include "hash-library/sha256.h"
#include "hash-library/hmac.h"

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
		std::string unix_epoch_time_now = std::to_string(unix_timestamp()*1000);
		//std::string unix_epoch_time_now = (std::chrono::system_clock::now().time_since_epoch()).count();
    std::string data_query_string = "recvWindow=20000&timestamp=" + unix_epoch_time_now;

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

		// Calculate SHA256 key
		SHA256 sha256object;
		std::string secret_key_hash = hmac<SHA256>(data_query_string, secret_key);

		// Create URL request string
		std::stringstream russ;
		russ << burl << ep2 << "?" << data_query_string << "&signature=" << secret_key_hash;
		std::string request_url = russ.str();

		// Test out the API GET request to binance
    auto response = cpr::Get(cpr::Url{"api.binance.com/api/v1/ticker/allPrices"});
    auto json = nlohmann::json::parse(response.text);
    std::cout << json.dump(4) << std::endl;

		// Try signed API GET request
		auto response2 = cpr::Get(
			cpr::Url{request_url},
			cpr::Header{{"X-MBX-APIKEY", public_key}}
		);

		auto json2 = nlohmann::json::parse(response2.text);
		std::cout << json2.dump(4) << std::endl;

		// Print out preliminary variables
		std::cout << "Public Key: " << public_key << std::endl;
		std::cout << "Hash of Secret Key: " << secret_key_hash << std::endl;
		std::cout << "Full request url: " << request_url << std::endl;



		return 0;
}
