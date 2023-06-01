# LRU 
## about
1. LRU_CACHE 
2. LRU_K 
3. LRU_PAGE_SWAP

## test
[LC156-LRU_Cache](https://leetcode.cn/problems/lru-cache/)

## shortcoming
实际上，我的设计貌似效率有些低下？
在 LC 上测试竟然 TLE。。。

## TODO
手写哈希映射(map)，通过跳表

## 逆天
我把哈希表和哈希映射搞混了！
哈希表是 unordered_set
哈希映射是 map/unordered_map
哈希表可以用 Array+List 的方式实现
哈希映射可以用 SkipList 实现，在 C++ 中，是用 Red-Black-Tree 实现的