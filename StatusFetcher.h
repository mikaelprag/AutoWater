#ifndef STATUSFETCHER_H
#define STATUSFETCHER_H

#include <string>

#ifdef __APPLE__
	#include <json/json.h>
#else
	#include <jsoncpp/json/json.h>
#endif

using namespace std;

enum COLORS {
    WATER = 1,
    MOISTURE = 2,
    WHITE = 3,
    NEUTRAL = 4
};

class StatusFetcher
{
private:
    int _waterLevel;
    int _moistureLevel;
    string _timeLevel;
    
    const string _url = "http://water.local/status";
    static size_t WriteCallback(char *contents, size_t size, size_t nmemb, void *userp);
    
public:
    StatusFetcher();
    ~StatusFetcher();
    
    int getWaterLevel();
    int getMoistureLevel();
    string getTimeLevel();
    
    bool fetchStatus();

};

#endif // STATUSFETCHER_H
