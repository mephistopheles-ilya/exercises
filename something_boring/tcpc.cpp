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

using std::cout;
using std::endl;
using std::cin;
using std::string;

#define  SERVER_PORT     5555
#define  SERVER_NAME    "127.0.0.1"
#define  BUFLEN          512

int  writeToServer(int fd);
int  readFromServer(int fd);

int  main(void)
{
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

    while(true) 
    {
        if(writeToServer(sock) < 0) break;
        if(readFromServer(sock) < 0) break;
    }
    cout << "The end" << endl << endl;

    close (sock);
    return 0;
}



int  writeToServer (int fd)
{
    int   nbytes;
    char  buf[BUFLEN];
    string str_to_read;

    cout << "Enter your message: ";
    if(!getline(cin, str_to_read))
    {
        return -1;
    }

    strcpy(buf, &str_to_read[0]);

    nbytes = write(fd, buf, strlen(buf) + 1);
    if(nbytes < 0)
    {
        cout << "Problems with writing" << endl;
        return -1;
    }else
    {
        cout << "Send to server " << nbytes << " bytes" << endl;
    }
    if(strstr(buf,"stop")) return -1;
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

