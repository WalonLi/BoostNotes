#RTII and Memory Management

----
###Smart Pointer

1. boost::scoped_ptr/boost::scoped_array<br/>
不能copy，且只會存活在這個scope，當離開這個scope時會自動free

2. boost::shared_ptr/boost::shared_array<br/>
ownership可轉交，會紀錄現在有幾個人指向他，當沒人指向他時才free<br/>
指像他的人都是用reference，並不是複製！<br/>

3. boost::make_shared<br/>
ex: auto p = boost::make_shared<int>(1) ;<br/>

4. boost::weak_ptr
避免smart pointer循環reference，導致無法自動free，memory leak<br/>
這時如果是用weak_ptr，則不會計算refer count，在生命週期結束時會自動free掉<br/>
可以利用weak_ptr::lock()，這會檢查該refer的ptr有無被free掉，<br/>
引用原文<br/>
invoking lock() returns a shared pointer that points to a valid object if one exists at the time of the call. If not, the shared pointer is set to 0 and is equivalent to a null pointer.<br/>
參考:<br/>
http://www.cnblogs.com/tianfang/archive/2008/09/20/1294590.html

5. boost::intrusive_ptr<br/>
跟shared_ptr很像，解釋有點複雜，直接看<br/>
http://www.cnblogs.com/edwardlost/archive/2011/02/17/1957019.html

----
###Pointer Container

1. boost::ptr_vector<br/>
只允許container內的元素為pointer，v.back()會返回該元素的值，算是挺方便的設計<br/>

2. boost::ptr_set/boost::ptr_list<br/>
boost::ptr_set會自動比較refence value，而去做排序(一般std不會)，這也是滿方便的一點<br/>
其他像list/map/deque...etc的做法也差不多(不知道會不會排序)<br/>

3. boost::ptr_inserter<br/>
就一些container的插入boost::ptr_container::back_inserter()/front_inserter()都在這邊<br/>
ex: <br/>
boost::ptr_vector\<int\> v ;<br/>
std::copy(a.begin(), a.end(), boost::ptr_container::ptr_back_inserter(v)) ; //從後面插進去<br/>

----
###ScopeExit

簡單sample<br/>
當函數離開做用域時，會自動執行這個function<br/>
````
int * foo()
{
     int *i = new int{10} ;
     BOOST_SCOPE_EXIT(&i)
     {
          delete i;<br/>
          i = 0;<br/>
     } BOOST_SCOPE_EXIT_END
}
````

----
###Pool

透過boost pool來對memory pool進行監悾、分配<br/>
當該pool解構時，會自動free掉內存，避免memory leak的問題<br/>

1  boost::simple_segregated_storage<br/>
底層function，做出一個memory storage出，並且分配記憶體給別人<br/>
理論上我們並不需要用到他<br/>

2  boost::object_pool<br/>

````
  boost::object_pool<int> pool;

  int *i = pool.malloc();     // 只負責分配空間
  *i = 1;

  int *j = pool.construct(2); // 呼叫constructer
````

3  boost::singleton_pool<br/>
用法如下<br/>

````
struct intpool { };
struct intpool2 { };
typedef singleton_pool<intpool, sizeof(int)> ipool1;
typedef singleton_pool<intpool2, sizeof(int)> ipool2;
    for (int i = 0; i < 10; ++i)
     {
         int *q1 = (int *)ipool1::malloc();
         int *q2 = (int *)ipool2::malloc();
        *q1 = i;
        *q2 = i*i;
        cout << *q1 << " and " << *q2 << endl;
     }
     ipool1::purge_memory();
     ipool2::purge_memory();
````
4  boost::pool_allocator/boost::fast_pool_allocator<br/>
主要跟STL容器一起使用，替代STL的allocator<br/>
至於其他更高階的用法實在是沒有心力去追了阿~~~

