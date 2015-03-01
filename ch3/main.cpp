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
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/bimap.hpp>
#include <boost/bimap/unconstrained_set_of.hpp>
#include <boost/bimap/support/lambda.hpp>

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
    hashed_non_unique< // first iterator by string hash index
      member<
        animal, std::string, &animal::name
      >
    >,
    hashed_non_unique< // second iteratpr by leg hash index.
      member<
        animal, int, &animal::legs
      >
    >
  >
> animal_multi;

typedef multi_index_container<
  animal,
  indexed_by<
    sequenced<>,
    ordered_non_unique<
      member<
        animal, int, &animal::legs
      >
    >,
    random_access<>
  >
> animal_multi_2;

void multiindex_func()
{
    /*
    animal_multi animals;

    animals.insert({"cat", 4});
    animals.insert({"shark", 0});
    animals.insert({"spider", 8});

    cout << animals.count("cat") << '\n';

    //const animal_multi::nth_index<1>::type &legs_index = animals.get<1>();
    //cout << legs_index.count(8) << '\n';

    auto &legs_index = animals.get<1>();
    auto it = legs_index.find(4);
    legs_index.modify(it, [](animal &a){ a.name = "dog"; });

    std::cout << animals.count("dog") << '\n';
    */
    animal_multi_2 animals_2;

    animals_2.push_back({"cat", 4});
    animals_2.push_back({"shark", 0});
    animals_2.push_back({"spider", 8});

    auto &legs_index = animals_2.get<1>();
    auto it = legs_index.lower_bound(4);
    auto end = legs_index.upper_bound(8);
    for (; it != end; ++it)
      std::cout << it->name << '\n';

    const auto &rand_index = animals_2.get<2>();
    std::cout << rand_index[0].name << '\n';

    cout << legs_index.begin()->name ;
}

void bimap_func()
{
    typedef boost::bimap<std::string,
      boost::bimaps::unconstrained_set_of<int>> bimap;
    bimap animals;

    animals.insert({"cat", 4});
    animals.insert({"shark", 0});
    animals.insert({"spider", 8});

    auto it = animals.left.find("cat");
    animals.left.modify_key(it, boost::bimaps::_key = "dog");

    std::cout << it->first << '\n';
}

int main()
{

    // multiindex_func() ;
    bimap_func() ;
    cout << endl ;

    return 0;
}


