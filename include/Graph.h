//
// Created by lizhi on 4/22/2024.
//

#ifndef LEXICAL_ANALYSIS_GRAPH_H
#define LEXICAL_ANALYSIS_GRAPH_H

#include "vector"

struct Edge {
    int from, to;  // 起点, 终点
    char cond;     // 条件
    Edge() {}

    Edge(int f, char c, int t) : from(f), cond(c), to(t) {}  // 构造函数
};

class Graph {  // nfa的一个子图, 包含起始状态, 最终状态, 还有一系列边集合
private:
    static int state;
    int start, end;
    std::vector<Edge> edges;
public:
    Graph() : start(-1), end(-1) {}  // 构造函数
    Graph(char c);

    int get_start() { return start; }  // 获取起始状态
    int get_end() { return end; }      // 获取终止状态

    void set_start(int s) { start = s; }  // 设置起始状态
    void set_end(int e) { end = e; }      // 设置终止状态

    void add_edge(int from, char cond, int to) {  // 添加边
        if (from == -1 || to == -1) return;
        // 判断是否已经存在这条边
        for (Edge edge : edges) {
            if (edge.from == from && edge.cond == cond && edge.to == to) {
                return;
            }
        }
        edges.emplace_back(from, cond, to);
    }

    std::vector<Edge> get_edges() { return edges; }  // 获取边集合

    static Graph concat(Graph &g1, Graph &g2);  // 连接两个子图
    static Graph union_(Graph &g1, Graph &g2);  // 并操作
    static Graph closure(Graph &g);             // 闭包操作
};

#endif //LEXICAL_ANALYSIS_GRAPH_H
