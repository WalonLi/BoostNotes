#Containers

----
###MultiIndex
提供一種Container，裏面可以塞不同的形別
例如想要把std::vector和std::set塞再同一個container做管理，就可以用這個
````
// 定義structure and container
struct animal
{
  std::string name;
  int legs;
};

typedef boost::multi_index::multi_index_container<
  animal,                                   // type of elements the container should store.
  boost::multi_index::indexed_by<           // denote different indexes the container should provide.
    boost::multi_index::hashed_non_unique<  // interface 1, 
                                            // its behavior like std::unorderer_set, look up value using a hash value.
      boost::multi_index::member<           // to access member variable.
        animal, std::string, &animal::name
      >
    >,
    boost::multi_index::hashed_non_unique<  // interface 2
      boost::multi_index::member<
        animal, int, &animal::legs
      >
    >
  >
> animal_multi;

int main()
{
    animal_multi animals;

    // inset data
    animals.insert({"cat", 4});
    animals.insert({"shark", 0});
    animals.insert({"spider", 8});

    // default use index "name" to search
    cout << animals.count("cat") << '\n';

    // 框框裏面<1>的數字代表第幾個parameter，以上面的例子，0就是name，1就是leg
    const animal_multi::nth_index<1>::type &legs_index = animals.get<1>();
    cout << legs_index.count(8) << '\n';
}
````
存在裏面的data都是const，如過要增加/修改值
請利用modify()這個member function
````
    auto &legs_index = animals.get<1>();
    auto it = legs_index.find(4);
    legs_index.modify(it, [](animal &a){ a.name = "dog"; });
    std::cout << animals.count("dog") << '\n';
````




