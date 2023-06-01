#include <iostream>
#include <algorithm>
#include <cstring>
#include <string>
#include <unordered_map>
#include <list>
#include <random>
#include <vector>

// #include <lru_v2.h>
#include <mylist.h>
#include <mymap.h>

using namespace std;

struct node
{
    int key;
    string value;
    node(int _key, string _value) : key(_key), value(_value) {}
    bool operator==(const node &rhs) const {
        return key == rhs.key;
    }
};

// add template specialized class in namespace std 
// to override hash function for user-type
namespace std 
{
    template<>
    struct hash<node> 
    {
        int operator()(const node &_node) {
            return std::hash<int>{}(_node.key);
        }
    };
}

#ifdef TEST_LIST
static void test_list()
{
    constexpr int COUNT = 5;
    List<int> L;
    for(int i = 1; i <= COUNT; i ++ ) 
        L.addAtHead(i);
    cout << L << endl;
    for(int i = 1; i <= COUNT; i ++ ) 
        L.addAtTail(-i);
    cout << L << endl;
    for(int i = 1; i <= COUNT; i ++ ) 
        L.addAtIndex(i * 2, i * 10);
    cout << L << endl;
    cout << L.size() << endl;
    for(int i = 1, j = 2; i <= COUNT; i ++ , j ++ )
        L.deleteAtIndex(j);
    cout << L << endl;
    cout << L.size() << endl;   
    for(int i = 0; i < L.size(); i ++ )
    {
        cout << "[" << i << "]";
        if(L.find(i) != -1)
            cout << "get: " << i << endl;
        else 
            cout << "not find: " << i << endl;
    }    
    for(int i = 0; i < L.size(); i ++ )
        cout << L.get(i) << ' ';
    cout << endl;
}
#endif 

#ifdef TEST_STD_HASH_TABLE
static void test_std_hash_table()
{
    constexpr int COUNT = 20;
    LRUCache<int, string> L(10);
    for(int i = 0; i < COUNT; i ++ )
        L.put(i, string("Hello: ") + to_string(i));
    cout << L._cache.size() << endl;
    cout << L._cache << endl;
    for(int i = COUNT / 2; i < COUNT; i += 2)
        L.put(i, string("fuck~ ") + to_string(i * 10));
    cout << L._cache.size() << endl;
    cout << L._cache << endl;
    for(auto &it : L._map)
    {
        cout << it.first << ' ' << it.second << endl;
    }
}
#endif 

int main()
{
    cout << "LRU-test-starting------------>\n";
    
    cout << "LRU-test-ending------------>\n";
}