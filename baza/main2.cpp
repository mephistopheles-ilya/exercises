#include "classes.h" 

int main(void)
{
    Baza baza("file_baza");
    vector<list<Man>::iterator> v1;

    cout << baza.find_n(v1, "Авдей", "Авдеев", "Авдеевич", 1000, 10'000, "RUB", "-") << endl;
    for(int i = 0; i < v1.size(); ++i) cout << (*v1[i]) << endl;
    v1.clear();
    //cout << baza.remove_n(v1) << endl;
    //cout << baza.find_n(v1, "Авдей", "Авдеев", "Авдеевич", 1000, 10'000, "RUB", "-") << endl;

    cout << baza.find_n(v1, "Авдей", "-", "-", std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), "-", "-") << endl;
    v1.clear();
    //cout << baza.remove_n(v1) << endl;
    //cout << baza.find_n(v1, "Авдей", "-", "-", std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), "-", "-") << endl;

    cout << baza.find_n(v1, "-", "Авдеев", "-", std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), "-", "-") << endl;
    v1.clear();
    //cout << baza.remove_n(v1) << endl;
    //cout << baza.find_n(v1, "-", "Авдеев", "-", std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), "-", "-") << endl;

    cout << baza.find_n(v1, "-", "-", "Авдеевич", std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), "-", "-") << endl;
    v1.clear();
    //cout << baza.remove_n(v1) << endl;
    //cout << baza.find_n(v1, "-", "-", "Авдеевич", std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), "-", "-") << endl;

    cout << baza.find_n(v1, "-", "-", "-", 1000, 10'000, "-", "-") << endl;
    v1.clear();
    //cout << baza.remove_n(v1) << endl;
    //cout << baza.find_n(v1, "-", "-", "-", 1000, 10'000, "-", "-") << endl;

    cout << baza.find_n(v1, "-", "-", "-", std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), "RUB", "-") << endl;
    v1.clear();
    //cout << baza.remove_n(v1) << endl;
    //cout << baza.find_n(v1, "-", "-", "-", std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), "RUB", "-") << endl;



    vector<list<Operations>::iterator> v2;
    cout << baza.find_o(v2, "-", 1000, 10'000, "RUB", "2022/00/00/00/00/00", "2022/12/31/24/60/60") << endl;
    v2.clear();
    //cout << baza.remove_o(v2) << endl;
    //cout << baza.find_o(v2, "-", 1000, 10'000, "RUB", "2022/00/00/00/00/00", "2022/12/31/24/60/60") << endl;

    cout << baza.find_o(v2, "-", 1000, 10'000, "-", "-", "-") << endl;
    v2.clear();
    //cout << baza.remove_o(v2) << endl;
    //cout << baza.find_o(v2, "-", 1000, 10'000, "RUB", "2022/00/00/00/00/00", "2022/12/31/24/60/60") << endl;

    cout << baza.find_o(v2, "-", std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), "RUB", "-", "-") << endl;
    v2.clear();
    //cout << baza.remove_o(v2) << endl;
    //cout << baza.find_o(v2, "-", 1000, 10'000, "RUB", "2022/00/00/00/00/00", "2022/12/31/24/60/60") << endl;

    cout << baza.find_o(v2, "-", std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), "-", "2022/00/00/00/00/00", "2022/12/31/24/60/60") << endl;
    v2.clear();
    //cout << baza.remove_o(v2) << endl;
    //cout << baza.find_o(v2, "-", 1000, 10'000, "RUB", "2022/00/00/00/00/00", "2022/12/31/24/60/60") << endl;



    return 0;
}




