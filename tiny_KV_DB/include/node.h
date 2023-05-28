#ifndef SKIPLIST_NODE_GUARD
#define SKIPLIST_NODE_GUARD

/*=================*/
/*   Node Define   */
/*=================*/
template<typename K, typename V>
class Node 
{
// [Error] when initialize order not match declare order
private:
    K key;
    V value;
public:
    Node() = default;
    ~Node();
    Node(K _k, V _v, int _level);

    K get_key() const;
    V get_value() const;
    void set_value(V);
public:
    int node_level;
    std::vector<Node<K,V>*> forward;
};


/*====================*/
/*   Node Implement   */
/*====================*/
template<typename K, typename V>
Node<K,V>::Node(K _k, V _v, int _level)
: key(_k), value(_v), node_level(_level), forward(_level + 1)
{}

template<typename K, typename V>
Node<K,V>::~Node() // ~作为函数名的一部分？
{}

template<typename K, typename V>
K Node<K,V>::get_key() const
{
    return key;
}

template<typename K, typename V>
V Node<K,V>::get_value() const
{
    return value;
}

template<typename K, typename V>
void Node<K,V>::set_value(V _value)
{
    value = _value;
}


#endif