#ifndef MY_BIDIRECTION_LIST_GUARD
#define MY_BIDIRECTION_LIST_GUARD

#include <iostream>
#include <memory>
#include <exception>

/* [TODO]
* Because this list is a bidirection list
* so there is a optimized way 
* when we get a node by index:
* if index > size/2 we can find reverse
* elif index <= size/2 we find forward 
* you can refer and test here [https://leetcode.cn/submissions/detail/366248813/] */

/*===========================*/
/*  BIDIRECTION LINKED LIST  */
/*============================*/
template<typename T>
class List
{
// [TODO] if there is a better way to do that ? 🤔
    template<typename Key, typename Value>
    friend class LRUCache;
    struct Node 
    {
        T val;
        std::shared_ptr<Node> prev; 
        std::shared_ptr<Node> next; 
        Node(T _val) : val(_val), prev(nullptr), next(nullptr) {}
        Node(T _val, std::shared_ptr<Node> _prev, std::shared_ptr<Node> _next)
            : val(_val), prev(_prev), next(_next) 
            {}
    };

public:
    template<typename U>
    friend std::ostream& operator<<(std::ostream &os, const List<U> &L) ;
// Have a virtual head node 😊
public:
    List();
    int get(int _idx) const;
    void addAtHead(T _val);
    void addAtTail(T _val);
    void addAtIndex(int _index, T _val);
    void deleteAtIndex(int _idx);
    int find(T _val) const ;
    int size() const { return _size; }
private:
    int _size;
    std::shared_ptr<Node> head;
    std::shared_ptr<Node> tail;
    void checkIndexValid(int _idx) const ;
};

template<typename T>
List<T>::List()
:   _size(0), head(std::make_shared<Node>(-1)), 
    tail(std::make_shared<Node>(-1)) 
{
    head->next = tail;
    tail->prev = head;
}

// check the index if valid 
template<typename T>
void List<T>::checkIndexValid(int _idx) const 
{
    if(_idx < 0 || _idx >= _size)   
        throw(std::range_error("[error] Not valid index"));
}

// return idx if find else return -1
template<typename T>
int List<T>::find(T _val) const 
{
    int idx = 0;
    auto cur = head->next;
    while(cur != tail)
    {
        if(cur->val == _val)    return idx;
        cur = cur->next;
        idx ++ ;
    }
    return -1;
}

// get value by index
template<typename T>
int List<T>::get(int _idx) const 
{
    checkIndexValid(_idx);
    auto cur = head;
    int counter = 0;
    while(cur != tail && counter < _idx)
    {
        counter ++ ;
        cur = cur->next;
    }
    return cur->next->val;
}

template<typename T>
void List<T>::addAtHead(T _val)
{
    /* |next|  -> |  next |  ->  |   next   |
       |head|     |newnode|      |head->next|
       |prev|  <- | prev  |  <-  |   prev   | */
    auto newNode = std::make_shared<Node>(_val, head, head->next);
    head->next->prev = newNode;
    head->next = newNode;
    _size ++ ;
}

template<typename T>
void List<T>::addAtTail(T _val)
{
    // tail->prev  <--> newnode <--> tail
    auto newNode = std::make_shared<Node>(_val, tail->prev, tail);
    tail->prev->next = newNode;
    tail->prev = newNode;
    _size ++ ;
}

// add before index
template<typename T>
void List<T>::addAtIndex(int _idx, T _val)
{
    checkIndexValid(_idx);

    // [BUG]
    // if(_idx == 0)  addAtHead(_val);
    // that code is invalid because we want to call addAtHead(T _val)
    // when _idx == 0 and then return void
    // but in this way, it will execute go on 
    // the should correct it by: return addAtHead(_val);
    // but call addAtHead(T _val) immediately
    if(_idx == 0)   return addAtHead(_val);
    if(_idx == _size - 1)   return addAtTail(_val);

    auto cur = head;
    for(int i = 0; i < _idx; i ++ )
        cur = cur->next;
    // cur <--> newNode <--> cur->next
    auto newNode = std::make_shared<Node>(_val, cur, cur->next);
    cur->next->prev = newNode;
    cur->next = newNode;
    _size ++ ;
}

template<typename T>
void List<T>::deleteAtIndex(int _idx)
{
    checkIndexValid(_idx);
    auto cur = head;
    for(int i = 0; i < _idx; i ++ )
        cur = cur->next;
    // cur (cur->next)~~ cur->next->next
    auto deleteNode = cur->next;
    deleteNode->next->prev = cur;
    cur->next = deleteNode->next;
    _size -- ;
}

template<typename T>
std::ostream& operator<<(std::ostream &os, const List<T> &L)  
{
    auto cur = L.head->next;
    os << "list: ";
    while(cur != L.tail)
    {
        os << cur->val << ' ';
        cur = cur->next;
    }
    return os;
}

#endif 