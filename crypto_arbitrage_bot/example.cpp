//Standard libraries
#include <iostream>
#include <fstream>
#include <ctime>
#include <chrono>
#include <thread>
#include <cmath>

//External libraries
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
			//TODO Do we need balance attribute?

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
			void updateCoinTradeNames();
			ArbitrageBot(std::vector<std::string> *coinNames):requestLimitSec(200), orderLimitSec(10), orderLimitDay(10000){
				coinsToTradeVector = coinNames;
				exNamesForward = new std::vector<std::string>();
				exNamesBackward = new std::vector<std::string>();
				updateCoinTradeNames();
			};
			//TODO Create destructor

			//API call helper methods
			long int unix_timestamp();
			string getKey(std::string path);
			string getRequestURL(std::string endpoint, std::string data_query_string, bool private);
			double getBalance(std::string ticker);

			//Panic methods
			bool shouldPanic();
			void panic();

			//Trade operation methods
			bool hasArbitrage(std::vector<std::string> *tradeTickersVector);
			bool executeTrade(std::vector<std::string> *tradeTickersVector, double amount);
			bool executeTriangularTrade(std::vector<std::string> *tradeTickersVector, double amount)

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

long int ArbitrageBot::unix_timestamp(){
    time_t t = std::time(0);
    long int now = static_cast<long int> (t);
    return now;
}

string ArbitrageBot::getKey(std::string keyPath){
	std::ifstream key_file(keyPath);
	std::string key;
	if(key_file.good()){
		std::getline(key_file, key);
	}
	return key;
}

string ArbitrageBot::getRequestURL(std::string endpoint, string data_query_string, bool private){


	std::string burl = "https://api.binance.com";

	SHA256 sha256object;
	std::string secret_key_hash = hmac<SHA256>(data_query_string, secret_key);
	std::stringstream russ;
	russ << burl << endpoint << "?" << data_query_string;
	if(private){
		public_key = getKey("../key_store/public.txt");
		secret_key = getKey("../key_store/secret.txt");
		russ<<"&signature=" << secret_key_hash;
	}
	return russ.str();
}

double ArbitrageBot::getBalance(std::string ticker){

	std::string unix_epoch_time_now = std::to_string(unix_timestamp()*1000);
	std::string data_query_string = "recvWindow=20000&timestamp=" + unix_epoch_time_now;
	request_url = getRequestURL("/api/v3/account", data_query_string, true);

	auto response = cpr::Get(
		cpr::Url{request_url},
		cpr::Header{{"X-MBX-APIKEY", public_key}}
	);

	auto json = nlohmann::json::parse(response.text);

	//TODO fix index accessors
	if(json["balances"][ticker])
		return json["balances"][ticker];
	return -1;
}

double ArbitrageBot::getExRate(std::string ticker){

	std::string unix_epoch_time_now = std::to_string(unix_timestamp()*1000);
	std::string data_query_string = "symbol=" + ticker;
	request_url = getRequestURL("/api/v3/ticker/price", data_query_string, false);

	auto response = cpr::Get(
		cpr::Url{request_url},
		cpr::Header{{"X-MBX-APIKEY", public_key}}
	);

	auto json = nlohmann::json::parse(response.text);

	//TODO fix index accessors
	return (double) json["price"];
}

// TODO DUMMY
bool ArbitrageBot::shouldPanic(){
	// check for panic conditions
	return false;
}

// TODO DUMMY
void ArbitrageBot::panic(){
	//liquidate assets
}

bool ArbitrageBot::hasArbitrage(std::vector<std::string> *tradeTickersVector){
	string baseCurrency = tradeTickersVector->at(0);
	double totalFee = std::math(1+this->fee,3);
	double feeFactor = 1/totalFee;
	double arbitrageFactor = 1;
	//determines arbitrage for the three currencies designated
	for(int i = 0; i<tradeTickersVector->size(); i++){
		arbitrageFactor *= tradeTickersVector->at(i);
	}
	arbitrageFactor *= feeFactor;
	//3 exchange rates multiplied should equal 1 for no arbitrage. Arbitrage exists if product is greater than 1.
	return arbitrageFactor > 1;
}


// TODO DUMMY
tradeSuccess(auto json){
	return true;
}

bool ArbitrageBot::executeTrade(string ticker, double amount){
	std::string unix_epoch_time_now = std::to_string(unix_timestamp()*1000);
	std::string data_query_string = "symbol=" + ticker + "&side=BUY&type=MARKET&quantity=" + amount + "&timestamp=" + unix_epoch_time_now;
	request_url = getRequestURL("/api/v3/ticker/price", data_query_string, true);

	auto response = cpr::Post(
		cpr::Url{request_url},
		cpr::Header{{"X-MBX-APIKEY", public_key}}
	);

	auto json = nlohmann::json::parse(response.text);
	return tradeSuccess(json);
	//TODO fix index accessors

}

bool ArbitrageBot::executeTriangularTrade(std::vector<std::string> *tradeTickersVector, double amount){
	double currentBalance;
	string currentTicker;
	for(int i = 0; i<tradeTickersVector->size(); i++){
		currentTicker = tradeTickersVector->at(i);
		balance = getBalance(currentTicker);
		while(executeTrade(currentTicker, balance)){
			balance = getBalance(currentTicker);
		}
	}
	//TODO if timeout value is reached, then return false
	return true;
}

// TODO DUMMY
bool ArbitrageBot::limitReached(int limitSec, int limitDay){
	//TODO make sure we leave room for a few panic sell trades
	return false;
}

void ArbitrageBot::run(){

	//TODO Change period to value parsed from API request
	int period = ceil(1000/ (double) requestLimitSec);
	int orders = 0;
	double balance;
	// Run bot
	//TODO Account for condition where a trade does not go through
	while(!limitReached(orderLimitSec, orderLimitDay)){
		if(shouldPanic()){
			panic();
		}
		else if(hasArbitrage()){
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
