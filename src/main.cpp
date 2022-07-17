#include<iostream>
#include<fstream>
#include<string>
#include "portal.h"
using namespace std;

int main() {	

	Portal *portal = new Portal();	
	string line;
	cout << "Enter one of the below Commands/Events" << endl;	
	while(getline(cin, line)) {
		if(!line.empty()) {
			portal->processInput(line);
		}	
	}
		
	return 0;
}
