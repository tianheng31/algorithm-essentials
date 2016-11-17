#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <string>
using namespace std;
// 最近公共祖先LCA_基于二分搜索的算法（倍增LCA）
// 采用图（树）的邻接表表示
#define MAX_V 200000	// 图中顶点数量的最大值
#define MAX_LOG_V 17	// MAX_LOG_V = floor[log2(MAX_V)]
vector<int> G[MAX_V];
int rootID;	// 树中根节点的编号
int parentID[MAX_LOG_V][MAX_V];	// 顶点v向上走2^k步所到的顶点
int depth[MAX_V]; // 节点的深度

map<string, int> name2id;
map<int, string> id2name;

int cnt = 0;

int InitTree() {
	rootID = 0;

	int total = 0;

	cin >> total;

	for (int i = 0; i < total; i++) {
		string father, son;
		cin >> father >> son;

		if (name2id.find(father) == name2id.end()) {
			name2id[father] = cnt;
			id2name[cnt] = father;
			cnt++;
		}
		if (name2id.find(son) == name2id.end()) {
			name2id[son] = cnt;
			id2name[cnt] = son;
			cnt++;
		}

		G[name2id[father]].push_back(name2id[son]);
		G[name2id[son]].push_back(name2id[father]);
	}

	return 0;
}

void dfs(int v, int p, int d) {
	// 通过深度优先遍历初始化
	parentID[0][v] = p;
	depth[v] = d;

	for (int i = 0; i < G[v].size(); i++) {
		if (G[v][i] != p)
			dfs(G[v][i], v, d + 1);
	}
}

void init(int V) {
	// V表示树中节点的总数
	// 预处理出任意顶点向上走一步所到的顶点和depth
	dfs(rootID, -1, 0);

	// 预处理出parentID[k]，k<=floor(MAX_LOG_V)，即向下取整
	// 以自底向上的方式求最优解，并充分利用重叠子问题的结果
	for (int k = 0; k + 1 < MAX_LOG_V; k++)
		for (int v = 0; v < V; v++) {
			if (parentID[k][v] < 0) {
				parentID[k + 1][v] = -1;
			}
			else {
				// parent2[v]=parent[parent[v]]，得到向上走两步所到的顶点
				parentID[k + 1][v] = parentID[k][(parentID[k][v])];
			}
		}
}

int LCA(int u, int v) {
	// 计算u和v的LCA（最近公共祖先）
	// 保证u的深度小于v
	if (depth[u] > depth[v]) swap(u, v);
	// 让u和v向上走到同一深度
	int log_v = (int)(log(depth[v] - depth[rootID]) / log(2));
	for (int i = log_v; i >= 0; i--) {
		if (depth[v] - (1 << i) >= depth[u]) {	// depth[v]-2^i >= depth[u]
			v = parentID[i][v];	// 向上移动到depth[v]+2^i
		}
	}

	if (u == v)	return u;

	// 利用二分搜索计算LCA
	for (int k = log_v; k >= 0; k--) {
		if (parentID[k][u] != parentID[k][v]) {
			u = parentID[k][u];
			v = parentID[k][v];
		}
	}
	return parentID[0][u];

	// 右移一位，偶数变为原来的一半，奇数变为原数减1的一半
}

int main(int argc, char** argv) {
	InitTree();
	init(cnt);
	int total = 0;
	cin >> total;
	for (int i = 0; i < total; i++) {
		string a, b;
		cin >> a >> b;
		cout << id2name[LCA(name2id[a], name2id[b])] << endl;
	}
	return 0;
}