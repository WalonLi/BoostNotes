#include <iostream>
#include <string>
#include <boost/algorithm/string.hpp>

int main()
{

    std::string str = "     Hello World" ;
    boost::algorithm::trim(str) ;
    
    std::cout << str << "\n";
    return 0 ;
}
