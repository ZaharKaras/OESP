#include "Battery.h"

Battery::Battery() {
	SYSTEM_POWER_STATUS status;
	GetSystemPowerStatus(&status);
	this->powerSupply = getPowerSupply(status);
	this->percent = status.BatteryLifePercent;
	this->lifeTime = status.BatteryLifeTime;
	this->stateCharge = getStateCharge(status);
	this->savingMode = getSavingMode(status);
}

string Battery::getPowerSupply(SYSTEM_POWER_STATUS status) {
	if (status.ACLineStatus == 1) {
		return "mains";
	}
	else if (status.ACLineStatus == 0) {
		return "battery";
	}
	else {
		return "unknown";
	}
}

string Battery::getStateCharge(SYSTEM_POWER_STATUS status) {
	switch (status.BatteryFlag) {
	case 0:
		return "not charging";
	case 1:
		return "high ( > 66% )";
	case 2:
		return "low ( > 20% )";
	case 4:
		return "critical ( < 5% )";
	case 8:
		return "charging";
	case 128:
		return "No system battery";
	default:
		return "unknown";
	}
}

string Battery::getSavingMode(SYSTEM_POWER_STATUS status) {
	if (status.SystemStatusFlag) {
		return "on";
	}
	return "off";
}

int Battery::getPercent()
{
	return percent;
}

string BatteryStatus(Battery battery)
{
	std::ostringstream oss;
	oss << "BATTERY" << std::endl
		<< "  Power supply: " << battery.powerSupply << std::endl
		<< "  State of charge: " << battery.stateCharge << std::endl
		<< "  Percentage: " << battery.percent << "%" << std::endl
		<< "  Saving mode: " << battery.savingMode << std::endl
		<< "  Life time: " << battery.lifeTime << "s" << std::endl;
	oss << std::endl;

	return oss.str();
}

