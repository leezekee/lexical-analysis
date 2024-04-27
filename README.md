# 词法分析

## 目录结构

* `bin`: 编译后的可执行文件
* `src`: 源代码
* `example`: 示例代码
* `lib`: 依赖库及编译后的库
* `include`: 头文件
* `test`: 测试代码

## 设计

### Regex

输入表达式，添加`&`符号，再转为后缀表达式

成员变量有三个：

* `raw_expr`: 输入表达式
* `suffix_expr`: 后缀表达式
* `expr`: 添加`&`符号后的表达式

### NFA

NFA由后缀表达式构造而来，跳转表用map<int, map<char, vector<int>>>表示，其中第一个int表示状态，第二个char表示输入字符，vector<int>表示跳转到的状态

### DFA

DFA结构类似于NFA，跳转表用map<int, map<char, int>>表示，其中第一个int表示状态，第二个char表示输入字符，int表示跳转到的状态

### Minimized DFA

最小化DFA，用于优化DFA，继承自DFA

## 说明

* NFA、DFA、MinimizedDFA均有`to_graph`方法，用于生成Graphviz的dot文件，可以通过`dot -Tpng -o output.png input.dot`生成图片, graphviz在`bin`目录下
* 对于NFA生成DFA方法，速度较慢，对于较大的NFA可能会有性能问题，初步分析为NFA状态较多，导致Closure闭包计算过多，其中存在大量重复计算，优化思路为采用时间复杂度为O(n)的方法，一次性计算所有节点的闭包，采用带备忘录的递归算法，即可达到化简计算的效果。

## 编译

```shell
mkdir build
cd build
cmake ..
```
## 运行

找到编译后的可执行文件，运行即可，直接运行bin下的可执行文件也可
