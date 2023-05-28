// simple version 1 of skiplist
#ifndef SKIPLIST_V1_GUARD
#define SKIPLIST_V1_GUARD

#include <myheader.h>

constexpr int SKIPLIST_MAX_LEVEL = 32;
constexpr double SKIPLIST_P = 0.25;

// default keep increment sequence with a virtual head node
class Skiplist
{
    // type of const static must be an integer
    const static int level = 32; // fixed level
private:
    struct Node 
    {
        int val;
        std::vector<std::shared_ptr<Node>> next;
        Node(int _val) : val(_val), next(level) {}
    };
    using SNode = std::shared_ptr<Node>;

public:
    Skiplist() : head(std::make_shared<Node>(-1)) {}
    bool search(int num)
    {
        std::vector<SNode> pre(level);
        find(num, pre);
        // get the lowest level pre node
        auto p = pre[0]->next[0];
        return p && p->val == num;
    }
    void add(int num)
    {
        std::vector<SNode> pre(level);
        find(num, pre);
        auto p = std::make_shared<Node>(num);
        int random_level = get_random_level();
        for(int i = 0; i < random_level; i ++ )
        {
            p->next[i] = pre[i]->next[i];
            pre[i]->next[i] = p;
        }
    }
    // return false if not exist
    bool erase(int num)
    {
        // if there check if not exist can use search() ?
        // but it will make performance worse ...
        std::vector<SNode> pre(level);
        find(num, pre);
        auto p = pre[0]->next[0];
        if(!p || p->val != num) return false;

        // delete every level 
        for(int i = 0; i < level && pre[i]->next[i] == p; i ++ )
            pre[i]->next[i] = p->next[i]; // make this level's ptr point to next
        return true;
    }
    int count(int num)
    {
        std::vector<SNode> pre(level);
        find(num, pre);
        auto p = pre[0]->next[0];
        if(!p || p->val != num)    return 0;
        int res = 0;
        while(p && p->val == num)
        {
            res ++ ;
            p = p->next[0];
        }
        return res;
    }
    /* TODO */
    // bool modify();
private:
    SNode head;
    // vector<Node*>pre store the Node* which 
    // have the biggest val and less than num 
    void find(int num, std::vector<SNode> &pre)
    {
        auto p = head;
        for(int i = level - 1; i >= 0; i -- )
        {
            while(p->next[i] && p->next[i]->val < num)   
                p = p->next[i];
            pre[i] = p; // cant use push_back() to add element otherwise the order wll reverse
        }
        /* we can ensure pre have count of level numbers 
        * because we set a head node 
        */
    }
    int get_random_level()
    {
        int rlevel = 1;
        while(((double)rand() / RAND_MAX) < SKIPLIST_P && rlevel < SKIPLIST_MAX_LEVEL)
            rlevel ++ ;
        return rlevel;
    }
};

#endif 