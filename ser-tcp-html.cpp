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
#include <sstream>
using namespace std;


#define PORT    5555
#define BUFLEN  4096

int   readFromClientHTTP(int fd, char *buf);
void  writeToClientHTTP(int fd, char *buf);


int  main(void)
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
                        err = readFromClientHTTP(act_set[i].fd, buf);
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
                            writeToClientHTTP(act_set[i].fd,buf);
                        }
                    }  
                }
            }
        }
    }
    
    return 0;
}


int  readFromClientHTTP(int fd, char *buf)
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



#include <time.h>

void ProcessHTML(stringstream &html, string & request)
{
    html << "<!DOCTYPE html>\n";
    html << "<html>\n";
    html << "<head>\n";
    html << "<meta charset = \"UTF-8\">\n";
    html << "<title>Bank</title>\n";
    html << "</head>\n";
    html << "<body>\n";
    html << "<form name=\"myform\" action=\"http://localhost:5555\"  method=\"get\">\n";
    html << "<p><b>Your Request:</b><br>\n";
    html << "<input placeholder=\"Enter smt\" type=\"text\" size=\"40\" name=\"request\"> <p>\n";
    html << "<input type=\"submit\" Value=\"Send\">\n";
    html << "</form>\n";
    html << "</body>\n";
    html << "</html>\n";




}


void  writeToClientHTTP(int fd, char *buf)
{
    int  nbytes;
    stringstream head; 
    stringstream html;
    string request = "";

    if (strstr(buf, "GET")) {
        char *p1 = strchr(buf, '/');
        char *p2 = strchr(p1, ' ');
        *p2 = 0;
        request = p1;
        printf("get request: %s\n", request.c_str());
        *p2 = ' ';
    }
    if (strstr(buf, "POST")) {
        char *p1 = strstr(buf, "\r\n\r\n");
        request = (p1 + 4);
        printf("post request: %s\n", request.c_str());
    }

    if (request != "/" && request.find("request") == string::npos)  {
        head << "HTTP/1.1 404 Not found\r\n";
        head << "Connection: close\r\n";
        head << "Content-length: 0\r\n";
        head << "\r\n";
        nbytes = write(fd, head.str().c_str(), head.str().length() + 1);
        return;
    }

    ProcessHTML(html, request);
    int html_length = html.str().length();



    head << "HTTP/1.1 200 OK\r\n";
    head << "Connection: keep-alive\r\n";
    head << "Content-type: text/html\r\n";
    head << "Content-length: " << html_length << "\r\n";
    head << "\r\n";

    nbytes = write(fd, head.str().c_str(), head.str().length() + 1);
    printf("http nb = %d\n", nbytes);

    nbytes = write(fd, html.str().c_str(), html.str().length() + 1);
    printf("html nb = %d\n", nbytes);

    if (nbytes<0) {
        perror("Server: write failure");
    }
}




