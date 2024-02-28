#include "classes.h" 


double inf  = std::numeric_limits<double>::infinity();

int main(void)
{
    Baza baza;
    baza.read_currency("file_currency");
    baza.read_records("file_baza");

    vector<list<Operations>::iterator> v1;
    vector<list<Man>::iterator> v2;

    cout << baza.find_o(v1, "-", inf, inf, "-", "2024/02/27/02/15/27", "2024/02/27/02/15/27") << endl;
    for(size_t i = 0; i < v1.size(); ++i) cout << (*v1[i]) << endl << endl;
    v1.clear();

    baza.create_operation("26426223769674995521", 900, "NOK", false, "2024/02/27/02/15/27", "78090564995732893703");  

    cout << baza.find_o(v1, "-", inf, inf, "-", "2024/02/27/02/15/27", "2024/02/27/02/15/27") << endl;
    for(size_t i = 0; i < v1.size(); ++i) cout << (*v1[i]) << endl << endl;
    v1.clear();

    cout << baza.find_n(v2, "Авдей", "Авдеев", "Авдеевич", inf, inf, "-", "-") << endl;
    for(size_t i = 0; i < v2.size(); ++i) cout << (*v2[i]) << endl;
    cout << baza.remove_n(v2) << endl << endl;

    cout << baza.find_n(v2, "Авдей", "Авдеев", "Авдеевич", inf, inf, "-", "-") << endl;




    return 0;
}




