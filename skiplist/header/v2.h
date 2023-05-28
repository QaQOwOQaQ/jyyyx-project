// simple version 2 of skiplist
#ifndef SKIPLIST_V2_GUARD
#define SKIPLIST_V2_GUARD

#include <myheader.h>

// redis set this way
constexpr int MAX_LEVEL = 32;
constexpr double SKIPLIST_P = 0.25;

// default increment and with a head node
class Skiplist
{
private:
    struct Node 
    {
        int val;
        std::vector<std::shared_ptr<Node>> next;
        Node(int _val, int _level) : val(_val), next(_level) {}
    };
    using SNode = std::shared_ptr<Node>;

public: 
    Skiplist() 
        : head(std::make_shared<Node>(-1, MAX_LEVEL)) 
        {} // head node shuld full level

    bool search(int num)
    {
        auto p = head;
        for(int i = curlevel - 1; i >= 0; i -- )
        {
            while(p->next[i] && p->next[i]->val < num)
                p = p->next[i];
        }
        return p->next[0] && p->next[0]->val == num;
    }

    void add(int num)
    {
        std::vector<SNode> pre(MAX_LEVEL, head);
        get_pre(num, pre);

        auto level = get_random_level();
        curlevel = std::max(curlevel, level);
        assert(level);
        auto newNode = std::make_shared<Node>(num, level);
        for(int i = 0; i < level; i ++ )
        {
            // insert newNode between pre and pre->next
            newNode->next[i] = pre[i]->next[i];
            pre[i]->next[i] = newNode;
        }
    }

    // return false if not exist
    bool erase(int num)
    {
        std::vector<SNode> pre(MAX_LEVEL, head);
        get_pre(num, pre);
        auto p = pre[0]->next[0];
        if(!p || p->val != num) return false;
        for(int i = 0; i < curlevel; i ++ )
        {
            if(pre[i]->next[i] != p)    break;
            pre[i]->next[i] = p->next[i];
        }
        // if the tallest level only have head node, delete this level
        while(curlevel > 1 && !head->next[curlevel - 1])
            curlevel -- ;
        return true;
    }

    int count(int num)
    {
        auto p = head;
        for(int i = curlevel - 1; i >= 0; i -- )
        {
            while(p->next[i] && p->next[i]->val < num)
                p = p->next[i];
        }
        p = p->next[0];
        if(!p || p->val != num)   return 0;
        int res = 0;
        while(p && p->val == num)  
        {
            res ++ ;
            p = p->next[0];
        }
        return res;
    }

private:
    int curlevel;
    SNode head;
    std::mt19937 gen;

    int get_random_level()
    {
        int rlevel = 1; // initialized to 1 not 0
        while(((double)gen() / gen.max()) < SKIPLIST_P && rlevel < MAX_LEVEL)
            rlevel ++ ;
        return rlevel;
    }

    // expect caller has been alloc for pre 
    void get_pre(int num, std::vector<SNode> &pre)
    {
        auto p = head;
        for(int i = curlevel - 1; i >= 0; i -- )
        {
            while(p->next[i] && p->next[i]->val < num)
                p = p->next[i];
            pre[i] = p;
        }
    }
};


#endif 