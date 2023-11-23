#include "Manager.h"

Manager::Manager() {
	isActive = false;  
}

Manager::~Manager() {
	camera.release();  
}

void Manager::help() {
	cout << "'help' - Show all commands" << endl
		<< "'photo' - Make photo " << endl
		<< "'info' - Show information about webcamera" << endl
		<< "'server' - Client send photo to you" << endl
		<< "'exit' - Exit" << endl;
}

void Manager::makePhoto() {
	Mat image;         
	camera.open(0);    
	if (!camera.isOpened()) {
		cout << "Error: failed to open webcam" << endl;
		exit(EXIT_FAILURE);
	}
	isActive = true;   

	camera >> image;              
	imwrite("./Photo/photo.jpg", image); 

	camera.release();  
	isActive = false;   
}

void Manager::aboutCamera() {
	Camera camera;

	cout << endl;
	cout << setw(32) << left << "Driver name: " << "| " << setw(48) << camera.getDriverName() << endl
		<< setw(32) << left << "Driver version: " << "| " << setw(48) << camera.getDriverVersion().substr(10) << endl
		<< setw(32) << left << "Webcam name: " << "| " << setw(48) << camera.getData(SPDRP_FRIENDLYNAME) << endl
		<< setw(32) << left << "Producer: " << "| " << setw(48) << camera.getData(SPDRP_MFG) << endl
		<< setw(32) << left << "Class: " << "| " << setw(48) << camera.getData(SPDRP_CLASS) << endl
		<< setw(32) << left << "Class GUID: " << "| " << setw(48) << camera.getData(SPDRP_CLASSGUID) << endl
		<< setw(32) << left << "Compatibleids: " << "| " << setw(48) << camera.getData(SPDRP_COMPATIBLEIDS) << endl
		<< setw(32) << left << "Device power data: " << "| " << setw(48) << camera.getData(SPDRP_DEVICE_POWER_DATA) << endl
		<< setw(32) << left << "Device description: " << "| " << setw(48) << camera.getData(SPDRP_DEVICEDESC) << endl
		<< setw(32) << left << "Enumerator name: " << "| " << setw(48) << camera.getData(SPDRP_ENUMERATOR_NAME) << endl
		<< setw(32) << left << "Hardware ID: " << "| " << setw(48) << camera.getData(SPDRP_HARDWAREID) << endl
		<< setw(32) << left << "Location information: " << "| " << setw(48) << camera.getData(SPDRP_LOCATION_INFORMATION) << endl
		<< setw(32) << left << "Location paths: " << "| " << setw(48) << camera.getData(SPDRP_LOCATION_PATHS) << endl
		<< setw(32) << left << "Lower filters: " << "| " << setw(48) << camera.getData(SPDRP_LOWERFILTERS) << endl
		<< setw(32) << left << "Physical device object name: " << "| " << setw(48) << camera.getData(SPDRP_PHYSICAL_DEVICE_OBJECT_NAME) << endl
		<< setw(32) << left << "Service: " << "| " << setw(48) << camera.getData(SPDRP_SERVICE) << endl << endl;
}

void Manager::doServer() {
	//Server server = Server();

	//server.openServer();
	//server.connectServer();

	server.sendMessage("photo");
	Sleep(5000);
	server.getPhoto();
	//server.sendMessage("disconnect");
}

bool Manager::doCommand(string command) {
	if (command == "") return false;
	if (commands.find(command) == commands.end())
		cout << "Error: invalid command" << endl;

	if (command == "photo") {
		console.hide();
		this->makePhoto();
	}
	if (command == "server") {
		this->doServer();
	}
	if (command == "info") {
		this->aboutCamera();
	}
	if (command == "help") {
		this->help();
	}
	if (command == "exit") {
		return true;
	}
	return false;
}

Console& Manager::getConsole() {
	return console;
}
