# KV-DB
## description
[使用 C++ 开发，基于 skiplist 实现的轻量级 KV 数据库 🔥🔥🚀](https://github.com/youngyangyang04/Skiplist-CPP)

## worse design
作者有很多不好甚至有错误的设计，例如：
1. 可以用 lock_guard 管理 mutex
2. 可以用 shared_ptr 管理指针
3. 指针数组用 vector<*> 比 T** 更优雅
4. 纯主观：不喜欢 class 内的成员变量加下划线
5. 不是很C++11，很多地方用 auto 更优雅
6. 用 nullptr 而非 NULL，作者根本不懂 C++11
7. endl 和 "\n" 的使用貌似杂乱无章？
8. 有些内存没有释放：例如 load_file 函数中的`*key`和`*value`，我搞不懂作者为啥这么喜欢爱分配内存？？明明可以不用的，例如这里明明可以用引用代替
9. level生成方法不好，很low
10. 对于锁的使用，也很让人迷惑，search() 难道不需要加锁吗？万一 search 的时候，数据被修改了呢？
11. 代码风格不好 😡
12. 在 get_key_value_from_string() 中，K 和 V 的类型被写死为了 string，但是 string 能转换的类型相当有限，例如整形和浮点型 string 都不能直接转换，因此会编译错误，我看 github 的 issue 里面有人提到过了，但是它都没有修改，除此之外，还有很多 issue 他都没有修改，呵呵，why ?


## good design
1.class 可以存放 ofstream 和 ifstream 对象 ^^
当需要使用时，使用 ifstream::open() 打开文件，ifstream::is_open() 判断文件是否成功打开
具体见 load_file() 和 dump_file()

2.关于类型转换，当我们要把磁盘上的文件加载到内存时，需要进行类型转换，但我们不能事先预知 KV 到底是什么类型。
我们可以用 <sstream> 中的 stringstream 解决，具体在 load_file() 中，但这么实现有一个问题，那就是如果 K,V 中不能存放空格，否则会出错，因为从 stringstream 中读取数据时，空格是作为分隔符存在的。


## strategy
1. 这里的 kv-engine 相当于 map，key 不可以重复且有序
2. 如果重复插入，那么忽略请求，std::map 是覆盖策略，要注意区分


## todo
1. key 如果是自定义类型，就 g 了，因为没有提供自定义比较函数的接口 😡😡😡
2. 锁的问题


## problem
发现了一个很坑爹的事，那就是如果使用 auto，vscode 就不会提示自动补全？

## stress-test
关于压力测试，请在该文件所在目录下执行命令 `make stress-test`。
另外，关于测试啥的，我还不太懂。。。

## 6.1
我原来没用 shared_ptr 吗？？
将所有的裸指针换成了智能指针，但是补全有问题，为啥啊？


