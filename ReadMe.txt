
Assumptions:

1) Car booking system has two kinds of inputs - user_input and trip_events. For simplicity, we are feeding both these from the command line

2) For simplicity, we assume that the commands entered are error-free; cabIds are auto-generated sequentially starting from 0; cityId are non-negative, but need not be sequential

3) Build from rootdir - make

4) Run from rootdir - ./poral.out

5) Following is a glossary of commands/events
A) REGISTER_CAB <cityId> - If cityId is not registered, both new cabId and the entered cityId are registered. Re-registering the same cityId at later stage has no effect
B) REGISTER_CITY <cityId> - Register city if not already registered
C) BOOK_CAB <originCityId> <destCityId> - Both most idle available cab in originCityId to go to destinationCityId. Both source and dest cityIds must be registered
D) TRIP_COMPLETED <cabId> <destCityId> - This is an event generated when the cab completes the trip. The system is updated accordingly. Once the trip is completed there is rest time before next trip commences
E) CHANGE_CITY <cabId> <newCityId> - Transfer the car to new city only if it is available
F) STATUS - Print the Stationary Cabs in cities(Available/Resting/Under_Maintenance), In Transit Cabs, Registered Cities ordered by how busy they are(num of bookings)

4) State Diag for Cab State Change
AVAILABLE ->(BOOK_CAB)-> IN_TRANSIT ->(TRIP_COMPLETED)-> TRIP_COMPLETED(Resting) 
 |								|
 V								V
 MAINTENANCE(?)							MAINTENANCE(?) ->Yes-> UNDER_MAINTENANCE
 |								|
 V								V
 UNDER_MAINTENANCE						No->Res_Time-> AVAILABLE 
