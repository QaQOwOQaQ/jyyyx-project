# skiplist principle and implement
## 1. reference
[redis用skiplist实现zset，MAX_LEVEL和P的定义在server.h](https://github.com/redis/redis/blob/unstable/src/t_zset.c)
[y总简单实现(V1参考)](https://leetcode.cn/problems/design-skiplist/solution/by-tonngw-ls2k/)
[V2参考](https://leetcode.cn/problems/design-skiplist/solution/she-ji-tiao-biao-by-capital-worker-3vqk/)
[原理](https://leetcode.cn/problems/design-skiplist/solution/she-ji-tiao-biao-by-capital-worker-3vqk/)
[OIWiKi](https://oi-wiki.org/ds/skiplist/)
[代码随想录项目](https://github.com/youngyangyang04/Skiplist-CPP)
[跳表实现TODO：没时间可以先不看](https://github.com/JelinYao/skiplist)

## 2. test
[LC测试题目](https://leetcode.cn/problems/design-skiplist/submissions/)
`src/test.cpp`

## 3. principle
在传统的单链表中，每个节点存放了指向下一个节点的引用，我们查找元素时，需要从链表头开始依次遍历，时间复杂度为 $O(N)$，这种查找的效率是很低的。
$William Pugh$ 发明了 $SkipList$，即跳表，就是**对链表的一种升级**，这种升级是建立在“以空间换时间”的基础上的。传统单链表只有一个 $next$ 指针，但是在跳表中，有很多个 $next$ 指针，并且将这些指针分层，由此来提高查询效率。
> 注意，跳表额外空间体现在增加了 $next$ 指针，而不是增加了节点的数量，节点个数是不变的，只不过每个节点的 $next$ 指针的数量增加了。

其实这种分层设计的思想可以稍微和树状数组联系一下，下面举个例子：假设有元素 `{1,3,5,6,6,7,9}`
如果是单链表，我们要查找 $7$，那么需要 $6$ 次查询
如果是跳表，假设跳表是如下形式：
```
level[1]: 1 => 3 ->   =>   => 6 ->   -> 9 -> NULL 
level[0]: 1 -> 3 -> 5 -> 6 -> 6 => 7 -> 9 -> NULL
```
从最高层开始查找,`1=>3=>6`，然后跳到下一层`=>7`，只需 $4$ 次
当然，这个例子比较简单，如果元素有上百万个，那个效率的提升就非常明显了！
在树状数组中，我们是以 `x&-x` 来分层的，但是在跳表中，分层实际上不太好办。这主要体现在，每个数据需要有多少个 $next$ 指针。
> 注意：$next$ 指针必须是连续的，也就是说，如果没有第三层 $next$ 指针，也必然没有第四层，第五层 ...

理想情况下，我们希望上一层的 $next$ 指针数是下一层的一半，这样就尽可能的平衡时间和空间。但如果这么做的话，每次修改跳表都需要重新调整跳表的结构，这个时间开销是太大了！
跳表中使用了**随机化**的思想，这基于“原始链表中元素数量足够大，且抽样足够随机”的话，每层元素时均匀的。所以说，每次插入元素的时候，第一层(最底层)一定要插入，然有 $1/p$ 的概率插入第二层，$1/p^2$ 的概率插入第三层 ...，在 $redis/zset$ 中，$p4$ 取 $0.25$。
> 关于随机化的实现，我们可以随机化出一个层数，所以小于等于该层的前驱节点的 $next$ 指针都需要指向新加入的节点。相当于在前 $level$ 层添加了一个新节点，但是新节点只占一份内存。

## 4. crud
`find(T num, Container &pre)`：辅助函数，查找 $num$，并保存其每一层的前驱节点到 $pre$ 当中。因此无论是删除还是添加，都需要使用到要处理节点的每一层的前驱节点(因为要修改他们的 $next$ 指针)。
`add(T num)`：添加元素并修改前驱每一层指针
`delete(T num)`：删除元素并修改每一层指针
`query(T num)`
`count(T num)`: 返回元素个数
<font color=blue>TODO:跳表的随机化访问，修改</font>
> 访问跳表中第 k 个节点，相当于访问初始有序链表中的第 k 个节点，很明显这个操作的时间复杂度是 O(n) 的，并不足够优秀。
跳表的随机访问优化就是对每一个前向指针，再多维护这个前向指针的长度。假设 A 和 B 都是跳表中的节点，其中 A 为跳表的第 a 个节点，B 为跳表的第 b 个节点 (a < b)，且在跳表的某一层中 A 的前向指针指向 B，那么这个前向指针的长度为 b - a。
现在访问跳表中的第 k 个节点，就可以从顶层开始，水平地遍历该层的链表，直到当前节点的位置加上当前节点在该层的前向指针长度大于等于 k，然后移动至下一层。重复这个过程直至到达第一层且无法继续行操作。此时，当前节点就是跳表中第 k 个节点。
这样，就可以快速地访问到跳表的第 k 个元素。可以证明，这个操作的时间复杂度为 O(\log n)。


> 注：下面的版本都是默认升序，带有头节点的链表

## 5. simple V1
`header/v1.h`
1. `level` 固定不可修改
2. 循环实现随机化层数
3. 用智能指针管理内存(但好像会导致性能变差)

## 6. simple V2
`header/v2.h`
与 `V1` 的不同点：
### 6.1 支持层数的动态变化
注意传入 `find()` 的 `vector` 数组 `pre` 的初始化。
不同于`V1`，它的大小为最大高度(而不是当前高度)并且不能初始化为 NULL，必须初始化为 `head`。
这其实是由我们的函数逻辑导致的。
在 `find()` 中，我们的 `for` 循环最大循环到`curlevel`，但是在 `add()` 中，我们可能会随机出一个大于当前层数的层数。
由于我们的 `add()` 依赖于 `find()` 函数初始的 `pre` 数组(添加一个节点需要用到前驱节点)，所以说就要保证即使随机化出来的层数大于当前层数，`pre` 也要有对应的 `node`，并且这个 `node` 要我们自己初始化。
因为在 `find()` 中，对于小于 `curlevel` 的 `pre[i]` 节点，即使我们没有初始化，也会在循环当中完成赋值(因此在 `v1` 的固定 `level` 版本中，我们不需要初始化 `pre`，`find()` 会完成初始化)，但是对于大于 `curlevel` 的 `pre[i]` 我们就无能为力了。
另外，在 `erase()` 函数当中，如果最高层只有 `head`(虚头节点)，那么层数可以减少。

### 6.2 head 的初始化
`head` 作为我们的头节点，他应该是包含所有层的。

## [TODO]7. Application: map by skiplist
<font color=blue>TODO</font>

## 8. Application: TINY-KV-DB by skiplist
`../tiny_KV_DB/*`



