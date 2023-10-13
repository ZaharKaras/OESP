#pragma once
#include "libraries.h"

using namespace std;

class Battery
{
private:
	string powerSupply;
	string savingMode;
	string stateCharge;
	int percent;
	int lifeTime;

public:
	Battery();
	string getPowerSupply(SYSTEM_POWER_STATUS status);
	string getStateCharge(SYSTEM_POWER_STATUS status);
	string getSavingMode(SYSTEM_POWER_STATUS status);
	int getPercent();
	friend string BatteryStatus(Battery battery);
};
