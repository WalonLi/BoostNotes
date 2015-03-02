/**
  Author: Walon Li
 **/

#include <iostream>
#include <set>
#include <string>
#include <boost/algorithm/string.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_array.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/ptr_container/ptr_set.hpp>
#include <boost/scope_exit.hpp>
#include <boost/pool/object_pool.hpp>


using namespace std;

void smart_poiner()
{
    /**
     * smart pointer
     */
    boost::scoped_ptr<int> p{new int{1}} ;
    cout << *p << "\n";
    p.reset(new int{2});
    cout << *p << "\n";
    p.reset();
    // cout << *p ; <-it's error, because it have been free


    auto p1 = boost::make_shared<int>(1) ;
    cout << typeid(p1).name() << "\n";

    boost::shared_array<int> p2{new int[1]} ;
    {
        boost::shared_array<int> p3{p2} ;
        p3[0] = 2 ;
    }
    cout << p2[0] << "\n" ;
}

void pointer_container()
{
    /**
     * Pointer Container
     */
    boost::ptr_vector<int> v1 ;
    v1.push_back(new int(1));
    cout << v1.back() << "\n";

    // test std::set
    std::set<std::unique_ptr<int>> v2 ;
    v2.insert(std::unique_ptr<int>(new int{2})) ;
    v2.insert(std::unique_ptr<int>(new int{1})) ;
    std::cout << **v2.begin() << "\n" ;

    // boost set, it will auto sort by int value
    boost::ptr_set<int> v3 ;
    v3.insert(new int(2)) ;
    v3.insert(new int(1)) ;
    std::cout << *v3.begin() << "\n";

}

int* scope_exit()
{
    int *i = new int{10};
    BOOST_SCOPE_EXIT(&i)
    {
        delete i;
        i = 0 ;
    } BOOST_SCOPE_EXIT_END

    cout << *i << "\n" ;
    return i ;
}

void pool()
{
    boost::object_pool<int> pool;
    int *i = pool.malloc() ;
    *i = 1 ;
    cout << *i << endl ;

    int *j = pool.construct(2) ;

    cout << *j << " " << pool.get_next_size() << endl;
}


int main()
{
    // pointer_container() ;
    //int *j = scope_exit() ;
    //cout << j <<" " << test();
    pool();
    cout << endl ;

    return 0;
}


