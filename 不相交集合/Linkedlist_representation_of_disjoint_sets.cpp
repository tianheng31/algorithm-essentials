#include <iostream>
#include <vector>
using namespace std;

#define list_size(list) ((list)->size)
#define list_head(list) ((list)->head)
#define list_tail(list) ((list)->tail)
#define list_is_head(list, element) ((element) == (list)->head ? 1 : 0)
#define list_is_tail(element) ((element)->next == NULL ? 1 : 0)
#define list_data(element) ((element)->data)
#define list_next(element) ((element)->next)

typedef struct ListElmt_ {
	void *data;
	struct ListElmt_ *representative; // 指向表中第一个对象（即代表）的指针
	struct ListElmt_ *next; // 指向表中下一对象的指针
}ListElmt;

typedef struct List_ {
	int size;
	void(*destroy)(void *data);
	ListElmt *head;
	ListElmt *tail;
}List;

void list_init(List *list, void(*destroy)(void *data)) {	// 初始化链表
	list->size = 0;
	list->destroy = destroy;
	list->head = NULL;
	list->tail = NULL;
}

int list_ins_next(List *list, ListElmt *element, const void *data) {
	// 在list指定的链表中element后面插入一个新元素
	ListElmt *new_element;
	if ((new_element = (ListElmt *)malloc(sizeof(ListElmt))) == NULL)
		return -1;
	new_element->data = (void *)data; // 强制类型转换

	// 如果element设置为NULL，则新元素插入链表头部
	if (element == NULL) {
		if (list_size(list) == 0)
			list->tail = new_element;

		new_element->next = list->head;
		list->head = new_element;

		/* 保证每个表中的第一个对象是所在集合的代表 */
		ListElmt *ptr = list->head;
		while (ptr != NULL)
		{
			ptr->representative = list->head;
			ptr = ptr->next;
		}
	}
	else {
		// 当插入的元素位于链表末尾时，更新tail使其指向新的结点
		if (element->next == NULL)
			list->tail = new_element;

		new_element->next = element->next;
		element->next = new_element;

		/* 新元素的代表指针指向链表中的第一个对象 */
		new_element->representative = list->head;
	}

	list->size++;

	return 0;
}

// 调用返回后，data指向已移除元素中存储的数据
// 由调用者负责管理data所引用的存储空间
int list_rem_next(List *list, ListElmt *element, void **data) {
	// 从链表中移除由element所指定的元素之后的那个结点
	ListElmt *old_element;
	if (list_size(list) == 0)
		return -1;
	// 链表的头结点需要移除
	if (element == NULL) {
		*data = list->head->data;
		old_element = list->head;
		list->head = list->head->next;
		// 移除操作使得整个链表成为空链表
		if (list_size(list) == 1)			list->tail = NULL;

		/* 保证每个表中的第一个对象是所在集合的代表 */
		ListElmt *ptr = list->head;
		while (ptr != NULL)
		{
			ptr->representative = list->head;
			ptr = ptr->next;
		}
	}
	// 移除其余位置上的结点
	else {
		if (element->next == NULL)
			return -1;
		*data = element->next->data;
		old_element = element->next;
		element->next = element->next->next;
		// 移除的目标结点是链表的尾结点
		if (element->next == NULL)
			list->tail = element;
	}
	free(old_element);
	list->size--;
	return 0;
}

void list_destroy(List *list) {
	void  *data;
	while (list_size(list) > 0) {
		if (list_rem_next(list, NULL, (void **)&data) == 0 && list->destroy != NULL) {
			// 如果调用list_init时destroy参数不为NULL
			// 则移除链表中每个元素时都调用该函数一次
			list->destroy(data);
		}
	}
	memset(list, 0, sizeof(List));
}

vector<List *> DisjointSet; // collection of disjoint dynamic sets

// 建立一个新的集合，其唯一成员（因而其代表）就是x
List *makeSet(void *x) {
	List *new_list = NULL;
	new_list = (List *)malloc(sizeof(List));
	list_init(new_list, NULL); // 这里有坑
	list_ins_next(new_list, NULL, x);
	DisjointSet.push_back(new_list); // adds to collection of sets
	return new_list;
}

// 返回一个指针，指向包含x的（唯一）集合的代表
template <typename DataType>
ListElmt *findSet(DataType *x) {
	for (int i = 0; i < DisjointSet.size(); i++) {
		ListElmt *ptr = DisjointSet[i]->head;
		while (ptr != NULL)
		{
			if (*(DataType *)(ptr->data) == *x) {
				return ptr->representative;
			}
			else {
				ptr = ptr->next;
			}
		}
	}

	return NULL;
}

// 将包含x和y的动态集合，合并为一个新的集合
template <typename DataType>
List *unionSet(DataType *x, DataType *y) {
	ListElmt *rep_a = findSet(x);
	ListElmt *rep_b = findSet(y);
	List *a = NULL;
	List *b = NULL;
	vector<List *>::iterator it;
	for (it = DisjointSet.begin(); it != DisjointSet.end(); it++) {
		if ((*it)->head != NULL && (*it)->head->representative == rep_a) {
			a = *it;
		}
		if ((*it)->head != NULL && (*it)->head->representative == rep_b) {
			b = *it;
		}
	}

	if (a->size > b->size) {
		swap(a, b); // 保持链表a的长度比b短
	}

	// 将链表a拼接到b的表尾
	b->tail->next = a->head;
	b->tail = a->tail;
	// 对于链表a中的每一个对象，都需更新其指向代表的指针
	ListElmt *ptr = a->head;
	while (ptr != NULL) {
		ptr->representative = b->head->representative;
		ptr = ptr->next;
	}

	for (it = DisjointSet.begin(); it != DisjointSet.end(); ) {
		if ((*it) == a)
			it = DisjointSet.erase(it);
		else
			it++;
	}

	free(a);

	return b;
}

int destroySet() {
	for (int i = 0; i < DisjointSet.size(); i++) {
		List *ptr = DisjointSet[i];
		list_destroy(ptr);
	}
	return 0;
}

template <typename DataType>
void printSet() {
	cout << "开始打印！" << endl;
	for (int i = 0; i < DisjointSet.size(); i++) {
		ListElmt *ptr = DisjointSet[i]->head;
		cout << "正在打印代表为" << *(DataType *)ptr->representative->data << "的集合：";
		while (ptr != NULL)
		{
			cout << *(DataType *)ptr->data << " ";
			ptr = ptr->next;
		}
		cout << endl;
	}
	cout << "打印结束！" << endl << endl;
}

int main() {
	int a = 1;
	makeSet((void *)&a);
	printSet<int>();
	int b = 2;
	makeSet((void *)&b);
	printSet<int>();
	int c = 3;
	makeSet((void *)&c);
	printSet<int>();
	int d = 4;
	makeSet((void *)&d);
	printSet<int>();

	unionSet<int>(&a, &b);
	printSet<int>();
	unionSet<int>(&c, &d);
	printSet<int>();
	unionSet<int>(&a, &c);
	printSet<int>();

	destroySet();

	system("pause");
	return 0;
}