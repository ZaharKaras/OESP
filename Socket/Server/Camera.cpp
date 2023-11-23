#include "Camera.h"

string Camera::getData(DWORD property) {
	char data[256] = "";
	SP_DEVINFO_DATA deviceInfoData = { 0 };
	HDEVINFO deviceInfoSet = SetupDiGetClassDevsA(&GUID_DEVCLASS_CAMERA, "USB", NULL, DIGCF_PRESENT);

	if (deviceInfoSet == INVALID_HANDLE_VALUE) {
		cout << "Error: cannot get device list";
		exit(EXIT_FAILURE);
	}

	deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
	SetupDiEnumDeviceInfo(deviceInfoSet, 0, &deviceInfoData);
	SetupDiGetDeviceRegistryPropertyA(deviceInfoSet, &deviceInfoData, property, NULL, (PBYTE)data, sizeof(data), 0);
	SetupDiDestroyDeviceInfoList(deviceInfoSet);

	return string(data);
}

string Camera::getDriverName() {
	char* driverName = new char[256];
	char driverVersion[256];

	strcpy_s(driverName, 256, "");
	
	if (!capGetDriverDescriptionA(0, driverName, 255, driverVersion, 255)) {
		cout << "Error: Webcam driver not found";
	}

	return string(driverName);
}

string Camera::getDriverVersion() {
	char driverName[256];
	char* driverVersion = new char[256];

	strcpy_s(driverVersion, 256, "");
	
	if (!capGetDriverDescriptionA(0, driverName, 255, driverVersion, 255)) {
		cout << "Error: Webcam driver not found";
	}

	return string(driverVersion);
}
