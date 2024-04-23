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

struct Client
{
    int id;
    unsigned int sum;
    int fd;
    bool end;
    Client(int fd): id(0), sum(0), fd(fd), end(false) {}
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
        int nbytes;
        size_t flag = 0;
        for(Client& x: clients)
        {
            flag += x.end;
        }
        if((flag > 0) && (flag == (clients.size())))
        {
            string message;
            unsigned int max = 0;
            int id_max = 0;
            for(Client& x: clients)
            {
                if(x.sum > max) 
                {
                    max = x.sum;
                    id_max = x.id;
                }
            }
            for(Client& x: clients)
            {
                message = "Max number = " + std::to_string(max) + " , your number = " + std::to_string(x.sum) + " , max id = " + std::to_string(id_max);
                nbytes = write(x.fd, message.c_str(), strlen(message.c_str()) + 1);
                close(x.fd);
            }
            std::cout << std::endl;
            clients.clear();
            num_set = 1;
        }
        else flag = -1;
        int ret = poll(act_set, num_set, -1);
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
                            clients.emplace_back(new_sock);
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
                        nbytes = read(act_set[i].fd, buf, BUFLEN);
                        if(nbytes > 0)
                        {
                            /*cout << "server got message: ";
                            for(int k = 0; k < nbytes; ++k) cout << buf[k];
                            cout <<  " from fd = " <<  act_set[i].fd << endl;*/

                            string separate_word;
                            for(int k = 0; k < nbytes; ++k)
                            {
                                if(buf[k] == '\0')
                                {
                                    if(strstr(separate_word.c_str(), "id="))
                                    {
                                        int K = std::atoi(separate_word.c_str() + 3);
                                        clients[i - 1].id = K;
                                    }else
                                    {
                                        int n = std::atoi(separate_word.c_str());
                                        if(n == -1) clients[i - 1].end = true;
                                        else clients[i - 1].sum += n;
                                    }
                                    separate_word.clear();
                                }else
                                { 
                                    separate_word.push_back(buf[k]);
                                }
                            }
                        }else
                        {
                            close(act_set[i].fd);
                            act_set[i] = act_set[num_set - 1];
                            clients[i] = clients[clients.size() - 1];
                            clients.pop_back();
                            --i;
                            --num_set;
                        }

                    }  
                }
            }
        }
        for(Client& x: clients)
        {
            cout << "id = " << x.id << " sum = " << x.sum << " fd = " << x.fd << endl;
        }
        std::cout << std::endl;
    }

    
    return 0;
}
