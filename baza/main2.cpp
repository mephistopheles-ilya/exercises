#include "man_classes.h" 

int main(void)
{
    Baza baza("file_baza");
    baza.print_n();
    vector<list<Man>::iterator> v1;
    cout << baza.find_money(v1, 1000, 5000) << endl;
    cout << baza.remove_n(v1) << endl;
    baza.print_n();
    return 0;
}




