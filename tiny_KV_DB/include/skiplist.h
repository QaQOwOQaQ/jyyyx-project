#ifndef SKILLIST_GUARD
#define SKIPLIST_GUARD

#include <iostream>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <random>
#include <fstream>
#include <mutex>
#include <memory>
#include <sstream>  // type convertion

#include <node.h>

#define STORE_FILE "./store/dumpFile"
#define SKIPLIST_P 0.25
#define MAX_LEVEL 32

std::mutex mut;
std::string delimiter = ":"; // 界定符
std::mt19937 gen;

/*=====================*/
/*   SkipList Define   */
/*=====================*/

template<typename K, typename V>
class SkipList
{
// template alias
template<typename U1, typename U2>
using SNode = std::shared_ptr<Node<U1,U2>>;

public:
    SkipList();
    ~SkipList();
    int insert_element(K,V);
    void displat_list() const ;
    bool search_element(K);
    void delete_element(K);
    void dump_file();
    void load_file();
    int size() const ;
private:
    SNode<K,V> create_node(K,V,int/*level*/);
    int get_random_level(); // similar to my function name? hh😊
    bool is_valid_string(const std::string &str);
    void get_key_value_from_string(
        const std::string &str, K &key, V &value);
private:
    // cur level of skip list
    int cur_level;
    // skip list cur element count
    int element_count;
    // pointer to head node
    SNode<K,V> head;
    // file operator
    std::ofstream file_writer;
    std::ifstream file_reader;
};



/*=======================*/
/*   SkipList Implement  */
/*=======================*/
template<typename K, typename V>
typename std::shared_ptr<Node<K,V>> SkipList<K,V>::create_node(K _k,V _v,int _level)
{
    return std::make_shared<Node<K,V>>(_k, _v, _level);
}

// insert given (key,value) in skip list
// return 1 means element exists
// return 0 means insert successfully
/* 
                           +------------+
                           |  insert 50 |
                           +------------+
level 4     +-->1+                                                      100
                 |
                 |                      insert +----+
level 3         1+-------->10+---------------> | 50 |          70       100
                                               |    |
                                               |    |
level 2         1          10         30       | 50 |          70       100
                                               |    |
                                               |    |
level 1         1    4     10         30       | 50 |          70       100
                                               |    |
                                               |    |
level 0         1    4   9 10         30   40  | 50 |  60      70       100
                                               +----+

*/

// return 1 if that k is already existed 
// else insert this k,v into skiplist and return 0
template<typename K, typename V>
int SkipList<K,V>::insert_element(K _key, V _value)
{
    // create update array and initialize it
    // update is array which put node that node->forward[i] should be operated later
    std::vector<SNode<K,V>> update(MAX_LEVEL + 1, head);
    
    // add mutex
    std::lock_guard<std::mutex> lock(mut);
    auto cur = head;

    // start from higest level of silp list
    for(int i = MAX_LEVEL; i >= 0; i -- )
    {
        while(cur->forward[i] && cur->forward[i]->get_key() < _key)
            cur = cur->forward[i];
        update[i] = cur;
    }

    // reached level 0 and forward pointer to right node
    // which is desired to insert key
    cur = cur->forward[0];
    
    // if cur node have key equal to search key, we get it
    if(cur && cur->get_key() == _key)
    {
        std::cout << "key: " << _key << ", exist: " << cur->get_value() << std::endl;
        return 1;
    }

    // if go here, it means:
    // if cur is NULL that means we have reached to end of level
    // if cur's key is not equal this _key 
    //   -- that means we have to insert node between update[0] and cur node

    // generate a random level for node
    int random_level = get_random_level();

    // if random level is greater than skip list's cur level
    // change cur_level
    if(random_level > cur_level)  
        cur_level = random_level;

    // create new node with random level generated
    auto newNode = create_node(_key, _value, random_level);
    
    // insert node (update, newNode, update->next)
    for(int i = 0; i <= random_level; i ++ )
    {
        newNode->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = newNode;
    }
    std::cout << "successfully insert key: " << _key 
        << ", value: " << _value << std::endl;
    element_count ++ ;
    return 0;
}

template<typename K, typename V>
void SkipList<K,V>::displat_list() const 
{
    std::cout << "\n****Skip List****" << "\n";
    for(int i = 0; i <= cur_level; i ++ )
    {
        auto cur = head->forward[i]; // skip head node
        std::cout << "Level " << i << ": ";
        while(cur->forward[i] != nullptr)
        {
            // (K1,V1), (K2,V2), (K3,V3) ... (Kn, Vn),
            // TODO：remove the last ','
            std::cout << "(" << cur->get_key() << "," 
                << cur->get_value() << "), ";
            cur = cur->forward[i];
        }
        std::cout << std::endl;
    }
}

