#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <set>
using namespace std;

void debug() { printf("DEBUG\n"); }
void debug(int i) { printf("DEBUG: %d\n", i); }

class Graph {
public:
    Graph(int n) {
        if(n <= 0)
            throw std::invalid_argument( "n must be positive" );
        size = n;
        edges = new bool*[size];
        for(int i = 0; i < n; i++) {
            edges[i] = new bool[size];
            for(int j = 0; j < n; j++)
                edges[i][j] = false;
        }
    }

    /* destructor */
    ~Graph() {
        internal_destructor();
    }

    /* copy constructor */
    Graph(const Graph &g) {
        internal_copy(g);
    }

    /* assignment operator */
    Graph& operator=(const Graph &g) {
        if(this == &g) return *this;
        internal_destructor();
        internal_copy(g);
        return *this;
    }

    int get_size() { return size; }

    /* returns true iff edge has been added successfully */
    bool add_edge(int from, int to) {
        if(edges[from][to])
            return false;
        else {
            edges[from][to] = true;
            return true;
        }
    }

    /* returns true iff edge has been removed successfully */
    bool rem_edge(int from, int to) {
        if(edges[from][to]) {
            edges[from][to] = false;
            return true;
        } else
            return false;
    }

    set<int> outgoing_edges(int vertex) {
        if(vertex < 0 || vertex >= size)
            throw std::invalid_argument( "vertex is out of bounds" );
        set<int> og;
        for(int i = 0; i < size; i++)
            if(edges[vertex][i])
                og.insert(i);
        return og;
    }

    bool has_outgoing(int vertex) {
        if(vertex < 0 || vertex >= size)
            throw std::invalid_argument( "vertex is out of bounds" );
        for(int i = 0; i < size; i++)
            if(edges[vertex][i])
                return true;
        return false;
    }

    set<int> incoming_edges(int vertex) {
        if(vertex < 0 || vertex >= size)
            throw std::invalid_argument( "vertex is out of bounds" );
        set<int> ic;
        for(int i = 0; i < size; i++)
            if(edges[i][vertex])
                ic.insert(i);
        return ic;
    }

    bool has_incoming(int vertex) {
        if(vertex < 0 || vertex >= size)
            throw std::invalid_argument( "vertex is out of bounds" );
        for(int i = 0; i < size; i++)
            if(edges[i][vertex])
                return true;
        return false;
    }

private:
    int size;
    bool **edges;

    void internal_destructor() {
        for(int i = 0; i < size; i++) {
            delete[] edges[i];
        }
        delete[] edges;
    }

    void internal_copy(const Graph &g) {
        size = g.size;
        edges = new bool*[size];
        for(int i = 0; i < size; i++) {
            edges[i] = new bool[size];
            for(int j = 0; j < size; j++) {
                edges[i][j] = g.edges[i][j];
            }
        }
    }
};

void top_sort_school(Graph &g, int k) {
    // noedge - vtx has no incoming edges
    // nonoedge - vtx has no incoming or outgoing edges
    // we want to prioritize taking courses that have prereqs.
    queue<int> noedge, nonoedge, tmpnoedge, tmpnonoedge;
    vector<int> v;
    int l, m, acc; acc = 0;
    set<int> nbrs;

    for(int i = 0; i < g.get_size(); i++) {
        if(!g.has_incoming(i)) {
            if(!g.has_outgoing(i)) nonoedge.push(i);
            else noedge.push(i);
        }
    }

    while(!noedge.empty() || !nonoedge.empty()) {
        acc++;
        m = min(k, (int) (noedge.size() + nonoedge.size()));
        //cout << "stack sz: " << m << endl;
        for(int i = 0; i < m; i++) {
            if(!noedge.empty()) {
                l = noedge.front(); noedge.pop();
            } else {
                l = nonoedge.front(); nonoedge.pop();
            }
            //cout << acc << ": " << l << endl;

            // remove all edges outgoing from k
            for(int j : g.outgoing_edges(l)) {
                g.rem_edge(l, j);
                if(!g.has_incoming(j)) {
                    if(!g.has_outgoing(j)) tmpnonoedge.push(j);
                    else tmpnoedge.push(j);
                }
            }
        }
        while(!tmpnoedge.empty()) {
            noedge.push(tmpnoedge.front()); tmpnoedge.pop();
        }
        while(!tmpnonoedge.empty()) {
            nonoedge.push(tmpnonoedge.front()); tmpnonoedge.pop();
        }
    }

    cout << acc << endl;
}

/*
Solution using two queues instead of stacks that still does not work

g++ -std=c++11 g-graduation.cpp -o k

4 2
3 3 4 0

3 3
0 1 2

10 3
9 9 9 9 10 10 10 10 11 11 0

6 3
2 0 2 2 0 0

9 3
2 0 2 2 0 0 6 6 6
*/
int main() {
    int n, k, a;
    cin >> n >> k;
    Graph g = Graph(n);
    for(int i = 0; i < n; i++) {
        cin >> a;
        if(a)
            g.add_edge(i, a - 1);
    }

    top_sort_school(g, k);
    return 0;
}
