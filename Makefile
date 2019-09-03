all:
	g++ StatusFetcher.cpp main.cpp -o AutoWater -lcurl -ljsoncpp -lncurses -std=c++11 

clean:
	rm AutoWater

