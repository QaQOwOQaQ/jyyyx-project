#include <iostream>
#include <cstring>
#include <vector>
#include <list>

using namespace std;

int Size, Cnt;
list<int> q;

void out_Message() //输出中间信息
{
    //物理块号小的是最近使用优先级最高的
    int idx = 0;
    cout << "物理块号与页号的映射关系为：" << endl;
    cout << "内存中存放的页数为：" << q.size() << endl;
    for (auto it = q.begin(); it != q.end(); ++ it)
    {
        cout << "物理块号：" << idx ++ << " 页号：" << *it << endl;
    }
    cout << "--------------------------------------------------------" << endl;
}

void LRU(vector<int> &v)
{
    for(auto &addr : v)
    {
        //打印页的信息
        int pageNum = addr / Size;
        int pageOffset = addr % Size;
        cout << "\t逻辑地址：" << addr << endl;
        cout << "\t页号: " << pageNum << " 页内偏移: " << pageOffset << endl;

        //LRU,最近加入的在队头
        cout << "\t状态信息：";
            //该页在内存当中
        bool has_found = false;
        for (auto it = q.begin(); it != q.end(); ++ it)
        {
            int page = *it;
            if(page == pageNum)
            {
                cout << "该页在内存当中" << endl;
                q.erase(it);
                q.push_front(page);

                has_found = true;
                break;
            }
        }
        if(has_found)
        {
            out_Message();
            continue;
        }
            //该页不在内存当中
        if(q.size() < Cnt)
        {
            cout << "该页不在内存中，内存中还有空闲的物理块，调入该页" << endl;
        }
        else
        {
            auto back = q.back();
            cout << "该页不在内存中，内存中没有空闲的物理块，置换出最早进入内存的页：" << back << endl;
            q.pop_back();
        }
        q.push_front(pageNum);//加入队列要放在判断的前面,否则q.size()大小会被改变
        out_Message();
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
    vector<int> LA;
    Read(LA);
    LRU(LA);
}

/*test1
256 3
128 1024 512 1025 513 256 257 1 2048 -1

//对应的页号为：
0 4 2 4 2 1 1 0 8
*/