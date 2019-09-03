#ifdef __APPLE__
	#include <json/json.h>
#else
	#include <jsoncpp/json/json.h>
#endif

#include <string>
#include <iostream>
#include <string.h>
#include <curl/curl.h>
#include "StatusFetcher.h"

StatusFetcher::StatusFetcher()
{
}

StatusFetcher::~StatusFetcher()
{
}

bool StatusFetcher::fetchStatus() {
    
    bool retVal = false;
    
    curl_global_init(CURL_GLOBAL_ALL);
    CURLcode res;
    CURL* easyhandle = curl_easy_init();
    std::string readBuffer;
    Json::CharReaderBuilder builder;
    Json::CharReader * reader = builder.newCharReader();

    Json::Value root;
    std::string jsonErrors;
    
    curl_easy_setopt(easyhandle, CURLOPT_URL, _url.c_str());
    curl_easy_setopt(easyhandle, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA, &readBuffer);
    
    res = curl_easy_perform(easyhandle);
    
    if(res == CURLE_OK) {
        bool parsingSuccessful = reader->parse(readBuffer.c_str(), readBuffer.c_str() + readBuffer.size(), &root, &jsonErrors);

        if(parsingSuccessful) {
            
            string waterLevel = Json::Value(root["water"]).asString();
            _waterLevel = std::stoi(waterLevel);
            
            string moistureLevel = Json::Value(root["moisture"]).asString();
            _moistureLevel = std::stoi(moistureLevel);
            
            retVal = true;
        
        } else {
            std::cout << "Response error: " << readBuffer << std::endl;
        }
    } else {
        std::cout << "HTTP error: " << res << std::endl;
    }
    
    curl_easy_cleanup(easyhandle);
    
    return retVal;
}

size_t StatusFetcher::WriteCallback(char *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int StatusFetcher::getMoistureLevel() {
    int maxValue = 65;
    int realMoistureLevel = _moistureLevel * (10.0 / maxValue);
    return realMoistureLevel;
}

int StatusFetcher::getWaterLevel() {
  
  int waterLevelMin = 4;
  int waterLevelMax = 24;
  
  float realWaterScale = (waterLevelMax - waterLevelMin) / 10.0;
  
  int offsetWaterLevel = _waterLevel - waterLevelMin;
  int realWaterLevel = (waterLevelMax - waterLevelMin) - offsetWaterLevel;
  int realWaterValue =  realWaterLevel / realWaterScale;
  
  return realWaterValue;
}