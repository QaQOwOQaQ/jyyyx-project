#include <myhead.h>

using namespace std;

#define MAX_LEVEL 8
// #define SKIPLIST_

// with a head node
class Skiplist 
{
public:
    struct Node
    {
        int val;
        vector<Node*> next;
        Node(int _val) 
            : val(_val), next(MAX_LEVEL) // initialization vector in class
                // next.resize(level, nullptr) ??
            {} 
    };
public:
    Skiplist(int _level = MAX_LEVEL) : level(_level), head(new Node(-1)) {}
    ~Skiplist() {}
    
    bool search(int target) 
    {
        vector<Node*> pre(level);
        find(target, pre);
        auto p = pre[0]->next[0];
        return p && p->val == target;
    }

    void add(int num)
    {
        vector<Node*> pre(level);
        find(num, pre);
        auto newNode = new Node(num);
        for(int i = 0; i < level; i ++ )
        {
            // insert between pre and pre->next
            newNode->next[i] = pre[i]->next[i];
            pre[i]->next[i] = newNode;
            if(rand() % 2)  break;
        }
    }

    bool erase(int num)
    {
        vector<Node*> pre(level);
        find(num, pre);
        auto p = pre[0]->next[0];
        if(!p || p->val != num) return false;
        for(int i = 0; i < level && pre[i]->next[i] == p; i ++ )
            pre[i]->next[i] = p->next[i];
        delete p;
        return true;
    }
private:
    int level;
    Node *head;
    void find(int num, vector<Node*>& pre) const 
    {
        auto p = head;
        for(int i = level - 1; i >= 0; i -- )
        {
            while(p->next[i] && p->next[i]->val < num)
                p = p->next[i];
            pre[i] = p;
        }
    }
};

int main()
{
    return 0;
}