// dump all data into memory file
// [Warnning] the format of dump file and load file must be identical
template<typename K, typename V>
void SkipList<K,V>::dump_file()
{
    std::cout << "dump file-------------------->" << std::endl;
    file_writer.open(STORE_FILE);
    if(!file_writer.is_open())  
    {
        std::cout << "[Error] open file error" << std::endl;
        return ;
    }
    // only need to write level 0
    auto cur = head->forward[0];
    while(cur)
    {
        // K1:V1
        // K2:V2
        // ...
        // Kn:Vn
        file_writer << cur->get_key() << ":" << cur->get_value() << "\n";
        std::cout << cur->get_key() << ":" << cur->get_value() << ";\n";
        // cur = cur->forward[0];
        cur = cur->forward[0];
    }
    // flush immediately
    file_writer.flush();
    file_writer.close();
    std::cout << "saved into disk successfully--------->" << std::endl;
    return ;
}

// load data from disk
template<typename K, typename V>
void SkipList<K,V>::load_file()
{
    file_reader.open(STORE_FILE);
    std::cout << "load file----------->" << std::endl;
    if(!file_reader.is_open())
    {
        std::cout << "[Error] open file error!" << std::endl;
        return ;
    }
    std::string line;
    K key;
    V value;
    while(getline(file_reader, line))
    {
        get_key_value_from_string(line, key, value);
        insert_element(key, value);
        std::cout << "key: " << key << ", value: " << value << std::endl;
    }
    file_reader.close();
}

template<typename K, typename V>
int SkipList<K,V>::size() const 
{
    return element_count;
}

template<typename K, typename V>
void SkipList<K,V>::get_key_value_from_string(
    const std::string &str, K &key, V &value)
{
    if(!is_valid_string(str))   return ;
    std::stringstream sstr;
    // add a space explicitly to split key string and value string
    sstr << str.substr(0, str.find(delimiter)) << " " << str.substr(str.find(delimiter) + 1);
    sstr >> key >> value;
}

template<typename K, typename V>
bool SkipList<K,V>::is_valid_string(const std::string &str)
{
    return !str.empty() && 
        str.find(delimiter) != std::string::npos;
}

// TODO: if dind't need to lock ????
// Search for element in skip list 
/*
                           +------------+
                           |  select 60 |
                           +------------+
level 4     +-->1+                                                      100
                 |
                 |
level 3         1+-------->10+------------------>50+           70       100
                                                   |
                                                   |
level 2         1          10         30         50|           70       100
                                                   |
                                                   |
level 1         1    4     10         30         50|           70       100
                                                   |
                                                   |
level 0         1    4   9 10         30   40    50+-->60      70       100
*/
template<typename K, typename V>
bool SkipList<K,V>::search_element(K _key)
{
    std::cout << "search element---------------" << std::endl;
    auto cur = head;
    for(int i = cur_level; i >= 0; i -- )
    {
        while(cur->forward[i] && cur->forward[i]->get_key() < _key)
            cur = cur->forward[i];
    }
    cur = cur->forward[0];
    if(cur && cur->get_key() == _key)
    {
        std::cout << "Found key: " << _key 
            << ", value: " << cur->get_value() << std::endl;
        return true;
    }
    std::cout << "Not Found Key:" << _key << std::endl;
    return false;
}

// TODO
template<typename K, typename V>
void SkipList<K,V>::delete_element(K _key)
{
    std::lock_guard<std::mutex> lock(mut);
    auto cur = head;
    std::vector<SNode<K,V>> update(MAX_LEVEL + 1, head);
    for(int i = cur_level; i >= 0; i -- )
    {
        while(cur->forward[i] && cur->forward[i]->get_key() < _key)
            cur = cur->forward[i];
        update[i] = cur;
    }

    cur = cur->forward[0];
    // delete when existed
    if(cur || cur->get_key() == _key)
    {
        for(int i = 0; i <= cur_level; i ++ ) 
        {
            if(update[i]->forward[i] != cur)
                break;
            update[i]->forward[i] = cur->forward[i];
        } 
        // decrease level when no data this level
        while(cur_level > 0 && head->forward[cur_level] == nullptr)
            cur_level -- ;
        std::cout << "Successfully deleted key "<< _key << std::endl;
        element_count -- ;
    }
}

template<typename K, typename V>
SkipList<K,V>::SkipList()
: cur_level(0), element_count(0), head(std::make_shared<Node<K,V>>(K(), V(), MAX_LEVEL))
{}

template<typename K, typename V>
SkipList<K,V>::~SkipList()
{
    if(file_reader.is_open())   file_reader.close();
    if(file_writer.is_open())   file_writer.close();
}

template<typename K, typename V>
int SkipList<K,V>::get_random_level()
{
    int level = 1;
    while(((double)gen() / gen.max()) < SKIPLIST_P && level < MAX_LEVEL)
        level ++ ;
    return level;
}


#endif 