#include <iostream>
#include <cstring>
#include <vector>
#include <queue>

#define N 110

using namespace std;

struct Node{
    int block;
    int page;
}node[N];

queue<Node> memory;

int Size, Cnt;      // N：页面大小；M：物理块数
int idx;            //内存中页的个数
bool in_block[N];   //某页是否在内存当中

void out_Message() //输出中间信息
{
    cout << "物理块号与页号的映射关系为：" << endl;
    for (int i = 0; i < Cnt; i ++ )
    {
        string page = node[i].page == -1 ? "Empty" : to_string(node[i].page);
        cout << "物理块号：" << node[i].block << " 页号：" << page << endl;
    }
    cout << "--------------------------------------------------------" << endl;
}

void FIFO(vector<int> &v)
{
    for(auto &addr : v)
    {
        //打印页的信息
        int pageNum = addr / Size;
        int pageOffset = addr % Size;
        cout << "\t逻辑地址：" << addr << endl;
        cout << "\t页号: " << pageNum << " 页内偏移: " << pageOffset << endl;

        //FIFO
        cout << "\t状态信息：";
        if(in_block[pageNum])//在内存中
        {
            cout << "该页在内存当中！" << endl;
        }
        else if(memory.size() < Cnt)//不在内存中但是还有空闲的物理块,调用
        {
            in_block[pageNum] = true;
            cout << "该页不在内存中，内存中还有空闲的物理块，调入该页" << endl;
            node[memory.size()].page = pageNum;
            memory.push(node[memory.size()]);
        }
        else//不在内存中并且没有空闲的物理块,置换
        {
            in_block[pageNum] = true;
            auto back = memory.front();
            cout << "该页不在内存中，内存中没有空闲的物理块，置换出最早进入内存的页：" << back.page << endl;
            node[back.block].page = pageNum;
            memory.pop();
            memory.push({back.block, pageNum});
        }
        out_Message();
    }
}

void Init()
{
    for (int i = 0; i < N; i ++ )
    {
        node[i].block = i;
        node[i].page = -1;//没有放入页
    }
}

void Read(vector<int> &LA)
{
    cout << "请输入页的大小和物理块个数：";
    cin >> Size >> Cnt;
    cout << "请输入页面的逻辑地址，输入-1结束: ";
    int addr;
    while(cin >> addr, addr != -1)
        LA.push_back(addr);
}

int main()
{
    vector<int> LA;//Logical_Address
    Init();
    Read(LA);
    FIFO(LA);
    return 0;
}