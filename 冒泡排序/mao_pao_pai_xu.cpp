#include <iostream>
using namespace std;

// start表示开始位置
// end表示结束位置，最后一个元素后一个位置，即左闭右开区间
int BubbleSort(int a[], int start, int end) {
	int tmp;
	int i, j;
	int exchange; // 是否发生交换
	for (i = start; i < end - 1; i++) {
		exchange = 0;
		for (j = end - 1; j > i; j--) {
			if (a[j] < a[j - 1]) {
				tmp = a[j - 1];
				a[j - 1] = a[j];
				a[j] = tmp;
				exchange = 1; // 如果这一趟发生了交换，则为true
			}
		}
		if (exchange == 0) { // 如果某一趟没有发生交换，说明无逆序，排序已经完成
			return 0;
		}
	}
	return 0;
}

int main(int argc, char** argv) {
	int a[5] = { 87,12,56,45,78 };
	BubbleSort(a, 0, 5);
	for (int i = 0; i < 5; i++) {
		cout << a[i] << endl;
	}
	system("pause");
	return 0;
}