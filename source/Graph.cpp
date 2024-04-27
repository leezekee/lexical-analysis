//
// Created by lizhi on 4/22/2024.
//

#include "Graph.h"
#include "common.h"

int Graph::state = 0;

Graph::Graph(char c) {
    start = state++;
    end = state++;
    add_edge(start, c, end);
}

Graph Graph::concat(Graph &g1, Graph &g2) {
    Graph g = Graph();
    g.set_start(g1.get_start());
    g.set_end(g2.get_end());
    for (Edge edge : g1.get_edges()) {
        g.add_edge(edge.from, edge.cond, edge.to);
    }
    for (Edge edge : g2.get_edges()) {
        g.add_edge(edge.from, edge.cond, edge.to);
    }
    g.add_edge(g1.get_end(), EPSILON, g2.get_start());
    return g;
}

Graph Graph::closure(Graph &g) {
    Graph new_g = Graph();
    new_g.set_start(g.get_start());
    new_g.set_end(g.get_end());
    for (Edge edge : g.get_edges()) {
        new_g.add_edge(edge.from, edge.cond, edge.to);
    }
    new_g.add_edge(new_g.get_start(), EPSILON, new_g.get_end());
    new_g.add_edge(new_g.get_end(), EPSILON, new_g.get_start());
    return new_g;
}

Graph Graph::union_(Graph &g1, Graph &g2) {
    Graph g = Graph();
    int start = state++;
    int end = state++;
    g.set_start(start);
    g.set_end(end);
    g.add_edge(start, EPSILON, g1.get_start());
    g.add_edge(start, EPSILON, g2.get_start());
    g.add_edge(g1.get_end(), EPSILON, end);
    g.add_edge(g2.get_end(), EPSILON, end);
    for (Edge edge : g1.get_edges()) {
        g.add_edge(edge.from, edge.cond, edge.to);
    }
    for (Edge edge : g2.get_edges()) {
        g.add_edge(edge.from, edge.cond, edge.to);
    }
    return g;
}
