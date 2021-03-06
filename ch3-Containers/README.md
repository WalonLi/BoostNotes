#Containers

----
###MultiIndex
[參考資料](http://blog.tinlans.org/2008/04/09/boostmultiindex/)<br/>
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
###Bimap
用法類似std::map，但他不止可以利用key找，也可以利用value找key
底層是用multi_index去實作，所以你也可以利用其method(例如modify_key, modify_data)做些你要做的事
````
int main()
{
  typedef boost::bimap<std::string, int> bimap;
  bimap animals;

  animals.insert({"cat", 4});
  animals.insert({"shark", 0});
  animals.insert({"spider", 8});

  std::cout << animals.left.count("cat") << '\n';
  std::cout << animals.right.count(8) << '\n';

  for (auto it = animals.begin(); it != animals.end(); ++it)
    std::cout << it->left << " has " << it->right << " legs\n";
}
````
Default的情況下，每個key/value是利用boost::bimaps::set_of
如果要像std::multimap(key值可以重複)，可以利用boost::bimaps::multiset_of
````
  // Using boost::bimaps::set_of explicit，這會等同於上面
  typedef boost::bimap<boost::bimaps::set_of<std::string>,
    boost::bimaps::set_of<int>> bimap;
  // like as std::multimap
  typedef boost::bimap<boost::bimaps::set_of<std::string>,
    boost::bimaps::multiset_of<int>> bimap;
````
boost::bimaps::unordered_set_of/boost::bimaps::unordered_multiset_of/boost::bimaps::list_of/boost::bimaps::vector_of
這些都和C++標準裡差不多用法
boost::bimaps::unconstrainted_set_of則是讓我們無法用該元素找資料
````
  // 這樣我們就不能利用value(第二個資料)來做search or count，變得很像標準的std::map
  typedef boost::bimap<std::string,
    boost::bimaps::unconstrained_set_of<int>> bimap;
  bimap animals;

  animals.insert({"cat", 4});
  animals.insert({"shark", 0});
  animals.insert({"spider", 8});

  auto it = animals.left.find("cat");
  //這裡繼承了multi_index的特性，可以利用iterator來修改key/value，這在std::map是做不到的
  animals.left.modify_key(it, boost::bimaps::_key = "dog");
  std::cout << it->first << '\n';
````

----
####Array

boost::array is similar to std::array, which was added to the standard library with C++11.
````
#include <boost/array.hpp>
#include <string>
#include <algorithm>
#include <iostream>

int main()
{
  typedef boost::array<std::string, 3> array;
  array a;

  a[0] = "cat";
  a.at(1) = "shark";
  *a.rbegin() = "spider";

  std::sort(a.begin(), a.end());

  for (const std::string &s : a)
    std::cout << s << '\n';

  std::cout << a.size() << '\n';
  std::cout << a.max_size() << '\n';
}
````

----
####Unordered

boost::unordered大部份相同於C++11的std::unordered，大部分的情況下甚至可以互換都沒問題
唯一的差別在於boost的不要求其中元素可以排序，因為他不會做排序的動作，如果資料不需要排序的化，推荐使用boost的~
但是該元素必須要有能計算hash值的function，一般string、int等等的都不用做，但是class/struct的話就要做了，請看下面
````
struct animal
{
  std::string name;
  int legs;
};

// == operator也要實作
bool operator==(const animal &lhs, const animal &rhs)
{
  return lhs.name == rhs.name && lhs.legs == rhs.legs;
}

std::size_t hash_value(const animal &a)
{
  std::size_t seed = 0;
  // 利用hash_combine產生hash值
  boost::hash_combine(seed, a.name);
  boost::hash_combine(seed, a.legs);
  return seed;
}

int main()
{
  typedef boost::unordered_set<animal> unordered_set;
  unordered_set animals;

  animals.insert({"cat", 4});
  animals.insert({"shark", 0});
  animals.insert({"spider", 8});
}
````

----
####CircularBuffer

環狀的buffer，恩，就是環狀的buffer XDDDD，如果超過size時，則會把最前面的data擠掉
我們也無法任意修改值，只能用push/pop來update資料.
````
  typedef boost::circular_buffer<int> circular_buffer;
  circular_buffer cb{3};

  std::cout << cb.capacity() << '\n'; // 看容量 ： 3
  std::cout << cb.size() << '\n';     // 看有幾個element ： 0

  cb.push_back(0);
  cb.push_back(1);
  cb.push_back(2);
  cb.push_back(3);                    // 這個會override掉原本的data 0

  std::cout << cb.size() << '\n';     // 3

  for (int i : cb)
    std::cout << i << '\n';           // 1 2 3

  std::cout << std::boolalpha << cb.is_linearized() << '\n'; // 看是不是線性，如果buffer override時，就會變成非線性
  circular_buffer::array_range ar1, ar2;                     // 這個循環buffer主要由begin array和end arrary所組成
  ar1 = cb.array_one();                                      // 這時候他的data有 1 2
  ar2 = cb.array_two();                                      // 這時候他的data有 3
  cout << ar1.second << ";" << ar2.second << '\n';           // first為data iterator，second為array size

  cb.linearize();                                            // 重新序列化
  ar1 = cb.array_one();                                      // 1 2 3       
  ar2 = cb.array_two();                                      // 沒東西
````

----
####Heap

1  Prioriy Queue - 大部分用法都跟std::priority_queue一樣，多了一個queue的element count比較而已
````
  priority_queue<int> pq;
  pq.push(2);
  pq.push(3);
  pq.push(1);

  for (int i : pq)
    std::cout << i << '\n';

  priority_queue<int> pq2;
  pq2.push(4);
  std::cout << std::boolalpha << (pq > pq2) << '\n';
````

2  binomial_heap - 提供一種queue，support merge/update等等
````
  binomial_heap<int> bh;
  bh.push(2);
  bh.push(3);
  bh.push(1);

  binomial_heap<int> bh2;
  bh2.push(4);
  bh.merge(bh2);
  
  auto handle = bh.push(9); // restore 9 element handle
  bh.update(handle, 4);     // use this handle to update element

  std::cout << bh.top() << '\n';

  for (auto it = bh.ordered_begin(); it != bh.ordered_end(); ++it) // 從high priority到low priority
    std::cout << *it << '\n';
  std::cout << std::boolalpha << bh2.empty() << '\n'; // merge完後會變空的!
````

----
####Intrusive

比一般std的container performance還要好，但是比較難用<br/>
他不會為每個element new出一個空間，而是會指向實體的object(應該是該obj的reference)，所以他在call push_back時
理所當然的也不會有任何exception，因為不用copy or allocate memory阿，這也是為什麼performance比較好的原因

限制也挺多的，一來不能隨便把你想要的std obj放進去，你必須時做你的class，並繼承一些東東，
就像下面那段code，如果想要存進intrusive的話，必須繼承boost::list_back_hook，
如果今天存的element是pointer的話，就要格外小心，不只要移除list item，更要把該element free掉<br/>
ps: intrusive也有support set，但大致用法差不多，就不介紹
````
using namespace boost::intrusive;

struct animal : public list_base_hook<>
{
  std::string name;
  int legs;
  animal(std::string n, int l) : name{std::move(n)}, legs{l} {}
};

int main()
{
  animal a1{"cat", 4};
  animal a2{"shark", 0};
  animal a3{"spider", 8};

  typedef list<animal> animal_list; //it's a boost intrusive list, behavior like as std::list
  animal_list animals;
  animals.push_back(a1);
  animals.push_back(a2);
  animals.push_back(a3);
  a1.name = "dog";

  // be careful if you store a pointer
  animal *a4 = new animal{"duck", 10};
  animals.push_back(*a4);
  // If you want to remove pointer element, provide three way to use.
  // 1.
  animals.pop_back();
  delete a4;
  // 2.
  animals.pop_back_and_dispose([](animal *a){ delete a; });
  // 3. 這個更進階，利用boost的link mode來決定要不要移除element
  //    具體而言怎麼做就不講了，可以直接去看boost::intrusive::constant_time_size和link mode
  struct animal : public list_base_hook<link_mode<auto_unlink>> // support link mode
  {
    std::string name;
    int legs;
    animal(std::string n, int l) : name{std::move(n)}, legs{l} {}
  };
  typedef constant_time_size<false> constant_time_size;
  typedef list<animal, constant_time_size> animal_list;
  animal_list animals;
  animals.push_back(*a3) ;
  delete a3;                //會自動從list移除掉a3，超屌
  
  for (const animal &a : animals)
    std::cout << a.name << '\n';    // print dog, shark, spider
}
````

----
####MultiArray

多維的Array，提供一些看似簡便的方法，但是有夠難用
````
  // like as char a[2][6]
  boost::multi_array<char, 2> a{boost::extents[2][6]};

  typedef boost::multi_array<char, 2>::array_view<1>::type array_view;
  typedef boost::multi_array_types::index_range range;
  array_view view = a[boost::indices[0][range{0, 5}]];

  std::memcpy(view.origin(), "tsooB", 6);
  std::reverse(view.begin(), view.end());

  std::cout << view.origin() << '\n';

  boost::multi_array<char, 2>::reference subarray = a[1];
  std::memcpy(subarray.origin(), "C++", 4);

  std::cout << subarray.origin() << '\n';
````

----
####Container

跟std container差不多，多了boost::container::slist和boost::container::stable_vector





