#ifndef PORTAL
#define PORTAL
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <chrono>
#include <set>
#include <deque>
#include <sstream>
#include <unistd.h>
#include "cab.h"
using namespace std;

class Portal {

private:
 
	map<uint64_t, deque<uint64_t> > cityCabsMap;	
	vector<Cab> cabs;
	set<uint64_t> inTransitCabs;
	vector<pair<uint64_t,uint64_t> > cities;	
	uint64_t curCabId;	
	
	void registerCab(uint64_t cityId);
	void registerCity(uint64_t cityId);	
	
	bool changeCabCity(uint64_t cabId, uint64_t newcity);
	bool changeCabState(uint64_t cabId, CabState cabstate);
	bool isCityRegistered(uint64_t cityId) const;
	
	void bookCab(uint64_t cityId, uint64_t destId);	
	void processTripCompleted(uint64_t cabId, uint64_t cityId);	
	
	uint64_t getCurTime() const;	
	
public:	

	Portal():curCabId(0) {}
	
	void processInput(string event);
	void printStatus();
};

// Comparator to order cities by number of bookings
class Compare {

public:
	bool operator()(const pair<uint64_t, uint64_t> &a, const pair<uint64_t, uint64_t> &b) const {
		return a.second < b.second;
	} 
};
#endif
