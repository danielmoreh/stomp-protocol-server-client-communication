#include <stdlib.h>
#include "../include/ConnectionHandler.h"
#include "../include/summary.h"
#include "../include/user.h"
#include "../include/StompProtocol.h"
#include <thread>

static void threadRun(StompProtocol &protocol, ConnectionHandler &connectionHandler);

int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		std::cerr << "Usage: " << argv[0] << " host port" << std::endl
				  << std::endl;
		return -1;
	}

	std::string host = argv[1];
	short port = atoi(argv[2]);
	while (1)
	{
		// big loop from here
		ConnectionHandler connectionHandler(host, port);
		if (!connectionHandler.connect())
		{
			std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
			return 1;
		}
		StompProtocol protocol;
		while (1)
		{ // will be true until the user connects to the server
			const short bufsize = 1024;
			char buf[bufsize];
			std::cin.getline(buf, bufsize);
			std::string line(buf);
			if (line.find("login") != std::string::npos)
			{
				protocol.handleIO(line, connectionHandler);
				string answer;
				if (!connectionHandler.getLine(answer))
				{
					std::cout << "Disconnected. Exiting...\n"
							  << std::endl;
				}
				protocol.handleFrameFromServer(answer, connectionHandler);
				break;
			}
		}
		if (!protocol.shouldTerminate())
		{
			std::thread t([&protocol, &connectionHandler]()
						  { threadRun(protocol, connectionHandler); });
			while (!protocol.shouldTerminate())
			{
				const short bufsize = 1024;
				char buf[bufsize];
				std::cin.getline(buf, bufsize);
				std::string line(buf);
				protocol.handleIO(line, connectionHandler);
				if (line.find("logout") != std::string::npos){
					//sleep to insure that the client will get the receipt of the logout frame
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
				}
			}
			//cout << "terminating current connection" << endl;
			t.join();
		}
	}
	return 0;
}

static void threadRun(StompProtocol& protocol, ConnectionHandler& connectionHandler){
	while(!protocol.shouldTerminate()){
		string answer;
		if (!connectionHandler.getLine(answer)) {
			std::cout << "Disconnected. Exiting...\n" << std::endl;
			break;
		}
		protocol.handleFrameFromServer(answer, connectionHandler);

	}
}