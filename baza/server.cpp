#include "classes.h"

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


#define  PORT          5555
#define  BUFLEN         512
char     buf[BUFLEN];

int main(void)
{

    Baza baza;
    baza.read_currency("file_currency");
    baza.read_records("file_baza");
    cout << endl;

    int sock;
    //int nbytes;;
    //unsigned int  size;
    //struct sockaddr_in addr_from;
    struct sockaddr_in addr_this;
    //struct hostent *hostinfo;

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

        vector<list<Man>::iterator> v1;
        vector<list<Operations>::iterator> v2;

        while (true)
        {

            size = sizeof(addr_from);
            nbytes = recvfrom(sock, buf, BUFLEN, 0, reinterpret_cast<struct sockaddr*>(&addr_from), &size);
            if(nbytes > 0)
            {
                cout << "Get message from" << endl;
                cout << "type " << addr_from.sin_family << endl;
                cout << "port " << ntohs(addr_from.sin_port) << endl;
                cout << "ip addr " << inet_ntoa(addr_from.sin_addr) << endl;
            }else
            {
                cout << "Cannot receive message"<< endl;
                break;
            }

            cout << "bytse: " << nbytes << endl << buf << endl << endl;

            string message;
            baza.work_with_str(message, v1, v2, string(buf));
            if(v1.size() > 0 || v2.size() > 0) 
            {
                string s;
                s += "found " + std::to_string(v1.size() + v2.size()) + " objects";
                nbytes = sendto(sock, s.c_str(), strlen(s.c_str()) + 1, 0, reinterpret_cast<struct sockaddr*>(&addr_from), sizeof(addr_from));
            }else if(message.size() > 0)
            {
                nbytes = sendto(sock, message.c_str(), strlen(message.c_str()) + 1, 0, reinterpret_cast<struct sockaddr*>(&addr_from), sizeof(addr_from));
            }else
            {
                string s = "Not found or deleted";
                nbytes = sendto(sock, s.c_str(), strlen(s.c_str()) + 1, 0, reinterpret_cast<struct sockaddr*>(&addr_from), sizeof(addr_from));
            }

            for(size_t i = 0; i < v1.size(); ++i)
            {
                string s = v1[i] -> create_string();
                nbytes = sendto(sock, s.c_str(), strlen(s.c_str()) + 1, 0, reinterpret_cast<struct sockaddr*>(&addr_from), sizeof(addr_from));
            }
            for(size_t i = 0; i < v2.size(); ++i)
            {
                string s = v2[i] -> create_string();
                nbytes = sendto(sock, s.c_str(), strlen(s.c_str()) + 1, 0, reinterpret_cast<struct sockaddr*>(&addr_from), sizeof(addr_from));
            }
            
            v1.clear();
            v2.clear();
        }
    }

    close(sock);
    return 0;
}

