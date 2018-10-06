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

// Planning
// Inputs: Coin tickers, exchanges
// Methods:
// Panic sell
//	List of conditions
// Find arbitrage existence (Coin Tickers, exchanges, max loop size)
//	Matrix of values
// 	Find closed loop so there is no risk
// Execute trade (list of ticker/exchange tuples)
//
//	print expected profit and current time
// 	move money to maximize trade? may increase transaction time and is too complicated. diversify? Or only base operations around one cryptocoin.
// 	print actual profit and trade execution time
// how to ensure we have money to make trade?
// Things to consider: Fees, Latency, Panic Conditions
// Exchange specific buy trade sell instructions to abstract

// Key methods planned to be implemented when program is generalized to multiple exchanges
// void panic(){}
//
// std::vector<std::vector<long>> *tradeMatrix createTradeMatrix(std::vector<std::string> *tickers, std::vector<std::string> *exchanges){}
//
// bool hasCycle(std::vector<std::string> *exchanges, std::vector<std::string> *tickers){}
//
// std::vector<std::tuple<string, string>> *tickerExchangeVector findArbitrage(std::vector<std::vector<long>> *tradeMatrix, int cycleSize, long fee){}
//
// bool executeTrade(std::vector<std::tuple<string, string>> *tickerExchangeVector){}
//
// bool diversify(std::vector<std::string> *coinsToHoldVector){}


// Methods with 3 coins on one exchange while holding in one coin
bool shouldPanic(){
	// check for panic conditions
	return false;
}

void panic(){
	//liquidate assets
}

bool hasArbitrage(std::vector<std::string> *coinsToTradeVector, double fee){
	return false;
}

double getAmount(string ticker){
	return 0;
}

bool executeTrade(std::vector<std::string> *tradeTickersVector, double amount){
	return false;
}

bool limitReached(int limitSec, int limitDay){
	return false;
}

void run(){
	//initialize constants
	std::vector<std::string> coinsToTradeVector{ "BTC", "ETH", "XRP" };
	std::vector<std::string> exNamesForward;
	std::vector<std::string> exNamesBackward;
	int numCoins = coinsToTradeVector.size();
	double amount;
	double fee = 0.001;
	//api request to get info
	// exchange limits per second
	int requestLimitSec = 100;
	int orderLimitSec = 100;
	int orderLimitDay = 100;
	int orders = 0;

	for(int i = 0; i<numCoins; i++){
		exNamesForward.push_back(coinsToTradeVector.at(i)+coinsToTradeVector.at((i+1)%numCoins));
		exNamesBackward.push_back(coinsToTradeVector.at(numCoins-i-1)+coinsToTradeVector.at((numCoins-i-2)%numCoins));
	}

	while(!limitReached(orderLimitSec, orderLimitDay)){
		if(hasArbitrage(exNamesForward)){
			amount = getAmount("BTC");
			executeTrade(std::vector<std::string> *exNamesForward, amount)
			orders +=3;
		}
		else if (hasArbitrage(exNamesBackward)){
			amount = getAmount("BTC");
			executeTrade(std::vector<std::string> *exNamesForward, amount)
			orders +=3;
		}
		else{
			std::cout<<"No arbitrage opportunity at this moment."<<std::endl;
		}
		sleep( 1/ (double) requestLimitSec);
	}

	//loop template
	for(int i = 0; i<numCoins; i++){

	}
}

void startBot(){
	run();
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

		auto response3 = cpr::Get(
			cpr::Url{"api.binance.com/api/v1/exchangeInfo"}
		);

		auto json3 = nlohmann::json::parse(response3.text);
		std::cout << json3.dump(4) << std::endl;

		// Print out preliminary variables
		std::cout << "Public Key: " << public_key << std::endl;
		std::cout << "Hash of Secret Key: " << secret_key_hash << std::endl;
		std::cout << "Full request url: " << request_url << std::endl;

		return 0;
}
