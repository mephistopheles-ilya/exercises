#include "classes.h"

int main(void)
{
    Baza baza;
    baza.read_currency("file_currency");
    baza.read_records("file_baza");

    vector<list<Man>::iterator> v1;
    vector<list<Operations>::iterator> v2;
    string message;

    string str;
    while(true)
    {
        getline(cin, str);

        cout << baza.work_with_str(message, v1, v2, str) << endl;
        for(size_t i = 0; i < v1.size(); ++i)
        {
            cout << (*v1[i]) << endl;
        }
        for(size_t i = 0; i < v2.size(); ++i)
        {
            cout << (*v2[i]) << endl;
        }
        
        v1.clear();
        v2.clear();
    }



    return 0;    
}

