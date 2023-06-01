//OPT
#include <iostream>
#include <cstring>
#include <vector>

#define N 110

using namespace std;

int page_size, block_cnt, page_cnt;
int visit[N];//visit越大表示访问的越晚 
struct Node{
    int page;
    int block;
}node[N];

//Init要放在read的后面，因为需要用到页数和物理块数 
void Init()
{
    for(int i = 0; i < block_cnt; i ++ )    
    {
        node[i].page = -1;
        node[i].block = i;
    }
    for(int i = 0; i < page_cnt; i ++ )	visit[i] = 0;
}

int Read(vector<int> &LA)
{
    cout << "请输入页面大小和物理块数: ";
    cin >> page_size >> block_cnt;
    page_size = 1;
    block_cnt = 3;

    int logical_address;
    cout << "请依次输入页面的逻辑地址（输入-1结束）：";
    while(cin >> logical_address, logical_address != -1)
        LA.push_back(logical_address);

	// int test[] = {1,2,3,4,1,3,4,2,5,2,4,1};
	// for(int i = 0; i < 12; i ++ )	LA.push_back(test[i]);
	
	cout << "size: " << LA.size() << endl;
    for(auto &x : LA)   cout << x << ' ';
    cout << endl;

    return (int)LA.size();
}

void out_Message()
{
    cout << "物理块号" << '\t' << "页号" << endl;
    for(int i = 0; i < block_cnt; i ++ )
    {
        string page = to_string(node[i].page);
        if(node[i].page == -1)  page = "void";
        cout << node[i].block << '\t' << '\t' << page << endl; 
    }
    cout << endl;
}

//找到一个可以存放该页的位置
int get_Block(int page_num)
{
    //页号已经在物理块
    for(int i = 0; i < block_cnt; i ++ )
        if(node[i].page == page_num)
            return block_cnt;
    
    //有空闲物理块 
    for(int i = 0; i < block_cnt; i ++ )
    	if(node[i].page == -1)
    		return i;
    
    //无空闲物理块 
    return -1;   
}

//将该页放入物理块中
void put_Block(int pos, int page_num)
{
    node[pos].page = page_num;
    visit[pos] ++ ;
}

//替换出一页 
int replace_Block(int it, const vector<int> &v)
{
	for(int i = 0; i < page_cnt; i ++ )	visit[v[i] / page_size] = page_cnt;	
	for(int i = it + 1; i < page_cnt; i ++ )//遍历后面的所有页 
    {
        int page_num = v[i] / page_size;
        for(int j = 0; j < block_cnt; j ++ )//查找物理块中的页 
			if(node[j].page == page_num)//如果是物理块中的页	
				visit[node[j].page] = min(visit[node[j].page], i);						
    }

	cout << "visit情况：";
	for(int i = 0; i < block_cnt; i ++ )	cout << visit[node[i].page] << ' ';
	cout << endl;

    int id = -1, last = -1;
    for(int i = 0; i < block_cnt; i ++ )
        if(visit[node[i].page] > last)
        {
            last = visit[node[i].page];
            id = i;
        }
    
    //返回物理块号 
    return id;  
}

void OPT(const vector<int> &v)
{
    for(int i = 0; i < page_cnt; i ++ )
    {
        int page = v[i];
        int page_num = page / page_size, page_offset = page % page_size;
        
        cout << "---------------------------------------------------------------------" << endl;
        cout << '\t' << "逻辑地址：" << page << endl; 
        cout << "页号：" << page_num << ' ' << "页内偏移：" << page_offset << endl;
        
        int pos = get_Block(page_num);
        
        if(pos == block_cnt)
        {
            cout << "该页号已经存在于物理块中!" << endl;
        }
        else if(pos != -1)
        {
            cout << "内存中还有空闲的物理块! " << endl;
            put_Block(pos, page_num);
        }
        else//pos==-1
        {
            cout << "内存中没有空闲的物理块! ";
            int back = replace_Block(i, v);
            cout << "置换出的页号为：" << node[back].page << endl;
            put_Block(back, page_num);
        }
        
        out_Message();
    }
}

int main()
{
    vector<int> LA;

    page_cnt = Read(LA);
	Init();
        
    OPT(LA);

    cout << "done" << endl;

    return 0;
}

/*test case1
1 3
1 2 3 4 1 3 4 2 5 2 4 1

add 1: 			1 -1 -1
add 2: 			1 2 -1
add 3: 			1 2 3
change 2 -> 4: 	1 4 3 
continue;
continue;
continue;
change 3 -> 2: 	1 4 2
change 1 -> 5: 	5 4 2
continue;
continue;
change * -> 1   1 * *

*/
