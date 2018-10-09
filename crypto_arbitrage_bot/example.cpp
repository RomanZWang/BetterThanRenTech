#include <iostream>
#include <fstream>
#include <ctime>
#include <chrono>
#include <thread>

#include <cpr/cpr.h>
#include <json.hpp>
#include "hash-library/sha256.h"
#include "hash-library/hmac.h"

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

class ArbitrageBot {
    // Access specifier
		private:
			std::vector<std::string> *coinsToTradeVector;
			std::vector<std::string> *exNamesForward;
			std::vector<std::string> *exNamesBackward;
			// NEED NUMCOINS int numCoins = coinsToTradeVector.size();
		public:
			double balance;
			double fee;
			//api request to get info
			// exchange limits per second
			int requestLimitSec;
			int orderLimitSec;
			int orderLimitDay;

			//TODO Implement rate limiter
			//https://stackoverflow.com/questions/28008549/limit-while-loop-to-run-at-30-fps-using-a-delta-variable-c
			//http://www.cplusplus.com/forum/general/71038/
			std::chrono::high_resolution_clock::time_point startTime;

			//Class related methods
			ArbitrageBot(std::vector<std::string> *coinNames):requestLimitSec(200), orderLimitSec(10), orderLimitDay(10000){
				coinsToTradeVector = coinNames;
				exNamesForward = new std::vector<std::string>();
				exNamesBackward = new std::vector<std::string>();
				updateCoinTradeNames();
			};
			//TODO Create destructor
			void updateCoinTradeNames();

			//API call helper methods
			void calcAndOutput(const std::string& src);
			long int unix_timestamp();
			double getBalance(std::string ticker);

			//Panic methods
			bool shouldPanic();
			void panic();

			//Trade operation methods
			bool hasArbitrage();
			bool executeTrade(std::vector<std::string> *tradeTickersVector, double amount);

			//TODO modify parameters as necessary
			bool limitReached(int limitSec, int limitDay);
			void run();


};

void ArbitrageBot::updateCoinTradeNames(){
	int numCoins = this->coinsToTradeVector->size();

	for(int i = 0; i<numCoins; i++){
		exNamesForward->push_back(this->coinsToTradeVector->at(i) + this->coinsToTradeVector->at((i+1)%numCoins));
		exNamesBackward->push_back(this->coinsToTradeVector->at((numCoins-i)%numCoins) + this->coinsToTradeVector->at(numCoins-i-1));
	}
}

void ArbitrageBot::calcAndOutput(const std::string& src){
	std::cout << "src : \"" << src << "\"\n";
}

long int ArbitrageBot::unix_timestamp(){
    time_t t = std::time(0);
    long int now = static_cast<long int> (t);
    return now;
}


// DUMMY
double ArbitrageBot::getBalance(std::string ticker){
	std::ifstream public_key_file("../key_store/public.txt");
	std::string public_key;
	if(public_key_file.good()){
		std::getline(public_key_file, public_key);
	}
	std::string secret_key;
	std::ifstream secret_key_file("../key_store/secret.txt");
	if(secret_key_file.good()){
		std::getline(secret_key_file, secret_key);
	}

	std::string burl = "https://api.binance.com";
	std::string ep2 = "/api/v3/account";
	std::string unix_epoch_time_now = std::to_string(unix_timestamp()*1000);
	std::string data_query_string = "recvWindow=20000&timestamp=" + unix_epoch_time_now;

	SHA256 sha256object;
	std::string secret_key_hash = hmac<SHA256>(data_query_string, secret_key);

	std::stringstream russ;
	russ << burl << ep2 << "?" << data_query_string << "&signature=" << secret_key_hash;
	std::string request_url = russ.str();

	auto response2 = cpr::Get(
		cpr::Url{request_url},
		cpr::Header{{"X-MBX-APIKEY", public_key}}
	);

	auto json2 = nlohmann::json::parse(response2.text);

	if(json2["balances"]["BTC"])
		return json2["balances"]["BTC"];
	return -1;
}


// DUMMY
bool ArbitrageBot::shouldPanic(){
	// check for panic conditions
	return false;
}

// DUMMY
void ArbitrageBot::panic(){
	//liquidate assets
}

// DUMMY
bool ArbitrageBot::hasArbitrage(){
	return false;
}


// DUMMY
bool ArbitrageBot::executeTrade(std::vector<std::string> *tradeTickersVector, double amount){
	return false;
}

// DUMMY
bool ArbitrageBot::limitReached(int limitSec, int limitDay){
	return false;
}

void ArbitrageBot::run(){

	//TODO Change period to value parsed from API request
	int period = ceil(1000/ (double) requestLimitSec);
	int orders = 0;
	double balance;
	// Run bot
	while(!limitReached(orderLimitSec, orderLimitDay)){
		if(hasArbitrage()){
			balance = getBalance("BTC");
			executeTrade(exNamesForward, balance);
			orders +=3;
		}
		else if (hasArbitrage()){
			balance = getBalance("BTC");
			executeTrade(exNamesForward, balance);
			orders +=3;
		}
		else{
			std::cout<<"No arbitrage opportunity at this moment."<<std::endl;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(period));
		std::cout<<period<<std::endl;
	}
}

int main(int argc, char** argv) {

	std::vector<std::string> coins{ "BTC", "ETH", "XRP" };
	ArbitrageBot firstBot(&coins);
	firstBot.run();
	return 0;
}
