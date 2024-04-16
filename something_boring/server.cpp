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

using std::cout;
using std::endl;
using std::cin;
using std::string;

#define  PORT          5555
#define  BUFLEN         512
char     buf[BUFLEN];

int main(void)
{
    int sock;
    struct sockaddr_in addr_this;

    addr_this.sin_family = AF_INET;
    addr_this.sin_addr.s_addr = INADDR_ANY;
    addr_this.sin_port = htons(PORT);

    sock = socket(PF_INET,SOCK_DGRAM,0);
    if (sock < 0)
    {
        cout << "Problem with socet" << endl;
        return -1;
    }

    int opt = 1;
    setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,(char*)&opt,sizeof(opt));

    int err;
    err = bind(sock, reinterpret_cast<struct sockaddr*>(&addr_this), sizeof(addr_this));
    if (err < 0)
    {
        cout << "Problem with socket" << endl;
        return -1;
    }
 
    {
        socklen_t size;
        int nbytes;
        struct sockaddr_in addr_from;

        while (true)
        {
            cout << "Waiting for message..." << endl;
            size = sizeof(addr_from);
            nbytes = recvfrom(sock, buf, BUFLEN, 0, reinterpret_cast<struct sockaddr*>(&addr_from), &size);
            if(nbytes > 0)
            {
                cout << "Get message from port: " << ntohs(addr_from.sin_port) << endl;
            }else
            {
                cout << "Cannot receive message"<< endl;
                break;
            }

            cout << "Message: " <<  buf << endl;

            string message;
            cout << "Enter your message: ";
            getline(cin, message);
            nbytes = sendto(sock, message.c_str(), strlen(message.c_str()) + 1, 0, reinterpret_cast<struct sockaddr*>(&addr_from), sizeof(addr_from));
            cout << "Send " << nbytes << " bytes" << endl;
        }
    }

    close(sock);
    return 0;
}

