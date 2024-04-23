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
#include <climits>


#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::vector;

#define PORT    5555
#define BUFLEN  512

int   readFromClient(int fd, char *buf);
void  writeToClient (int fd, int N);
struct Client
{
    int N;
    int counter;
    int fd;
    int i;
    bool received;
    Client(int fd, int i): N(INT_MAX), counter(INT_MAX), fd(fd), i(i), received(false) {}
};


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

    vector<Client> clients;
    clients.reserve(100);
    while(true)
    {
        sleep(1);
        int ret = poll(act_set, num_set, 0);
        if(ret > 0)
        {
            for(i = 0; i < num_set; ++i)
            {
                if(act_set[i].revents & POLLIN)
                {
                    //cout << "get POLLIN at fd " << act_set[i].fd << " i = " << i << endl;
                    act_set[i].revents &= ~POLLIN;
                    if(i ==0)
                    {
                        size = sizeof(client); 
                        new_sock = accept(act_set[i].fd, reinterpret_cast<struct sockaddr*>(&client), &size);
                        //cout << "new client at port " <<  ntohs(client.sin_port) << endl;
                        if (num_set < 100)
                        {
                            act_set[num_set].fd = new_sock;
                            clients.emplace_back(new_sock, num_set);
                            act_set[num_set].events = POLLIN;
                            act_set[num_set].revents = 0;
                            ++num_set;
                        }else
                        {
                            //cout << "no more sockets for client" << endl;
                            close(new_sock);
                        }
                    }else
                    {
                        readFromClient(act_set[i].fd, buf);
                        int N = atoi(buf);
                        clients[i - 1].N = N;
                        clients[i - 1].counter = N;
                        clients[i - 1].received = true;
                    }  
                }
            }
        }
        for(size_t j = 0; j < clients.size(); ++j)
        {
            cout << j << ' ' << "N = " << clients[j].N << " counter = " << clients[j].counter << " fd = " << clients[j].fd << " i = " << clients[j].i << endl;
        }

        cout << "SIZE = " << clients.size() << " CAPASITY = " << clients.capacity() << endl;
        for(size_t j = 0; j < clients.size(); ++j)
        {
            if(clients[j].received)
            {
                if(clients[j].counter <= 1) 
                {
                    writeToClient(clients[j].fd, clients[j].N);
                    close(clients[j].fd);
                    if(j != (clients.size() - 1))
                    {
                        act_set[clients[j].i] = act_set[num_set - 1];
                        clients[clients.size() - 1].i = clients[j].i;
                        clients[j] = clients[clients.size() - 1];
                        --j;
                    }
                    --num_set;
                    clients.pop_back();
                }else
                {
                    --clients[j].counter;
                }
            }
        }
        for(size_t j = 0; j < clients.size(); ++j)
        {
            cout << j << ' ' << "N = " << clients[j].N << " counter = " << clients[j].counter << " fd = " << clients[j].fd << " i = " << clients[j].i << endl;
        }
        cout << endl;


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
        //cout << "Server got message: " << buf << " from fd = " <<  fd <<  endl;
        return 0;
    }
}



void  writeToClient (int fd, int N)
{
    int  nbytes;
    string message;

    message = "Your time " + std::to_string(N) + " ended";
    nbytes = write(fd, message.c_str(), strlen(message.c_str()) + 1);
    if(nbytes < 0)
    {
        cout << "Server: write failure" << endl;
    }else
    {
       // cout << "Send " << message << " to fd = " << fd << endl;
    }

}
