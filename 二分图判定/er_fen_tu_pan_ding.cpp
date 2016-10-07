#include <iostream>
#include <vector>
using namespace std;

#define MAX_V 1000
vector<int> G[MAX_V]; // 图
int V; // 顶点数
int color[MAX_V]; // 顶点i的颜色

int InitGraph(){
	V = 4;
	G[0].push_back(1);
	G[0].push_back(3);
	G[1].push_back(0);
	G[1].push_back(2);
	G[2].push_back(1);
	G[2].push_back(3);
	G[3].push_back(0);
	G[3].push_back(2);
	return 0;
}

// 递归算法用栈保存每层的状态
// 把顶点染成1或-1
bool dfs(int v, int c){
	color[v] = c; // 把顶点v染成颜色c
	for (int i = 0; i < G[v].size(); i++){
		// 如果相邻的顶点同色，则返回false
		if (color[G[v][i]] == c){
			return false;
		}

		// 如果相邻的顶点还没被染色，则染成-c
		if (color[G[v][i]] == 0 && !dfs(G[v][i], -c)){
			return false;
		}
	}
	// 如果所有顶点都染过色了，则返回true
	return true;
}

void solve(){
	for (int i = 0; i < V; i++){
		if (color[i] == 0){
			// 如果顶点i还没被染色，则染成1
			if (!dfs(i, 1)){
				cout << "No" << endl;
				return;
			}
		}
	}
	cout << "Yes" << endl;
}

int main(int argc, char** argv) {
	InitGraph();
	solve();
	return 0;
}