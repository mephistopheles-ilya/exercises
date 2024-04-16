#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>


#include <iostream>

using std::cout;
using std::endl;
using std::cin;
using std::string;

#define PORT    5555
#define BUFLEN  512

int   readFromClient(int fd, char *buf);
void  writeToClient (int fd);

int  main (void)
{
    int     i, err;
    int     sock, new_sock;
    struct  sockaddr_in  addr;
    struct  sockaddr_in  client;
    char    buf[BUFLEN];
    socklen_t  size;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        cout << "Server: cannot create socket" << endl;
        return -1;
    }

    int opt = 1;
    setsockopt(sock, SOL_SOCKET,SO_REUSEADDR, reinterpret_cast<const void*>(&opt), sizeof(opt));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    err = bind(sock, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr));
    if(err < 0)
    {
        cout << "Server: cannot bind socket" << endl;
        return -1;
    }

    err = listen(sock, 100);
    if(err < 0)
    {
        cout << "Server: listen queue failure" << endl;
        return -1;
    }

    pollfd  act_set[100];
    act_set[0].fd = sock;
    act_set[0].events = POLLIN;
    act_set[0].revents = 0;
    int num_set = 1;

    while(true)
    {
        cout << "Waiting for messages..." << endl;
        int ret = poll(act_set, num_set, -1);
        if(ret <= 0)
        {
            cout << "Server: poll  failure" << endl;
            return -1;
        }
        if(ret > 0)
        {
            for(i = 0; i < num_set; ++i)
            {
                if(act_set[i].revents & POLLIN)
                {
                    cout << "get POLLIN at fd " << act_set[i].fd << endl;
                    act_set[i].revents &= ~POLLIN;
                    if(i ==0)
                    {
                        size = sizeof(client); 
                        new_sock = accept(act_set[i].fd, reinterpret_cast<struct sockaddr*>(&client), &size);
                        cout << "new client at port " <<  ntohs(client.sin_port) << endl;
                        if (num_set < 100)
                        {
                            act_set[num_set].fd = new_sock;
                            act_set[num_set].events = POLLIN;
                            act_set[num_set].revents = 0;
                            ++num_set;
                        }else
                        {
                            cout << "no more sockets for client" << endl;
                            close(new_sock);
                        }
                    }else
                    {
                        err = readFromClient(act_set[i].fd, buf);
                        if(err < 0 || strstr(buf,"stop"))
                        {
                            cout << "####################################### GET STOP at fd " << act_set[i].fd << endl << endl;
                            close (act_set[i].fd);
                            if(i < num_set - 1)
                            {
                                act_set[i] = act_set[num_set - 1];
                                --num_set;
                                --i;
                            }
                        }else
                        {
                            writeToClient(act_set[i].fd);
                        }
                    }  
                }
            }
        }
    }
    
    return 0;
}



int  readFromClient (int fd, char *buf)
{
    int  nbytes;
    nbytes = read(fd,buf,BUFLEN);
    if(nbytes < 0)
    {
        cout << "Server: read failure" << endl;
        return -1;
    }else if(nbytes == 0)
    {
        cout << "Read nbytes = 0" << endl;
        return -1;
    }else
    {
        cout << "Server got message: " << buf << endl;
        return 0;
    }
}



void  writeToClient (int fd)
{
    int  nbytes;
    string message;

    cout << "Enter your message: ";
    getline(cin, message);
    nbytes = write(fd, message.c_str(), strlen(message.c_str()) + 1);
    if(nbytes < 0)
    {
        cout << "Server: write failure" << endl;
    }else
    {
        cout << "Send " << nbytes << " bytes" << endl;
    }

}
