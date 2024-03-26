#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "classes.h"

#define  SERVER_PORT     5555
#define  SERVER_NAME    "127.0.0.1"
#define  BUFLEN          512

int  writeToServer(int fd, std::fstream& in);
int  readFromServer(int fd, std::fstream& out);

int  main(int argc, char** argv)
{
    if(argc != 3)
    {
        cout << "Wrong amount of raguments" << endl;
        return -1;
    }

    std::fstream in, out;
    in.open(argv[1], std::ios::in);
    out.open(argv[2], std::ios::out);
    if(!in.is_open() || !out.is_open())
    {
        cout << "Cannot open file " << argv[1] << " " << argv[2] << endl;
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

    while(true) 
    {
        if(writeToServer(sock, in) < 0) break;
        if(readFromServer(sock, out) < 0) break;
    }
    cout << "The end" << endl << endl;

    close (sock);
    in.close();
    out.close();
    return 0;
}



int  writeToServer (int fd, std::fstream& in)
{
    int   nbytes;
    char  buf[BUFLEN];
    string str_to_read;

    if(!std::getline(in, str_to_read))
    {
        return -1;
    }

    unsigned int sl = (rand()%500 + 10) * 1000;
    usleep(sl);

    strcpy(buf, &str_to_read[0]);

    nbytes = write(fd, buf, strlen(buf) + 1);
    if(nbytes < 0)
    {
        cout << "Problems with writing" << endl;
        return -1;
    }
    if(strstr(buf,"stop")) return -1;
    return 0;
}


int  readFromServer (int fd, std::fstream& out)
{
    int   nbytes, i = 0;
    char buf[BUFLEN]; // buf to read from server
    char str_to_cmp[BUFLEN]; // buf to find end word END
    memset(buf, 0, BUFLEN); // clear buf
    memset(str_to_cmp, 0, BUFLEN); // clear buff
    int full_part = 1; // is str_to_cmp contain part from previous buf
    int stop = 0; // end word was found in str_to_cmp
    int length_to_remember = 0; // how many char str_to_cmp contain from previous buf

    nbytes = read(fd, buf, BUFLEN);
    cout << buf << endl; 

    while(!stop)
    {
        if(nbytes < 0)
        {
            out << "Error with reading" << endl;
            cout << "Error with reading" << endl;
            return -1;
        } else if (nbytes == 0)
        {
            out << "Client: no message" << endl;
            cout << "Error with reading" << endl;
            return 0;
        } else 
        {
            int first = 0, second = 0; // iterator to select parts in buf
            for(int j = 0; j < nbytes; ++j)
            {
                if(buf[j] == '\0')
                {
                    first = second; // move iterators
                    second = j + 1;

                    if(full_part == 1) // if there is no extra previous part
                    {
                        strcpy(str_to_cmp, buf + first);
                    }else
                    {
                        strcpy(str_to_cmp + length_to_remember, buf + first);
                        full_part = 1;
                    }

                    if(strcmp(str_to_cmp, "END") == 0) 
                    {
                        stop = 1;
                    }

                    out << endl;
                    ++i;
                }else
                {
                    out << buf[j];
                }
            }

            if(buf[nbytes - 1] != '\0')
            {
                full_part = 0;
                length_to_remember = nbytes - second;
                for(int i = second, j = 0; i < nbytes; ++i, ++j)
                {
                    str_to_cmp[j] = buf[i];
                }
            }
            memset(buf, 0, BUFLEN);
            if(stop == 1) break;
            nbytes = read(fd, buf, BUFLEN);
        }
    }

    out << "Received " << i - 2 << " objects" << endl;
    cout << "Received " << i - 2 << " objects" << endl << endl;

    return 0;
}

