#include <algorithm>
#include "portal.h"

// process commands
void Portal::processInput(string event) {
	
	stringstream ss(event);
	string str;	
	getline(ss, str, ' ');	
	if(str == "REGISTER_CAB") {
		getline(ss, str, ' ');
		uint64_t cityId = stoull(str);	
		registerCab(cityId);
	}
	else if(str == "REGISTER_CITY"){
		getline(ss, str, ' ');
		uint64_t cityId = stoull(str);
		registerCity(cityId);
	}
	else if(str == "BOOK_CAB") {
		getline(ss, str, ' ');
		uint64_t originId = stoull(str);
		getline(ss, str, ' ');
		uint64_t destId = stoull(str);
		bookCab(originId, destId);
	}
	else if(str == "TRIP_COMPLETED") {
		getline(ss, str, ' ');
		uint64_t cabId = stoull(str);
		getline(ss, str, ' ');
		uint64_t destId = stoull(str);
		processTripCompleted(cabId, destId);
	}
	else if(str == "CHANGE_CITY") {
		getline(ss, str, ' ');
		uint64_t cabId = stoull(str);
		getline(ss, str, ' ');
		uint64_t newCityId = stoull(str);
		changeCabCity(cabId, newCityId);
	}
	else if(str == "STATUS") {
		printStatus();		
	} 
}

// REGISTER_CAB, auto-assign next available cabId
void Portal::registerCab(uint64_t cityId) {
	
	Cab c(cityId, curCabId);	
	cabs.push_back(c);		
	cityCabsMap[cityId].push_back(curCabId);
	++curCabId;
	if(!isCityRegistered(cityId)) {
		pair<uint64_t, uint64_t> p(cityId, 0);
		cities.push_back(p);
	}
}

// REGISTER_CITY - city may have no registered cabs
void Portal::registerCity(uint64_t cityId) {
	if(!isCityRegistered(cityId)) {
		pair<uint64_t, uint64_t> p(cityId, 0);
		cities.push_back(p);
	}		
}

// TRIP_COMPLETED
void Portal::processTripCompleted(uint64_t cabId, uint64_t destId) {	
	
	cabs[cabId].tripEndTime() = getCurTime(); 
	if(!cabs[cabId].isMaintenanceRequired()) {
		cabs[cabId].cabState() = TRIP_COMPLETED;
	}

	inTransitCabs.erase(cabId);	
	cityCabsMap[destId].push_back(cabId);	
}

// CHANGE_CITY - Change city only for available cabs
bool Portal::changeCabCity(uint64_t cabId, uint64_t newCityId) {
	
	if(cabs[cabId].cabState() != AVAILABLE) {
		cout << "City for Cab: " << cabId << " cannot be changed as it is not AVAILABLE" << endl;
		return false;
	}	
	
	uint64_t oldCityId = cabs[cabId].cityId();
	cabs[cabId].cityId() = newCityId;
	deque<uint64_t> &dq = cityCabsMap[oldCityId];
	dq.erase(remove(dq.begin(), dq.end(), cabId));

	if(cityCabsMap.find(newCityId) == cityCabsMap.end()) {
		cityCabsMap[newCityId].push_back(cabId);
	}
	else {
		deque<uint64_t> tmp = cityCabsMap[newCityId];
		for(deque<uint64_t>::iterator it=tmp.begin(); it != tmp.end(); ++it) {
			uint64_t cId = *it;
			if(cabs[cId].tripEndTime() >= cabs[cabId].tripEndTime()) {
				tmp.insert(it, cabId);
				break;
			}
		}
		cityCabsMap[newCityId] = tmp;
	}
	return true;	
}

// BOOK_CAB
void Portal::bookCab(uint64_t originId, uint64_t destId) {
	if(cityCabsMap.find(originId) == cityCabsMap.end()) {
		cout << "City " << originId << " has No Cabs"  << endl;
		return;
	}
	
	deque<uint64_t> dq = cityCabsMap[originId];	
	uint64_t c = -1;
	for(uint64_t i=0; i !=dq.size(); ++i) {
		uint64_t cabId = dq[i];
		if(cabs[cabId].prepareCabForTrip()) {
			c = cabId;
			dq.erase(dq.begin()+i);
			break; 
		}
	}

	if(c == -1ULL) {
		cout << "City " << originId << " has cabs, but none of them are Available to be booked" << endl;
		return;
	}

	if(dq.empty()) {
		cityCabsMap.erase(originId);
	}
	else {
		cityCabsMap[originId] = dq;
	}

	Cab &cab = cabs[c];
	cab.cabState() = IN_TRANSIT;
	cab.destId() = destId;
	cab.tripStartTime() = getCurTime();
	inTransitCabs.insert(cab.cabId());
	for(uint64_t i=0; i<cities.size(); ++i) {
		if(cities[i].first == originId) {
			++cities[i].second;
		}
	}
	cout << "Cab: " << cab.cabId() << " booked from City " << originId << " to City " << destId << endl;		
}

// STATUS - print available cabs in different cities, cabs in transit and cities ordered by num of bookings
void Portal::printStatus()  {
	cout << "\nCitywise Stationary Cabs:" << endl;
	for(map<uint64_t, deque<uint64_t> >::iterator it = cityCabsMap.begin(); it != cityCabsMap.end(); ++it) {
		cout << "City: " << it->first << endl;
		deque<uint64_t> dq = it->second;
		for(uint64_t i=0; i<dq.size(); ++i) {
			uint64_t cabId = dq[i];
			cabs[cabId].prepareCabForTrip();
			Cab c = cabs[cabId];
			if(c.cabState() == AVAILABLE) {
				cout << "Cab: " << c.cabId() << " Status: " << "Available" << endl;
			}
			else if(c.cabState() == TRIP_COMPLETED) {
				cout << "Cab: " << c.cabId() << " Status: " << "Resting" << endl;
			}
			else if(c.cabState() == UNDER_MAINTENANCE) {
				cout << "Cab: " << c.cabId() << " Status: " << "Under_Maintenance" << endl;
			}
		}
		cout << endl;
	}	

	if(!inTransitCabs.empty()) {	
		cout << "In Transit Cabs:" << endl;
		for(set<uint64_t>::iterator it=inTransitCabs.begin();it != inTransitCabs.end(); ++it) {
			Cab c = cabs[*it];
			cout << "Cab: " << c.cabId() << " From City: " << c.cityId() << " to City: " << c.destId() << endl;
		}
	}
	
	make_heap(cities.begin(), cities.end(), Compare());
	cout<< "\nCities Ordered By Demand (CityId:Numbookings): " << endl;		
	for(uint64_t i=0; i<cities.size(); ++i) {
		cout << cities[i].first << ":" << cities[i].second << " ";	
	}
	cout << endl << endl;
}

bool Portal::changeCabState(uint64_t cabId, CabState newCabState) {
	
	if(cabId >= cabs.size()) {
		return false;
	}	
	
	cabs[cabId].cabState() = newCabState;
	return true;	
}

bool Portal::isCityRegistered(uint64_t cityId) const {
	for(uint64_t i=0; i<cities.size(); ++i) {
		if(cities[i].first == cityId) {
			return true;
		}
	}

	return false;
}

uint64_t Portal::getCurTime() const {
	return chrono::duration_cast<chrono::seconds>(chrono::system_clock::now().time_since_epoch()).count();
}
