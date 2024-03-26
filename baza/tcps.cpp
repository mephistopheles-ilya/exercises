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

#include "classes.h"

#define PORT    5555
#define BUFLEN  512

int   readFromClient(int fd, char *buf);
void  writeToClient (int fd, char *buf, Baza& baza);

int  main (void)
{
    Baza baza;
    baza.read_currency("file_currency");
    baza.read_records("file_baza");
    cout << endl;

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
        int ret = poll(act_set, num_set, -1);
        if(ret < 0)
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
                            writeToClient(act_set[i].fd,buf, baza);
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



void  writeToClient (int fd, char *buf, Baza& baza)
{
    int  nbytes;
    vector<list<Man>::iterator> v1;
    vector<list<Operations>::iterator> v2;
    string message;

    baza.work_with_str(message, v1, v2, string(buf));
    cout << "Start writing to client" << endl;
    if(v1.size() > 0 || v2.size() > 0) 
    {
        string s;
        s += "found " + std::to_string(v1.size() + v2.size()) + " objects";
        cout << s << endl;
        nbytes = write(fd,s.c_str(),strlen(s.c_str())+1);
        if(nbytes < 0)
        {
            cout << "Server: write failure" << endl;
        }

    }else if(message.size() > 0)
    {
        nbytes = write(fd,message.c_str(),strlen(message.c_str())+1);
        if(nbytes < 0)
        {
            cout << "Server: write failure" << endl;
        }
    }else
    {
        string s = "Not found or deleted";
        nbytes = write(fd,s.c_str(),strlen(s.c_str())+1);
        if(nbytes < 0)
        {
            cout << "Server: write failure" << endl;
        }
    }

    for(size_t i = 0; i < v1.size(); ++i)
    {
        string s = std::to_string(i) + " " +  (v1[i] -> create_string()) ;
        nbytes = write(fd,s.c_str(),strlen(s.c_str()) + 1);
        if(nbytes < 0)
        {
            cout << "Server: write failure" << endl;
        }
    }
    for(size_t i = 0; i < v2.size(); ++i)
    {
        string s = v2[i] -> create_string();
        nbytes = write(fd,s.c_str(),strlen(s.c_str())+1);
        if(nbytes < 0)
        {
            cout << "Server: write failure" << endl;
        }
    }
    string s = "END";
    nbytes = write(fd, s.c_str(), strlen(s.c_str()) + 1);
    if(nbytes < 0) 
    {
        cout << "Server: write failure" << endl;
    }
    cout << "Stop writing to client" << endl << endl;
}
