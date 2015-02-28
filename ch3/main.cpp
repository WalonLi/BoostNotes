/**
  Author: Walon Li
 **/

#include <iostream>
#include <set>
#include <string>
#include <locale>
#include <boost/algorithm/string.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>

using namespace std;

using namespace boost::multi_index;

// define a type
struct animal
{
  std::string name;
  int legs;
};

// define
typedef multi_index_container<
  animal,   // your structure
  indexed_by<
    hashed_non_unique<
      member<
        animal, std::string, &animal::name
      >
    >,
    hashed_non_unique<
      member<
        animal, int, &animal::legs
      >
    >
  >
> animal_multi;

void multiindex_func()
{
    animal_multi animals;

    animals.insert({"cat", 4});
    animals.insert({"shark", 0});
    animals.insert({"spider", 8});

    cout << animals.count("cat") << '\n';

    //&legs_index = animals.get<1>();
    //cout << legs_index.count(8) << '\n';

    auto &legs_index = animals.get<1>();
    auto it = legs_index.find(4);
    legs_index.modify(it, [](animal &a){ a.name = "dog"; });

    std::cout << animals.count("dog") << '\n';
}

int main()
{

    multiindex_func() ;
    cout << endl ;

    return 0;
}


