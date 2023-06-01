#ifndef LRU_CACHE_GUARD
#define LRU_CACHE_GUARD

#include <iostream>
#include <algorithm>
#include <memory>
#include <unordered_map>

#include <mylist.h>

#define DEBUG

/* Design Pattern
* 1. use bidirection-linked-list to implement LRU strategy
* move the nodes to list head which be used recently
* 2. use hash mapping to ensure get() and put() executed in O(1)
* Actually, I maintain a bidirection linked list to store value
* and use hash mapping to get value by unique key in O(1) time complexiyu
* so we need to matain two containers: one birection list and other hash mapping
* hash mapping store the mapping from int(key) to list node(value)  */

/*=========================*/
/*   LRU CACHE VERSION 1   */
/*=========================*/

template<typename Key, typename Value>
class LRUCache 
{
public:
    LRUCache(int cap) : _capacity(cap), _size(0) {}

    // if key have existed in cache 
    // return mapped value and move to head
    // else return default value
    Value get(const Key &key)
    {
        int idx = -1;
        if((idx = _cache.find(key)) == -1)  
            return Value{};
        // move to head
        _cache.deleteAtIndex(idx);
        _cache.addAtHead(key);
        return _map[key];
    }

    // if key have existed in cache 
    // change the mapped value and move to head
    // else insert this K-V into head
    void put(const Key &key, const Value &value)
    {
        int idx = -1;
        if((idx = _cache.find(key)) == -1)  
        {
            if(_size >= _capacity)
            {
                _cache.deleteAtIndex(_size - 1);
                _size -- ;
            }
            _cache.addAtHead(key);
            _size ++ ;
        }
        else 
        {
            _cache.deleteAtIndex(idx);
            _cache.addAtHead(key);
        }
        _map[key] = value;
    }
    void put(const Key &key, const Value &&value)
    {
        int idx = -1;
        if((idx = _cache.find(key)) == -1)  
        {
            if(_size >= _capacity)
            {
                _cache.deleteAtIndex(_size - 1);
                _size -- ;
            }
            _cache.addAtHead(key);
            _size ++ ;
        }
        else 
        {
            _cache.deleteAtIndex(idx);
            _cache.addAtHead(key);
        }
        _map[key] = std::move(value);
    }

private:
#ifdef DEBUG
public:
#endif 
    const int _capacity;
    int _size;
    List<Key> _cache;
    std::unordered_map<Key, Value> _map;
};

#endif 
