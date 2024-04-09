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

#include <sstream>
#include "classes.h"
using namespace std;


#define PORT    5555
#define BUFLEN  4096

int   readFromClientHTTP(int fd, char *buf);
void  writeToClientHTTP(int fd, char *buf, Baza& baza);
string convert_str(const string& str);


int  main(void)
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
                            writeToClientHTTP(act_set[i].fd,buf, baza);
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

void ProcessHTML(stringstream &html, string& name, string& surname, string& surname2, string& money, string& currency, string& bank_account, size_t amount)
{
    html << "<!DOCTYPE html>\n";
    html << "<html>\n";
    html << "<head>\n";
    html << "<meta charset = \"UTF-8\">\n";
    html << "<title>Bank</title>\n";

    /*html << "<style>\n";
    html << "body {\n";
    html << "font-family: Arial, sans-serif;\n";
    html << "display: flex;\n";
    html << "align-items: center;\n";
    html << "justify-content: center;\n";
    html << "height: 100vh;\n";
    html << "background-color: #f5f5f5;\n";
    html << "}\n";
    html << "</style>\n";*/

    html << "</head>\n";
    html << "<body>\n";
    html << "<form name=\"myform\" action=\"http://localhost:5555\"  method=\"get\">\n";
    html << "<p><b>Your Request:</b><br>\n";

    html << "<input ";
    if(name.size() != 0 && name != "-"s) html << "value =" << "\"" << name << "\""  << ' ';
    else html << "placeholder=\"name\" ";
    html << "type=\"text\" size=\"40\" name=\"request_name\"> <p>\n";

    html << "<input ";
    if(surname.size() != 0 && surname != "-"s) html << "value = " << "\""  << surname << "\"" <<  ' ';
    else html << "placeholder=\"surname\" ";
    html << "type=\"text\" size=\"40\" name=\"request_surname\"> <p>\n";

    html << "<input ";
    if(surname2.size() != 0 && surname2 != "-"s) html << "value = " << "\"" << surname2 << "\"" <<   ' ';
    else html << "placeholder=\"surname2\" ";
    html << "type=\"text\" size=\"40\" name=\"request_surname2\"> <p>\n";

    html << "<input ";
    if(money.size() != 0) html << "value = " << "\"" <<  money << "\"" <<  ' ';
    else html << "placeholder=\"money\" ";
    html << "type=\"text\" size=\"40\" name=\"request_money\"> <p>\n";

    html << "<input ";
    if(currency.size() != 0 && currency != "-"s) html << "value = " << "\"" <<  currency << "\"" <<  ' ';
    else html << "placeholder=\"currency\" ";
    html << "type=\"text\" size=\"40\" name=\"request_currency\"> <p>\n";

    html << "<input ";
    if(bank_account.size() != 0 && bank_account != "-"s) html << "value = " << "\"" <<  bank_account << "\"" <<  ' ';
    else html << "placeholder=\"bank-account\" ";
    html << "type=\"text\" size=\"40\" name=\"request_bank_account\"> <p>\n";

    html << "<input type=\"submit\" Value=\"Send\">\n";
    html << "</form>\n";
    if(amount > 0)
    {
        html << "<p><b>Found " << amount << " objects </b></p>\n";
        html << "<ol>\n";
    }else
    {
        html << "<p><b>Not found</b></p>\n";
        html << "</body>\n";
        html << "</html>\n";
    }




}


