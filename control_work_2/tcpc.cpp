#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <iostream>

/*
 * first parametr is id
 * second parametr is amount of numbers
 * to send id "id=?'
 * 
 */



using std::cout;
using std::endl;
using std::string;

#define  SERVER_PORT     5555
#define  SERVER_NAME    "127.0.0.1"
#define  BUFLEN          512

int  writeToServer(int fd, char* K, char* N );
int  readFromServer(int fd);

int  main(int argc, char** argv)
{
    if(argc != 3)
    {
        cout << "Wrong amount of arguments" << endl;
        return -1;
    }

    int err;
    int sock;
    struct sockaddr_in server_addr;
    struct hostent    *hostinfo;

    hostinfo = gethostbyname(SERVER_NAME);
    if(hostinfo == nullptr)
    {
        cout << "Unknown host " << SERVER_NAME << endl;
        return -1;
    }

    server_addr.sin_family = PF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr = *(reinterpret_cast<struct in_addr*>(hostinfo -> h_addr));

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        cout << "Client: socket was not created" << endl;
        return -1;
    }

    err = connect (sock, reinterpret_cast<struct sockaddr*>(&server_addr), sizeof(server_addr));
    if(err < 0)
    {
        cout << "Client:  connect failure" << endl;
        return -1;
    }
    cout << "Connection is ready" << endl;

    writeToServer(sock, argv[1], argv[2]);
    readFromServer(sock);

    cout << "The end" << endl << endl;

    close (sock);
    return 0;
}



int  writeToServer (int fd, char* K, char* N)
{
    int   nbytes;
    string my_id(K);
    string message;
    int amount = std::atoi(N);
    int sum = 0, current = 0;

    message = "id=" + string(K);
    nbytes = write(fd, message.c_str(), message.size() + 1);
    if(nbytes > 0)
    {
        cout << "Send to server " << nbytes << " bytes, " << message << endl;
    }
    srand(time(NULL));
    for(int i = 0; i < amount; ++i)
    {
        sleep((rand()%4));
        current = rand()%1000;
        sum += current;
        message = std::to_string(current);
        nbytes = write(fd, message.c_str(), message.size() + 1);
        if(nbytes < 0)
        {
            cout << "Problems with writing" << endl;
            return -1;
        }else
        {
            cout << "Send to server " << nbytes << " bytes, " <<  "Number = " <<  message << endl;
        }
    }
    //sleep(2);
    message = std::to_string(-1);
    nbytes = write(fd, message.c_str(), message.size() + 1);
    if(nbytes > 0)
    {
        cout << "Send to server " << nbytes << " bytes, " << message << endl;
    }

    std::cout << "My number = " << sum << std::endl;
    return 0;
}


int  readFromServer (int fd)
{
    int   nbytes;
    char buf[BUFLEN];

    cout << "Wainting for reply..." << endl;
    nbytes = read(fd, buf, BUFLEN);
    if(nbytes > 0)
    {
        cout << "Get message: " << buf << endl; 
    }else
    {
        cout << "Problems with reading" << endl;
        return -1;
    }

    return 0;
}

