/**
  Author: Walon Li
 **/

#include <iostream>
#include <set>
#include <string>
#include <locale>
#include <boost/algorithm/string.hpp>

using namespace std;

void string_algorithms()
{
    using namespace boost::algorithm;
    //std::string s = "Boost C++ k\xfct\xfcphaneleri";
    //std::string upper_case1 = to_upper_copy(s);
    // I am not install Turkish language
    //std::string upper_case2 = to_upper_copy(s, std::locale{"tr_TR"});
    //std::locale::global(std::locale{"Turkish"});
    // std::cout << upper_case1 << '\n';
    //std::cout << upper_case2 << '\n';
    /*
    std::string s = "Boost C++ Libraries";
    std::cout << erase_first_copy(s, "s") << '\n';
    std::cout << erase_nth_copy(s, "s", 0) << '\n';
    std::cout << erase_last_copy(s, "s") << '\n';
    std::cout << erase_all_copy(s, "s") << '\n';
    std::cout << erase_head_copy(s, 5) << '\n';
    std::cout << erase_tail_copy(s, 9) << '\n';
    */
    std::string s = "Boost C++ Libraries";
    std::cout.setf(std::ios::boolalpha);
    std::cout << starts_with(s, "Boost") << '\n';
    std::cout << ends_with(s, "Libraries") << '\n';
    std::cout << contains(s, "C++") << '\n';
    std::cout << lexicographical_compare(s, "Boost") << '\n';
}

int main()
{
    string_algorithms() ;
    cout << endl ;

    return 0;
}


