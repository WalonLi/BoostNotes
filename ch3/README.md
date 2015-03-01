#Containers

----
###MultiIndex
[參考資料](http://blog.tinlans.org/2008/04/09/boostmultiindex/)
提供一種Container，裏面可以塞不同的形別，讓該container可以吐出不同型別的iterator.
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
                                            // its behavior like std::unorderer_set, look up value using string hash value.

      boost::multi_index::member<           // to access member variable.
        animal, std::string, &animal::name
      >
    >,
    boost::multi_index::hashed_non_unique<  // interface 2, look up value by using leg value.
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

    // find 會返回result的iterator，
    auto it = legs_index.find(4);
    legs_index.modify(it, [](animal &a){ a.name = "dog"; });
    std::cout << animals.count("dog") << '\n';
````
boost::multi_index::hashed_unique代表hash值只能有一個，
所以如果出現下列的code，count只會＝1(cat)
````
typedef multi_index_container<
  animal,
  indexed_by<
    hashed_non_unique<
      member<
        animal, std::string, &animal::name
      >
    >,
    hashed_unique<
      member<
        animal, int, &animal::legs
      >
    >
  >
> animal_multi;

main()
{
    animals.insert({"cat", 4});
    animals.insert({"dog", 4});
}
````
剩下的三種interface
boost::multi_index::sequenced           // Like as list, elements are stored in the given order.
boost::multi_index::ordered_non_unique  // 根據標準自動做sort，有lower_bound()和upper_bound()可以用.
boost::multi_index::random_access       // 隨機.
````

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
> animal_multi;

int main()
{
  animal_multi animals;

  animals.push_back({"cat", 4});
  animals.push_back({"shark", 0});
  animals.push_back({"spider", 8});

  auto &legs_index = animals.get<1>();
  auto it = legs_index.lower_bound(4);
  auto end = legs_index.upper_bound(8);
  for (; it != end; ++it)
    std::cout << it->name << '\n';

  const auto &rand_index = animals.get<2>();
  std::cout << rand_index[0].name << '\n';
}
````
Key extractor，就是幫助取值的東西啦~
boost::multi_index::identify    // ::ordered_unique根據Programer提供的 < operator去比較大小
boost::multi_index::mem_fun     // 根據Programer提供的function去取值 
````
class animal
{
public:
  animal(std::string name, int legs) : name_{std::move(name)},
    legs_(legs) {}
  bool operator<(const animal &a) const { return legs_ < a.legs_; }
  const std::string &name() const { return name_; }
private:
  std::string name_;
  int legs_;
};

typedef multi_index_container<
  animal,
  indexed_by<
    ordered_unique<
      identity<animal>
    >,
    hashed_unique<
      const_mem_fun<
        animal, const std::string&, &animal::name
      >
    >
  >
> animal_multi;
````

----






