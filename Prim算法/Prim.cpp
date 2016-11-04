#include <iostream>
#include <stdlib.h>
using namespace std;

typedef int heap_elem_type; // 元素的类型

// 基本类型（如int, long, float, double）的比较函数
int cmp_int(const int *a, const int *b) {
	int sub = *a - *b;
	if (sub > 0)
		return 1;
	else if (sub == 0)
		return 0;
	else return -1;
}

typedef struct Heap_
{
	int size; // 实际元素个数
	int capacity; // 容量
	heap_elem_type *elems; // 堆的数组
	int(*cmp)(const heap_elem_type*, const heap_elem_type*); // 元素的比较函数
}heap_t;

heap_t* heap_init(int capacity, int(*cmp)(const heap_elem_type*, const heap_elem_type*)) {
	heap_t *h = (heap_t *)malloc(sizeof(heap_t));
	h->size = 0;
	h->capacity = capacity;
	// 在创建堆时就分配存储空间
	h->elems = (heap_elem_type*)malloc(capacity * sizeof(heap_elem_type));
	h->cmp = cmp;
	return h;
}

void heap_destroy(heap_t *h) {
	free(h->elems);
	free(h);
}

int heap_size(heap_t *h) {
	return h->size;
}

// 判断堆是否为空
int heap_empty(heap_t *h) {
	return h->size == 0;
}

// 小根堆的自上向下筛选算法
// 堆的结点存储在数组elems[0..n-1]中
void heap_sift_down(heap_t *h, int start) {	// start是开始结点的下标
	int i = start;
	int j;
	int tmp = h->elems[start];
	for (j = 2 * i + 1; j < h->size; j = 2 * j + 1) {
		if ((j < h->size - 1) && (h->cmp(&(h->elems[j]), &(h->elems[j + 1])) > 0))
			j++; // j指向两子女中较小者
		if (h->cmp(&tmp, &(h->elems[j])) <= 0)
			break;
		else {
			h->elems[i] = h->elems[j];
			i = j;
		}
	}
	h->elems[i] = tmp;
}

// 小根堆的自下向上筛选算法
// 堆的结点存储在数组elems[0..n-1]中
void heap_sift_up(heap_t *h, int start) {
	int j = start;
	int i = (j - 1) / 2;
	int tmp = h->elems[start];
	while (j > 0) {
		if (h->cmp(&(h->elems[i]), &tmp) <= 0) // 父结点比子结点小
			break;
		else {
			h->elems[j] = h->elems[i];
			j = i;
			i = (i - 1) / 2;
		}
	}
	h->elems[j] = tmp;
}

// 获取堆顶元素
heap_elem_type heap_top(heap_t *h) {
	return h->elems[0];
}

// 弹出堆顶元素
void heap_pop(heap_t *h) {
	h->elems[0] = h->elems[h->size - 1];
	h->size--;
	heap_sift_down(h, 0);
}

// 添加一个元素
void heap_push(heap_t *h, heap_elem_type x) {
	if (h->size == h->capacity) { // 已满，重新分配内存
		heap_elem_type *tmp = (heap_elem_type *)realloc(h->elems, h->capacity * 2 * sizeof(heap_elem_type));
		h->elems = tmp;
		h->capacity *= 2;
	}
	h->elems[h->size] = x;
	h->size++;
	heap_sift_up(h, h->size - 1);
}

// 堆排序
void dui_pai_xu(int *a, int n, int(*cmp)(const int*, const int*)) {
	int i;
	int tmp;

	// 将数组a[0..n-1]变成一个最小堆
	heap_t *h = heap_init(n, cmp);
	free(h->elems); // 在创建堆时就分配了存储空间，free掉以免造成内存泄漏
	h->elems = a;
	h->size = n; // 手册中没有对h->size赋值
	i = (h->size - 2) / 2; // i指向最后一个结点的父结点
	while (i >= 0) {
		heap_sift_down(h, i);
		i--; // 按层次遍历
	}

	for (i = h->size - 1; i > 0; i--) {
		tmp = h->elems[i];
		h->elems[i] = h->elems[0];
		h->elems[0] = tmp;
		h->size--;
		heap_sift_down(h, 0);
	}

	//heap_destroy(h);
	free(h); // 不能free(h->elems)，因为h->elems = a
}

#define MAX_V 1000
#define MAX_E 1000
#define INF 0x3f3f3f3f
#define NIL -1

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

int cost[MAX_V][MAX_V]; // cost[u][v]表示边(u, v)的权值
// 相应的边不存在时设为INF

int key[MAX_V]; // key[v]是所有将v与树中某一顶点相连的边中的最小权值

int pi[MAX_V]; // pi[v]指树中v的父母

bool used[MAX_V]; // 顶点i是否包含在集合q中

int cmp_key(const int *a, const int *b) {
	int sub = key[*a] - key[*b];
	if (sub > 0)
		return 1;
	else if (sub == 0)
		return 0;
	else return -1;
}

int InitGraph(int total) {
	V = total;

	for (int i = 0; i < total; i++) {
		for (int j = 0; j < total; j++) {
			cin >> cost[i][j];
		}
	}

	return 0;
}

// r是待生成的最小生成树的根
int Prim(int r) {
	for (int i = 0; i < V; i++) {
		key[i] = INF;
		pi[i] = NIL;
	}

	key[r] = 0;

	// 不在树中的所有顶点都放在一个基于key域的最小优先级队列q中
	heap_t *h = heap_init(V, cmp_key);
	for (int i = 0; i < V; i++) {
		heap_push(h, i);
		used[i] = true;
	}

	while (heap_size(h) != 0) {
		int u = heap_top(h);
		heap_pop(h);
		used[u] = false;

		for (int i = 0; i < V; i++) {
			if (cost[u][i] != INF) {
				if (used[i] == true && cost[u][i] < key[i]) {
					pi[i] = u;
					key[i] = cost[u][i];
					
					// 保持最小堆的性质，运行时间的界为O(n)
					int temp = (h->size - 2) / 2; // i指向最后一个结点的父结点
					while (temp >= 0) {
						heap_sift_down(h, temp);
						temp--; // 按层次遍历
					}
				}
			}
		}
	}

	heap_destroy(h);

	return 0;
}

int main() {
	int total = 0;
	cin >> total;
	InitGraph(total);
	Prim(0);
	int sum = 0;
	for (int i = 0; i < V; i++) {
		if (pi[i] != -1 && cost[i][pi[i]] != INF) {
			sum = sum + cost[i][pi[i]];
		}
	}
	cout << sum << endl;
	return 0;
}