#include "cab.h"

// Check if maintenance_required, maintenance_completed, rest_completed
bool Cab::prepareCabForTrip() {
	
	if(isMaintenanceRequired()) {
		return false;
	}
	else if(d_cabState == AVAILABLE) {
		return true;
	}
	else if( (d_cabState == TRIP_COMPLETED)&& isRestComplete() ) {	
		return true;	
	}
	else if( (d_cabState == UNDER_MAINTENANCE)&& isMaintenanceComplete() ) {	
		return true;		
	}

	return false;
}

bool Cab::isMaintenanceComplete() {
	if(getCurTime() - d_lastMaintenanceTime >= d_maintenanceDuration) {
		d_cabState = AVAILABLE; 
		d_lastMaintenanceTime = getCurTime();
		return true;
	}
	else {
		return false;
	}
}

bool Cab::isRestComplete() {	
	if(getCurTime() - d_tripEndTime >= d_restDuration) {
		d_cabState = AVAILABLE; 
		return true;
	}
	else {
		return false;
	}
}

bool Cab::isMaintenanceRequired() {
	uint64_t t2 = getCurTime();	
	if(t2 - d_lastMaintenanceTime >= d_maintenanceWindow) {
		d_cabState = UNDER_MAINTENANCE;
		d_lastMaintenanceTime = getCurTime();	
		return true;	
	}
	else {
		return false;
	}
}

uint64_t Cab::getCurTime() const {
	return chrono::duration_cast<chrono::seconds>(chrono::system_clock::now().time_since_epoch()).count();
}
