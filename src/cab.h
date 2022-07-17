#ifndef CAB
#define CAB
#include <iostream>
#include <chrono>
using namespace std;

enum CabState{AVAILABLE, IN_TRANSIT, TRIP_COMPLETED, UNDER_MAINTENANCE};

class Cab {

private:
	uint64_t d_cabId;
	uint64_t d_cityId;
	uint64_t d_destId;
	CabState d_cabState;	
	uint64_t d_tripStartTime;
	uint64_t d_tripEndTime;
	uint64_t d_lastMaintenanceTime;
	const uint64_t d_maintenanceWindow; //seconds
	const uint64_t d_maintenanceDuration;	//seconds
	const uint64_t d_restDuration;	//seconds

	uint64_t getCurTime() const;
	
public:
	// For debugging set constants to low value maintenanceWindow(180), maintenanceDuration(30), restDuration(30)
	Cab(uint64_t cityId, uint64_t cabId): d_cabId(cabId), d_cityId(cityId),d_maintenanceWindow(15780000), d_maintenanceDuration(7200), d_restDuration(7200) {
			d_destId=0;
			d_cabState=AVAILABLE;
			d_tripStartTime=0;
			d_tripEndTime=0;
			d_lastMaintenanceTime=getCurTime();		
	}

	//Setters & Getters	
	uint64_t& cabId() {
		return d_cabId;
	}

	uint64_t& cityId() {
		return d_cityId;
	}

	uint64_t& destId() {
		return d_destId;
	}
	
	CabState& cabState() {
		return d_cabState;
	}

	uint64_t& tripStartTime() {
		return d_tripStartTime;
	}

	uint64_t& tripEndTime() {
		return d_tripEndTime;
	}	
	
	uint64_t& lastMaintenanceTime() {
		return d_lastMaintenanceTime;
	}	

	const uint64_t& cabId() const {
		return d_cabId;
	}

	const uint64_t& cityId() const {
		return d_cityId;
	}

	const uint64_t& destId() const {
		return d_destId;
	}
	
	const CabState& cabState() const {
		return d_cabState;
	}

	const uint64_t& tripStartTime() const {
		return d_tripStartTime;
	}

	const uint64_t& tripEndTime() const {
		return d_tripEndTime;
	}	
	
	const uint64_t& lastMaintenanceTime() const{
		return d_lastMaintenanceTime;
	}

	const uint64_t& maintenanceWindow() const {
		return d_maintenanceWindow;
	}
	
	const uint64_t& maintenanceDuration() const {
		return  d_maintenanceDuration; 
	}
	
	const uint64_t& restDuration() const{
		return d_restDuration;
	}

	bool prepareCabForTrip();
	bool isMaintenanceRequired();
	bool isMaintenanceComplete();
	bool isRestComplete();	
};

#endif

