#include "server/server.hpp"

#include <iostream>
#include <stdexcept>
#include <stdio.h>
#ifdef __linux__
	#include <unistd.h>
	#include <signal.h>
	#include <stdlib.h>
#endif
#ifdef _WIN32
	#include <windows.h>
#endif

godotserver::Server* server = nullptr;

#ifdef _WIN32
	BOOL WINAPI CtrlHandler(DWORD fdwCtrlType){
		switch (fdwCtrlType){
			case CTRL_C_EVENT:
				server->shutdown();
				//throw std::runtime_error("terminating\n");
				return true;
			case CTRL_CLOSE_EVENT:
				return true;
			case CTRL_BREAK_EVENT:
				return false;
			case CTRL_LOGOFF_EVENT:
				return false;
			case CTRL_SHUTDOWN_EVENT:
				return false;
			default:
				return false;
			}
	}
#endif

#ifdef __linux__
	void signal_handler(int s){
		if(s == 2){
			server->shutdown();
			//throw std::runtime_error("terminating\n");
		}
	}
#endif

int main(){
	server = new godotserver::Server;
	try{
		#ifdef __linux__
			struct sigaction sigint_handler;
			sigint_handler.sa_handler = signal_handler;
			sigemptyset(&sigint_handler.sa_mask);
			sigint_handler.sa_flags = 0;
			sigaction(SIGINT, &sigint_handler, NULL);
		#endif
		#ifdef _WIN32
			if(!SetConsoleCtrlHandler(CtrlHandler, true)){
				throw std::runtime_error("can't set control handler");
			}
		#endif
		server->run();
	}catch(const std::exception& e){
		std::cerr << e.what() << '\n';
        delete server;
		#ifdef _WIN32
			system("pause");
		#endif
		return 1;
	}
    delete server;
	printf("server terminatted\n");
	#ifdef _WIN32
		system("pause");
	#endif
    return 0;
}