#include <iostream>
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

int main() {
	makeSet(1);
	makeSet(2);
	makeSet(3);
	makeSet(4);
	makeSet(5);
	unionSet(1, 2);
	unionSet(3, 4);
	unionSet(3, 5);
	unionSet(1, 3);
	for (int i = 1; i <= 5; i++) {
		cout << findSet(i) << " ";
	}
	cout << endl;
	system("pause");
	return 0;
}