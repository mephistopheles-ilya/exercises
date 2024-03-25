#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>

#include <iostream>
#include "header.h"
#include "classes.h"

using std::cout;
using std::endl;

//#define  PORT_THIS      5556
#define  PORT_TO        5555    
#define  BUFLEN         512
char     buf[BUFLEN];


int main(int argc, char** argv)
{
    if(argc != 4)
    {
        cout << "wrong amount of argumets" << endl;
        return 1;
    }

    int sock;
    int nbytes;
    unsigned int  size;
    struct sockaddr_in addr_to;   
    struct sockaddr_in addr_from; 
    struct sockaddr_in addr_this;
    struct hostent *hostinfo;
    
    hostinfo = gethostbyname("127.0.0.1");
    if (hostinfo == nullptr)
    {
        cout << "Problem with IP" << endl;
        return -1;
    }

    addr_to.sin_family = hostinfo->h_addrtype;
    addr_to.sin_port = htons(PORT_TO);
    addr_to.sin_addr = *(reinterpret_cast<struct in_addr*>(hostinfo -> h_addr));

    addr_this.sin_family = AF_INET;
    addr_this.sin_addr.s_addr = INADDR_ANY;
    addr_this.sin_port = htons(atoi(argv[1]));

    sock = socket(PF_INET,SOCK_DGRAM,0);
    if (sock < 0)
    {
        cout << "Problem with socket" << endl;
        return -1;
    }

    int opt = 1;
    setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,(char*)&opt,sizeof(opt));

    int err;
    err = bind(sock, reinterpret_cast<struct sockaddr*>(&addr_this), sizeof(addr_this));
    if (err < 0)
    {
        cout << "Problem with bind" << endl;
        return -1;
    }

    std::fstream in, out;
    in.open(argv[2], std::ios::in);
    out.open(argv[3], std::ios::out);
    if(!in.is_open() || !out.is_open())
    {
        cout << "cannot open file" << endl;
        close(sock);
        return 1;
    }

    string str_to_read;

    while(std::getline(in, str_to_read))
    {
        strcpy(buf, str_to_read.c_str());
        size = strlen(buf);

        nbytes = sendto(sock, buf, strlen(buf) + 1, 0, reinterpret_cast<struct sockaddr*>(&addr_to), sizeof(addr_to));

        if(nbytes < 0)
        {
            out << "Cannot send data" << endl;
            close(sock);
            in.close();
            out.close();
            return -1;
        } else 
        {
            out << "Send mesasge of " << nbytes << " bytse" << endl;
        }

        size = sizeof(addr_from);
        nbytes = recvfrom(sock, buf, BUFLEN, 0, reinterpret_cast<struct sockaddr*>(&addr_from), &size);
        if(nbytes > 0)
        {
            out << buf << endl;
        }else
        {
            out << "cannnot  receive data" << endl;
            close(sock);
            in.close();
            out.close();
            return -1;
        }

        if(fcntl(sock, F_SETFL, O_NONBLOCK)) cout << "Socket unblocked" << endl;
        else cout << "fnctl failed" << endl;

        int i;
        for(i = 0; true; ++i)
        {
            size = sizeof(addr_from);
            nbytes = recvfrom(sock, buf, BUFLEN, 0, reinterpret_cast<struct sockaddr*>(&addr_from), &size);
            if(nbytes > 0)
            {
                out << buf << endl;
            }else 
            {
                break;
            }
            usleep(2000);
        }

        int flags = fcntl(sock, F_GETFL, 0);
        flags &= ~O_NONBLOCK;
        if(fcntl(sock, F_SETFL, flags)) cout << "Socket bloked" << endl;
        else cout << "fcntl failed" << endl;

        out << "Received " << i << " objects" << endl << endl;
    }
    close(sock);
    in.close();
    out.close();
    return 0;
}

