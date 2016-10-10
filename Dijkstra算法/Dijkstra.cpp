#include <iostream>
#include <limits>
#include <queue>
#include <vector>
using namespace std;

#define MAX_V 1000
#define MAX_E 1000
#define NIL -1

int V; // 图中顶点数
int E; // 图中边数

// 从顶点from指向顶点to的权值为cost的边
struct edge {
	int from;
	int to;
	int cost;
};

edge es[MAX_E]; // 边

int Pi[MAX_V]; // 存储u的父母
// 如果u没有父母（例如u=s或u尚未被发现），则Pi[u]=NIL

int d[MAX_V]; // 存储源顶点s和顶点u之间的距离

struct cmp {
	// 优先级队列按升序出队，即小根堆

	bool operator()(int x, int y) {
		// 排序关键字为顶点的d值
		return d[x] > d[y];
	}
};

priority_queue<int, deque<int>, cmp> q; // 顶点的最小优先队列

vector<int> S; // 顶点集合S
// 若顶点u加入到集合S中，从源点s到u的最终最短路径的权值均已确定

int InitGraph() {
	V = 5;
	E = 10;
	es[0] = { 0, 1, 10 };
	es[1] = { 0, 3, 5 };
	es[2] = { 1, 2, 1 };
	es[3] = { 1, 3, 2 };
	es[4] = { 2, 4, 4 };
	es[5] = { 3, 1, 3 };
	es[6] = { 3, 2, 9 };
	es[7] = { 3, 4, 2 };
	es[8] = { 4, 0, 7 };
	es[9] = { 4, 2, 6 };
	return 0;
}

// 松弛
void relax(int u, int v, int w) {
	// w = w(u, v)
	if (d[v] > (d[u] + w)) {
		// 优先级队列的底层是用heap来实现的
		// 经过修改d[v]之后的heap，不再是合法的heap
		queue<int> temp;
		while (q.size() != 0) {
			temp.push(q.top());
			q.pop();
		}

		d[v] = d[u] + w;

		// 重新再做一个heap
		while (temp.size() != 0) {
			q.push(temp.front());
			temp.pop();
		}

		Pi[v] = u;
	}
}

int Dijkstra(int s) {
	// 初始化每个顶点
	for (int i = 0; i < V; i++) {
		d[i] = INT_MAX;
		Pi[i] = NIL;
	}
	d[s] = 0;

	for (int i = 0; i < V; i++) {
		q.push(i); // 初始化最小优先队列
	}

	S.clear(); // 将集合S初始化为空集

	while (q.size() != 0) {
		int u = q.top(); // 顶点u具有最小的最短路径估计
		q.pop(); // 将顶点u从Q=V-S中删除，并加入到集合S中
		S.push_back(u);

		for (int i = 0; i < E; i++) {
			if (es[i].from == u) {
				// 对以u为起点的每条边(u, v)进行松弛
				relax(es[i].from, es[i].to, es[i].cost);
			}
		}
	}

	return 0;
}

// 输出从s到v的最短路径上的所有顶点
void PrintPath(int s, int v) {
	if (v == s) {
		cout << s << endl;
	}
	else if (Pi[v] == NIL) {
		cout << "No path from " << s << " to " << v << " exists." << endl;
	}
	else {
		PrintPath(s, Pi[v]);
		cout << v << endl;
	}
}

int main(int argc, char** argv) {
	InitGraph();
	Dijkstra(0);
	PrintPath(0, 4);
	system("pause");
	return 0;
}