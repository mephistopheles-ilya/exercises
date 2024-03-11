#include "classes.h"

int main(void)
{
    Baza baza;
    baza.read_currency("file_currency");
    baza.read_records("file_baza");

    string str;
    while(true)
    {
        getline(cin, str);
        cout << baza.work_with_str(str) << endl;
    }



    return 0;    
}

