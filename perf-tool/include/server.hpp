
#ifndef SERVER_H_
#define SERVER_H_

#include <ctime>
#include <thread>
#include <poll.h>
#include <vector>

#include "serverClients.hpp"
#include "json.hpp"

using json = nlohmann::json;

struct delayed_command_t
{
    std::time_t delayTill;
    json command;
};

class Server
{
    /*
     * Data members
     */
protected:
public:
private:
    int serverSocketFd, activeNetworkClients = 0, portNo;
    bool headlessMode = true, keepPolling = true;
    struct pollfd pollFds[ServerConstants::BASE_POLLS + ServerConstants::NUM_CLIENTS];
    NetworkClient *networkClients[ServerConstants::NUM_CLIENTS];
    CommandClient *commandClient;
    LoggingClient *loggingClient;
    std::thread perfThread;
    std::vector<delayed_command_t> delayedCommands;

    /*
     * Function members
     */
protected:
public:
    Server(int portNo, std::string commandFileName = "", std::string logFileName = "logs.txt");

    /* Handles server functionality and polling*/
    void handleServer(void);

    /* Handle the message queue, and sends to all clients */
    void handleMessagingClients(std::string message);

    /* Closes all open files, connectend socketfd, and then the server's socket */
    void shutDownServer(void);

private:
    /* Handles recieving commands for the agent from clients */
    void handleClientCommand(const std::string command, const std::string from);

    void execCommand(json command);

    void sendMessage(const int socketFd, const std::string message);

    void startPerfThread(int time);
};

#endif /* SERVER_H_ */
