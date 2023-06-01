# Implicit free list
## do what? 
通过 implicit free list 模拟 Malloc 和 Free
我们并不是在真正的 heap 上模拟，而是用一个 byte array 来模拟堆。

## block && payload
payload: 有效载荷，也就是用户申请的字节大小
block container payload, header, footer
当我们 malloc 的时候，实际分配的是一个 block
但是传递给我们的是指向 payload 的 pointer

## 画图的建议
``` shell
            | High addr |
            | --------- |
Hgh addr    | --------- |    low addr
            | --------- |   
            | low   addr|
```

## 简写
[8] means idx of 8
&   means and

## 快捷键
shift + option + ⬇️：复制上一行

## function
### round_up(x,c)
返回 x 在 c 字节对齐下的地址，这里的实现很巧妙
通过对 x 上取整即可。


## explanation
### (1)
为什么 heap 的 start_address 是4？
因为 8 字节对齐，第一个 payload 的 address 是 8
那么，它的 header 就是 8-4=4 
因此，heap 的 start_addres == 4

### (2)
在没有 footer 的隐式空闲链表中，我们只能单向遍历（向地址增大的方向），而不能向地址减小的方向遍历
类似 header 的思想，我们添加一个 footer，就可以实现反向遍历
但其实，最后一个节点是不需要 footer 的，为什么呢？
因为，footer 其实是提供给下一个（地址大）block 使用的，使得它可以反向遍历，但是最后一个 block 没有 next，因此它也不需要 footer了，但是我们假设或者想象说这个 footer 仍然存在，但是我们不使用它。
当然，当我们扩大（brk） heap 的空间时，需要记得加上这个 footer，因为此时他有 next block 了。

### (3)
`if(header_vaddr + blocksize == heap_end_vaddr + 1 + 4) `
该语句用来判断，header_vaddr 所在 block 是否是最后一个 block，为啥要 +1 呢？
对于最后一个 block 来说，它的 payload 的最后一个字节的 satrt addr = heap_end_vaddr, end addr = heap_end_vaddr + 1
也就是说，它的 footer 的第一个字节的 start addr = payload 的最后一个字节的结束处，也就是 heap_end_vaddr + 1
因此，footer 的最后一个字节的 start_addr = (heap_end_vaddr + 1) + 4

## other
### align
payload 8 bytes aligned
header and footer 4 bytes aligned

### small endian
``` C++
#include <iostream>
#include <cstring>
#include <algorithm>
#include <stdint.h>

using namespace std;

uint8_t hp[8];

int main()
{
    hp[0] = 0x11;
    hp[1] = 0x22;
    hp[2] = 0xaa;
    hp[3] = 0xbb;
    /*
    now hp like:
    0x00
    0x00
    0x00
    0x00
    0xbb
    0xaa
    0x22
    0x11
    */
    uint32_t val = *(uint32_t*)&hp[0];    
    // because small endian(lower addr store small bit)
    // left is higher bit
    // val should be 0xbbaa2211
    printf("val = 0x%x\n", val);
    return 0;
}
```

# Explicit free list
## compare to implicit
显式空闲链表的效率比隐式空闲链表的效率至少高出 100%
这是因为收到 **AFA** 规则的约束，A的数量一定大于等于F的数量
所以说，显式空闲链表的长度比隐式空闲链表少一半的节点
因此，查找的时间消耗也少了一半
a: allocated
f: free

