#include <iostream>
#include <stdlib.h>
using namespace std;

typedef int heap_elem_type; // 元素的类型

// 基本类型（如int, long, float, double）的比较函数
int cmp_int(const int *a, const int *b)
{
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

heap_t* heap_init(int capacity, int(*cmp)(const heap_elem_type*, const heap_elem_type*))
{
	heap_t *h = (heap_t *)malloc(sizeof(heap_t));
	h->size = 0;
	h->capacity = capacity;
	// 在创建堆时就分配存储空间
	h->elems = (heap_elem_type*)malloc(capacity * sizeof(heap_elem_type));
	h->cmp = cmp;
	return h;
}

void heap_destroy(heap_t *h)
{
	free(h->elems);
	free(h);
}

int heap_size(heap_t *h)
{
	return h->size;
}

// 判断堆是否为空
bool heap_empty(heap_t *h)
{
	return h->size == 0;
}

// 小根堆的自上向下筛选算法
// 堆的结点存储在数组elems[0..n-1]中
// start是开始结点的下标
void heap_sift_down(heap_t *h, int start)
{
	int i = start;
	int j;
	int tmp = h->elems[start];
	for (j = 2 * i + 1; j < h->size; j = 2 * j + 1) {
		if ((j < h->size - 1) && (h->cmp(&(h->elems[j]), &(h->elems[j + 1])) > 0))
			j++; // j指向两子女中较小者
		if (h->cmp(&tmp, &(h->elems[j])) <= 0)
			break;
		else
		{
			h->elems[i] = h->elems[j];
			i = j;
		}
	}
	h->elems[i] = tmp;
}

// 小根堆的自下向上筛选算法
// 堆的结点存储在数组elems[0..n-1]中
void heap_sift_up(heap_t *h, int start)
{
	int j = start;
	int i = (j - 1) / 2;
	int tmp = h->elems[start];
	while (j > 0)
	{
		if (h->cmp(&(h->elems[i]), &tmp) <= 0) // 父结点比子结点小
			break;
		else
		{
			h->elems[j] = h->elems[i];
			j = i;
			i = (i - 1) / 2;
		}
	}
	h->elems[j] = tmp;
}

// 获取堆顶元素
heap_elem_type heap_top(heap_t *h)
{
	return h->elems[0];
}

// 弹出堆顶元素
void heap_pop(heap_t *h)
{
	h->elems[0] = h->elems[h->size - 1];
	h->size--;
	heap_sift_down(h, 0);
}

// 添加一个元素
void heap_push(heap_t *h, heap_elem_type x)
{
	if (h->size == h->capacity)
	{	// 已满，重新分配内存
		heap_elem_type *tmp = (heap_elem_type *)realloc(h->elems, h->capacity * 2 * sizeof(heap_elem_type));
		h->elems = tmp;
		h->capacity *= 2;
	}
	h->elems[h->size] = x;
	h->size++;
	heap_sift_up(h, h->size - 1);
}

// 堆排序
void dui_pai_xu(int *a, int n, int(*cmp)(const int*, const int*))
{
	int i;
	int tmp;

	// 将数组a[0..n-1]变成一个最小堆
	heap_t *h = heap_init(n, cmp);
	free(h->elems); // 在创建堆时就分配了存储空间，free掉以免造成内存泄漏
	h->elems = a;
	h->size = n;
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

int main()
{
	int *array = (int *)malloc(10 * sizeof(int));
	array[0] = 50;
	array[1] = 25;
	array[2] = 76;
	array[3] = 37;
	array[4] = 62;
	array[5] = 84;
	array[6] = 31;
	array[7] = 43;
	array[8] = 55;
	array[9] = 92;

	dui_pai_xu(array, 10, cmp_int);

	for (int i = 0; i < 10; i++)
		cout << array[i] << endl;
	system("pause");
}