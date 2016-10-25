#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

#define MAX_N 100

int p[MAX_N]; // p[x]表示x的父结点
int rank_[MAX_N]; // 用秩表示结点x高度的一个上界

void makeSet(int x) {
	p[x] = x; // 每棵树的根是它自己的父结点
	rank_[x] = 0; // 树中唯一结点的初始秩为0
}

// 查询树的根
int findSet(int x) {
	// 路径压缩使查找路径上的每个结点都直接指向根节点
	// 路径压缩不改变结点的秩
	if (x != p[x])
		p[x] = findSet(p[x]);
	return p[x];
}

// 合并x和y所属的集合，按秩合并
void unionSet(int x, int y) {
	x = findSet(x);
	y = findSet(y);

	if (x == y) {
		return;
	}

	if (rank_[x] < rank_[y]) {
		// 具有较高秩的根成为具有较低秩的根的父结点
		p[x] = y;
	}
	else if (rank_[x] > rank_[y]) {
		p[y] = x;
	}
	else {
		p[y] = x; // 任选一个根作为父结点，并增加其秩的值
		rank_[x]++; // 注意增加x的高度而不是y
	}
}

#define MAX_E 1000

int V; // 图中顶点数
int E; // 图中边数

// 从顶点u指向顶点v的权值为cost的边
struct edge
{
	int u;
	int v;
	int cost;
};

edge es[MAX_E]; // 边

vector<edge> A; // 集合A是一个森林，是某个最小生成树的一个子集
// 加入集合A中的安全边总是图中连接两个不同连通分支的最小权边

bool cmp(const edge& e1, const edge& e2) {
	return e1.cost < e2.cost;
}

int InitGraph() {
	V = 9;
	E = 13;
	es[0] = { 0, 1, 4 };
	es[1] = { 1, 2, 8 };
	es[2] = { 2, 3, 7 };
	es[3] = { 3, 4, 9 };
	es[4] = { 4, 5, 10 };
	es[5] = { 5, 6, 2 };
	es[6] = { 6, 7, 1 };
	es[7] = { 7, 0, 8 };
	es[8] = { 7, 8, 7 };
	es[9] = { 6, 8, 6 };
	es[10] = { 8, 2, 2 };
	es[11] = { 7, 1, 11 };
	es[12] = { 2, 5, 4 };
	return 0;
}

int kruskal() {
	A.clear(); // 将集合A初始化为空集

	// 建立|V|棵树，每棵树都包含了图的一个顶点
	for (int i = 0; i < V; i++) {
		makeSet(i);
	}

	// 根据权值的非递减顺序，对es中的边进行排序
	sort(es, es + E, cmp);

	for (int i = 0; i < E; i++) {
		// 检查每条边的端点u和v是否属于同一棵树，如果是就会形成一个回路
		if (findSet(es[i].u) != findSet(es[i].v)) { // 两个顶点分属于不同的树
			A.push_back(es[i]); // 把边加入集合A中
			unionSet(es[i].u, es[i].v); // 对两棵树中的顶点进行合并
		}
	}
	return 0;
}

int main() {
	InitGraph();
	kruskal();
	cout << "打印某一最小生成树的边：" << endl;
	for (int i = 0; i < A.size(); i++) {
		cout << "连接顶点" << A[i].u << "和顶点" << A[i].v << "的边" << endl;
	}
	system("pause");
	return 0;
}