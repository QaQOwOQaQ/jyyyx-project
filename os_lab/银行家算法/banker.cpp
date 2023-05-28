#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>

#define M 10
#define N 20

using namespace std;

//N个进程M个资源
int n, m;
int ALL_Resource[M];
int Max[N][M];
int Avaiable[M];
int Allocation[N][M];
int Need[N][M];
int Request[N];
bool Finish[N];
int Work[N];

void init()
{
	cout << "请分别输入进程个数(n)和资源个数(m)：";
	cin >> n >> m;
	
	cout << "请录入该时刻的资源分配表" << endl;
	
	cout << "请输入各类资源总和(ALL_Resource)：" << endl;
	for(int j = 1; j <= m; j ++ )	
	{
		cin >> ALL_Resource[j];	
		Avaiable[j] = ALL_Resource[j];
	}
	
	cout << "请输入各个进程所需各类资源数量(Max)：" << endl;
	for(int i = 1; i <= n; i ++ )
		for(int j = 1; j <= m; j ++ )
		{
			cin >> Max[i][j];
			Need[i][j] = Max[i][j];
		}
		
	cout << "请输入各个进程当前时刻已分配资源情况(Allocation)：" << endl;
	for(int i = 1; i <= n; i ++ )
		for(int j = 1; j <= m; j ++ )
		{
			cin >> Allocation[i][j];
			Need[i][j] -= Allocation[i][j];
			Avaiable[j] -= Allocation[i][j];
		}
		
	cout << "通过统计得到各个进程当前时刻所需资源(Need)情况为:" << endl;
	for(int i = 1; i <= n; i ++ )
	{
		cout <<"P" << i << ": ";
		for(int j = 1; j <= m; j ++ )
			cout << Need[i][j] << ' ';
		cout << endl;
	}
	
	cout << "系统可分配资源数量(Avaiable)情况为：" << endl;
	for(int j = 1; j <= m; j ++ )	cout << Avaiable[j] << ' ';
	cout << endl;
}

bool cmp(int a[], int b[])//判断A数组的元素是否一一X小于等于B数组的元素
{
	for(int i = 1; i <= m; i ++ )	
		if(a[i] > b[i])
			return false;
	return true;
}

void output(int i)
{
	cout << "P" << i << '\t';
	for(int j = 1; j <= m; j ++ )	printf("%02d ", Work[j]);
	cout << '\t';
	for(int j = 1; j <= m; j ++ )	printf("%02d ", Need[i][j]);
	cout << '\t';
	for(int j = 1; j <= m; j ++ )	printf("%02d ", Allocation[i][j]);
	cout << '\t';
	for(int j = 1; j <= m; j ++ )	printf("%02d ", Work[j] + Allocation[i][j]);
	cout << '\t' << "True" << endl;
}

void update(int i)//进程号
{
	output(i);
	/*当给一个进程分配资源之后，有如下变化
	1.该进程的需求的资源减少
	2.该进程获得的资源增加
	3.系统中可用资源数量减少
	*/
	for(int j = 1; j <= m; j ++ )
	{
		Work[j] -= Need[i][j];
		Allocation[i][j] += Need[i][j];
	}
	
	//当进程执行结束之后,系统中可用资源数目增加
	for(int j = 1; j <= m; j ++ )
	{
		Work[j] += Allocation[i][j];
		Need[i][j] = 0;
	}
}

bool valid_sequence_check(vector<int> &sq)
{
	cout << "进程" << "      " << "Work" << "      " << "Need" << "      " 
		 << "Allocation" << "      " << "Work + Allocation" << "      " << "Finish" << endl;
	
	memset(Finish, false, sizeof Finish);
	
	for(int u = 1; u <= n; u ++ )
	{
		bool flag = false;
		for(int i = 1; i <= n; i ++ )
		{
			if(Finish[i])	continue;

			if(cmp(Need[i], Work))
			{
				flag = true;
				update(i);
				Finish[i] = true;
				sq.push_back(i);
			}
		}
		if(!flag)//找了一圈一个也没找到,直接结束
		{
			break;
		}
	}
	
	for(int i = 1; i <= n; i ++ )	
		if(!Finish[i])	return false;
	return true;
}

bool take_job()
{
	int id;
	cout << "请输入执行申请资源指令的进程号:";
	cin >> id;
	
	memset(Request, 0, sizeof Request);
	cout << "请输入所需资源数量：" << endl;
	for(int j = 1; j <= m; j ++ )	cin >> Request[j];
	
	if(!cmp(Request, Need[id]))
	{
		cout << "分配失败！所需资源数量超过最大需求的资源数量" << endl;
		return false;
	}
	
	if(!cmp(Request, Avaiable))
	{
		cout << "分配失败！所需资源数量超过系统可分配资源数量" << endl;
		return false;
	}
	
	//尝试性为该进程分配资源
	for(int j = 1; j <= m; j ++ )
	{
		Avaiable[j] -= Request[j];
		Allocation[id][j] += Request[j];
		Need[id][j] -= Request[j];
		
		Work[j] = Avaiable[j];
	}
	
	vector<int> sequence;
	if(!valid_sequence_check(sequence))
	{
		cout << "分配失败！不存在一个合法序列" << endl;
		for(int j = 1; j <= m; j ++ )//回溯
		{
			Avaiable[j] += Request[j];
			Allocation[id][j] -= Request[j];
			Need[id][j] += Request[j];
		}
		return false;	
	}
	
	cout << "一个合法序列如下：";
	for(auto &x : sequence)	cout << "P" << x << " ->";
	cout << "done!" << endl;
	
	return true;
}

int main()
{
	init();
	bool is_done = take_job();
	if(!is_done)	puts("分配不合法！");
	
	
	return 0;
}

/*测试样例
5 3

10 5 7

7 5 3
3 2 2
9 0 2
2 2 2
4 3 3

0 1 0
2 0 0 
3 0 2
2 1 1
0 0 2

2
0 0 0



*/