void  writeToClientHTTP(int fd, char *buf, Baza& baza)
{
    int  nbytes;
    stringstream head; 
    stringstream html;
    string name = "", surname = "", surname2 = "", money = "", currency = "", account_number = "";
    string request = "";

    if (strstr(buf, "GET"))
    {
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

    if (request != "/" && request.find("request") == string::npos)
    {
        head << "HTTP/1.1 404 Not found\r\n";
        head << "Connection: close\r\n";
        head << "Content-length: 0\r\n";
        head << "\r\n";
        nbytes = write(fd, head.str().c_str(), head.str().length() + 1);
        return;
    }

    string name_0 = "", surname_0 = "", surname2_0 = "";
    size_t pos = 0;
    if((pos = request.find("request_name=")))
    {
        for(size_t i = pos + ("request_name="s).size(); request[i] != '&' && i < request.size(); ++i)
        {
            name_0.push_back(request[i]);
        }
        name = move(convert_str(name_0));
    }
    if((pos = request.find("request_surname=")))
    {
        for(size_t i = pos + ("request_surname="s).size(); request[i] != '&' && i < request.size(); ++i)
        {
            surname_0.push_back(request[i]);
        }
        surname = move(convert_str(surname_0));
    }
    if((pos = request.find("request_surname2=")))
    {
        for(size_t i = pos + ("request_surname2="s).size(); request[i] != '&' && i < request.size(); ++i)
        {
            surname2_0.push_back(request[i]);
        }
        surname2 = move(convert_str(surname2_0));
    }
    if((pos = request.find("request_money=")))
    {
        for(size_t i = pos + ("request_money="s).size(); request[i] != '&' && i < request.size(); ++i)
        {
            money.push_back(request[i]);
        }
        size_t pos = money.find('+');
        if(pos != string::npos) money[pos] = ' ';
    }
    if((pos = request.find("request_currency=")))
    {
        for(size_t i = pos + ("request_currency="s).size(); request[i] != '&' && i < request.size(); ++i)
        {
            currency.push_back(request[i]);
        }
    }
    if((pos = request.find("request_bank_account=")))
    {
        for(size_t i = pos + ("request_bank_account="s).size(); request[i] != '&' && i < request.size(); ++i)
        {
            account_number.push_back(request[i]);
        }
    }

    cout << "Name: " << name << endl;
    cout << "Surname: " << surname << endl;
    cout << "Surname2: " << surname2 << endl;
    cout << "Money: " << money << endl;
    cout << "Currency: " << currency << endl;
    cout << "Account_number: " << account_number << endl;

    if(name.size() == 0) name = "-";
    if(surname.size() == 0) surname = "-";
    if(surname2.size() == 0) surname2 = "-";
    if(currency.size() == 0) currency = "-";
    if(account_number.size() == 0) account_number = "-";

    vector<list<Man>::iterator> v;
    string m1, m2;
    double lower_money, upper_money;
    pos = money.find(' ');
    if(pos != string::npos)
    {
        for(size_t i = 0; i < pos; ++i) m1.push_back(money[i]);
        for(size_t i = pos + 1; i < money.size(); ++i) m2.push_back(money[i]);
        lower_money = stod(m1);
        upper_money = stod(m2);
    }else
    {
        lower_money = upper_money = std::numeric_limits<double>::infinity();
    }
    baza.find_n(v, name, surname, surname2, lower_money, upper_money, currency, account_number);


    if(v.size() > 0)
    {
        ProcessHTML(html, name, surname, surname2, money, currency, account_number, v.size());
        string end = "</ol>\n</body></html>";
        size_t extra_length = 0;
        for(size_t i = 0; i < v.size(); ++i)
        {
            extra_length += (v[i] -> create_string()).size() + ("<li></li>\n"s).size();
        }
        head << "HTTP/1.1 200 OK\r\n";
        head << "Connection: keep-alive\r\n";
        head << "Content-type: text/html\r\n";
        head << "Content-length: " << html.str().length() + extra_length + end.size() << "\r\n";
        head << "\r\n";

        nbytes = write(fd, head.str().c_str(), head.str().length());

        nbytes = write(fd, html.str().c_str(), html.str().length());

        for(size_t i = 0; i < v.size(); ++i)
        {
            string s = "<li>" +  (v[i] -> create_string()) + "</li>\n";
            nbytes = write(fd, s.c_str(), s.size());
        }
        nbytes = write(fd, end.c_str(), end.size());
    }else
    {
        ProcessHTML(html, name, surname, surname2, money, currency, account_number, false);
        head << "HTTP/1.1 200 OK\r\n";
        head << "Connection: keep-alive\r\n";
        head << "Content-type: text/html\r\n";
        head << "Content-length: " << html.str().length() << "\r\n";
        head << "\r\n";

        nbytes = write(fd, head.str().c_str(), head.str().length());

        nbytes = write(fd, html.str().c_str(), html.str().length());
    }
    if(nbytes < 0) cout << "writing to client error" << endl; 
}


string convert_str(const string& str)
{
    string str_to_convertir(2, ' ');
    string res;
    size_t pos = 0;
    pos = str.find('%', pos);
    while(pos != str.npos)
    {
        str_to_convertir[0] = str[pos + 1];
        str_to_convertir[1] = str[pos + 2];
        pos = str.find('%', ++pos); 
        res.push_back(static_cast<char>(stoi(str_to_convertir, nullptr, 16)));
    }
    if(res.size() == 0) return str;
    return res;
}


