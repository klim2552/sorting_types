// sorting_types.cpp: определяет точку входа для консольного приложения.
//
#include "stdafx.h"
#include <iostream>
#include <ctime>
#include<cmath>
#include "windows.h"
#include <stack>
#include <iomanip>

using namespace std;

const int cycles = 100;// повышает точность вычислений времени работы функции.

class result //класс, описывающий результаты работы каждой сортировки
{
public:
	char* sortname;//название сортировки
	float speed[4];//время работы
	int countchanges[4];//число обменов
	int countcompare[4];//число сравнений
	int size[4];//размер сортированного массива
	bool check[4];// результат проверки работы сортировки
};

void innit(int arr[], int n)//Заполняет динамический массив размером N случайными числами.
{
	for (int i = 0; i < n; i++)
	{
		arr[i] = rand() % 100;
	}
}

bool checksort(int arr[], int n)//Проверяет правильность сортировки массива.
{
	for (int i = 0; i < n - 1; i++)
	{
		if (arr[i] > arr[i + 1])
		{
			return false;
		}
	}
	return true;
}

void out(int arr[], int n)
{
	for (int i = 0; i < n; i++)
		cout << arr[i] << endl;
}

void qsort(int arr[], int l, int r, int &comcount, int &swapcount)//Быстрая сортировка, необходимая для анализа рекурсивного метода быстрой сортировки.
{
	int key = arr[l + (r - l) / 2];
	int i = l;
	int j = r;
	while (i <= j)
	{
		comcount++;
		while (arr[i] < key)
		{
			i++;
			comcount++;
		}
		while (arr[j] > key)
		{
			j--;
			comcount++;
		}
		if (i <= j)
		{
			swap(arr[i], arr[j]);
			i++;
			j--;
			swapcount++;
			comcount++;
		}
	}
	if (i < r)qsort(arr, i, r, comcount, swapcount);
	if (l < j)qsort(arr, l, j, comcount, swapcount);
}

void analyseQuickSort_unrecursion(result *res, int n)//Анализ быстрой сортировки. Безрекурсивная реализация(со стеком)
{
	res[7].sortname = "QuickSortStack";
	int holy = log10(n) - 1;
	res[7].size[holy] = n;
	int *arr = new int[n];
	float t0, t1;
	t0 = GetTickCount();
	int comcount, swapcount;
	for (int k = 0; k < cycles; k++)
	{
		innit(arr, n);
		stack <int> buf;
		buf.push(n - 1);
		buf.push(0);
		int key, left, right, lcount, rcount;
		key = left = right = lcount = rcount = 0;
		comcount = 0;
		swapcount = 0;
		do {
			comcount++;
			left = buf.top();
			buf.pop();
			right = buf.top();
			buf.pop();
			{
				key = arr[(left + right) / 2];
				lcount = left;
				rcount = right;
				while (lcount < rcount)
				{
					comcount++;
					while (arr[lcount] < key)
					{
						lcount++;
						comcount++;
					}
					while (arr[rcount] > key)
					{
						comcount++;
						rcount--;
					}
					comcount++;
					if (lcount <= rcount)
					{
						swap(arr[lcount], arr[rcount]);
						swapcount++;
						lcount++;
						rcount--;
					}
				}
			}
			if (left < rcount)
			{
				buf.push(rcount);
				buf.push(left);
			}
			if (right > lcount)
			{
				buf.push(right);
				buf.push(lcount);
			}
		} while (buf.size() != NULL);
	}
	t1 = GetTickCount();
	res[7].check[holy] = checksort(arr, n);
	res[7].speed[holy] = (t1 - t0) / cycles;
	res[7].countchanges[holy] = swapcount;
	res[7].countcompare[holy] = comcount;
	delete[]arr;
	//out(arr, n);
}

void analyseQuickSort_recursion(result *res, int n)//Анализ быстрой сортировки. Рекурсивная реализация
{
	res[6].sortname = "QuickSortRecursion";
	int holy = log10(n) - 1;
	res[6].size[holy] = n;
	int *arr = new int[n];
	float t0, t1;
	t0 = GetTickCount();
	int comcount, swapcount;
	//innit(arr, n);
	//out(arr, n);
	//cout << "halo suka" << endl;
	for (int k = 0; k < cycles; k++)
	{
		innit(arr, n);
		comcount = 0;
		swapcount = 0;
		qsort(arr, 0, n - 1, comcount, swapcount);
	}
	t1 = GetTickCount();
	res[6].check[holy] = checksort(arr, n);
	res[6].speed[holy] = (t1 - t0) / cycles;
	res[6].countchanges[holy] = swapcount;
	res[6].countcompare[holy] = comcount;
	//out(arr, n);
	delete[]arr;
}

void analyseMergeSort(result *res, int n)//Анализ сортировки слиянием. Нерекурсивная реализация
{
	res[5].sortname = "MergeSort";
	int holy = log10(n) - 1;
	res[5].size[holy] = n;
	int *arr = new int[n];
	float t0, t1;
	t0 = GetTickCount();
	int comcount, swapcount;
	for (int k = 0; k < cycles; k++)
	{
		comcount = 0;
		swapcount = 0;
		innit(arr, n);
		for (int bsi = 1; bsi < n; bsi *= 2)
		{
			for (int bi = 0; bi < n - bsi; bi += 2 * bsi)
			{
				comcount += 6;
				int lbi = 0;
				int rbi = 0;
				int lb = bi;
				int mb = bi + bsi;
				int rb = bi + 2 * bsi;
				rb = (rb < n) ? rb : n;
				int* buffer = new int[rb - lb];
				while ((lb + lbi) < mb&&rb > (rbi + mb))
				{
					comcount += 3;
					if (arr[lb + lbi] < arr[mb + rbi])
					{
						buffer[lbi + rbi] = arr[lb + lbi];
						lbi++;
					}
					else
					{
						buffer[lbi + rbi] = arr[mb + rbi];
						rbi++;
					}
				}
				while ((lb + lbi) < mb)
				{
					comcount++;
					buffer[lbi + rbi] = arr[lb + lbi];
					lbi++;
				}
				while ((mb + rbi) < rb)
				{
					comcount++;
					buffer[lbi + rbi] = arr[mb + rbi];
					rbi++;
				}
				for (int mi = 0; mi < lbi + rbi; mi++)
				{
					comcount++;
					arr[lb + mi] = buffer[mi];
				}
				delete buffer;
			}
		}
	}
	t1 = GetTickCount();
	res[5].check[holy] = checksort(arr, n);
	res[5].speed[holy] = (t1 - t0) / cycles;
	res[5].countchanges[holy] = swapcount;
	res[5].countcompare[holy] = comcount;
	//out(arr, n);
	delete[]arr;
}

void analyseCocktailSort(result *res, int n)//Анализ шейкерной сортировки
{
	res[4].sortname = "CocktailSort";
	int holy = log10(n) - 1;
	res[4].size[holy] = n;
	int *arr = new int[n];
	float t0, t1;
	int comcount = 0, swapcount = 0;
	//int left = 0;
	//int right = n - 1;
	t0 = GetTickCount();
	for (int k = 0; k < cycles; k++)
	{
		int left = 0;
		int right = n - 1;
		innit(arr, n);
		swapcount = 0;
		comcount = 0;
		while (left <= right)
		{
			//comcount += 2;
			for (int i = left; i < right; i++)
			{
				comcount += 1;
				if (arr[i] > arr[i + 1])
				{
					swap(arr[i], arr[i + 1]);
					swapcount++;
				}
			}
			right--;
			for (int i = right; i > left; i--)
			{
				comcount += 1;
				if (arr[i - 1] > arr[i])
				{
					swap(arr[i - 1], arr[i]);
					swapcount++;
				}
			}
			left++;
		}
	}
	t1 = GetTickCount();
	res[4].check[holy] = checksort(arr, n);
	res[4].speed[holy] = (t1 - t0) / cycles;
	res[4].countchanges[holy] = swapcount;
	res[4].countcompare[holy] = comcount;
	delete[]arr;
	//out(arr, n);
}

void analyseBubbleSort(result *res, int n)//Анализ сортировки пузырьком
{
	res[3].sortname = "BubbleSort";
	int holy = log10(n) - 1;
	res[3].size[holy] = n;
	int *arr = new int[n];
	float t0, t1;
	int comcount, swapcount;
	t0 = GetTickCount();
	for (int k = 0; k < cycles; k++)
	{
		bool flag = true;
		innit(arr, n);
		comcount = 0;
		swapcount = 0;
		while (flag)
		{
			flag = false;
			comcount++;
			for (int i = 0; i < n - 1; i++)
			{
				comcount++;
				if (arr[i] > arr[i + 1])
				{
					swap(arr[i], arr[i + 1]);
					flag = true;
					swapcount++;
				}
			}
		}
	}

	t1 = GetTickCount();
	res[3].check[holy] = checksort(arr, n);
	res[3].speed[holy] = (t1 - t0) / cycles;
	res[3].countchanges[holy] = swapcount;
	res[3].countcompare[holy] = comcount;
	//out(arr, n);
	delete[] arr;

}

void analyseShellSort(result *res, int n)//Анализ сортировки Шелла
{
	res[2].sortname = "ShellSort";
	int holy = log10(n) - 1;
	res[2].size[holy] = n;
	int *arr = new int[n];
	innit(arr, n);
	float t0, t1;
	int comcount, swapcount;
	int key;
	int j;
	t0 = GetTickCount();
	for (int z = 0; z < cycles; z++)
	{
		innit(arr, n);
		comcount = 0;
		swapcount = 0;
		for (int d = n / 2; d > 0; d /= 2)
		{
			comcount++;
			for (int i = d; i < n; i++)
			{
				comcount++;
				key = arr[i];
				for (j = i; j >= d; j -= d)
				{
					comcount += 2;
					if (key < arr[j - d])
					{
						arr[j] = arr[j - d];
						swapcount++;
					}
					else break;
				}
				arr[j] = key;
			}
		}
	}
	t1 = GetTickCount();
	res[2].check[holy] = checksort(arr, n);
	res[2].speed[holy] = (t1 - t0) / cycles;
	res[2].countchanges[holy] = swapcount;
	res[2].countcompare[holy] = comcount;
	//out(arr,n);
	delete[] arr;
}

void analyseInsertionSort(result *res, int n)//Анализ сортировки вставками
{
	res[1].sortname = "InsertionSort";
	int holy = log10(n) - 1;
	res[1].size[holy] = n;
	int *arr = new int[n];
	//innit(arr, n);
	int key, j;
	int comcount = 0;
	int swapcount = 0;
	float t0, t1;
	t0 = GetTickCount();
	for (int k = 0; k < cycles; k++)
	{
		innit(arr, n);
		comcount = 0;
		swapcount = 0;
		for (int i = 0; i < n; i++)
		{
			key = arr[i];
			j = i;
			comcount += 1;
			while ((j > 0) && (arr[j - 1] > key))
			{
				arr[j] = arr[j - 1];
				comcount++;
				swapcount++;
				j -= 1;
			}
			arr[j] = key;
		}
	}
	t1 = GetTickCount();
	res[1].check[holy] = checksort(arr, n);
	res[1].speed[holy] = (t1 - t0) / cycles;
	res[1].countchanges[holy] = swapcount;
	res[1].countcompare[holy] = comcount;
	//out(arr, n);
	delete[] arr;
}

void analyseSelectionSort(result *res, int n)// анализ сортировки выбором
{
	res[0].sortname = "SelectionSort";
	int holy = log10(n) - 1;
	res[0].size[holy] = n;
	int *arr = new int[n];
	int minindex;
	int comcount = 0;
	int swapcount = 0;
	float t0, t1;
	t0 = GetTickCount();
	//innit(arr, n);
	for (int k = 0; k < cycles; k++)
	{
		innit(arr, n);
		swapcount = 0;
		comcount = 0;
		for (int i = 0; i < n - 1; i++)
		{
			minindex = i;
			comcount++;
			for (int j = i + 1; j < n; j++)
			{
				comcount += 3;
				if (arr[j] < arr[minindex])
				{
					minindex = j;
					comcount++;
				}
			}
			if (minindex != i)
			{
				swap(arr[i], arr[minindex]);
				swapcount++;
			}
		}
	}
	//out(arr, n);
	t1 = GetTickCount();
	res[0].check[holy] = checksort(arr, n);
	res[0].speed[holy] = (t1 - t0) / cycles;
	res[0].countchanges[holy] = swapcount;
	res[0].countcompare[holy] = comcount;
	delete[] arr;
}

void Calculate(result *res) //Записывает в массив Res результаты вычислений
{
	int number;
	for (int z = 0; z < 4; z++)
	{
		number = pow(10, z + 1);
		analyseInsertionSort(res, number);
		analyseBubbleSort(res, number);
		analyseSelectionSort(res, number);
		analyseShellSort(res, number);
		analyseMergeSort(res, number);
		analyseCocktailSort(res, number);
		analyseQuickSort_recursion(res, number);
		analyseQuickSort_unrecursion(res, number);
	}
}

void PrintResult(result *res)//выводит рассчеты в виде таблицы
{
	cout << "__Sort Name________Size_Check__Speed______Swaps______Compares___" << endl;
	for (int u = 0; u < 8; u++)
	{

		for (int u1 = 0; u1 < 4; u1++)
		{
			cout << left << setw(19) << res[u].sortname << left << setw(7) << res[u].size[u1] << left << setw(5) << res[u].check[u1] << setw(8) << left << res[u].speed[u1] << "ms  " << setw(10) << left << res[u].countchanges[u1] << setw(5) << left << res[u].countcompare[u1] << endl;
		}
		cout << "--------------------------------------------------------------------------------" << endl;
	}

}

void main()
{
	result *res = new result[8];
	srand(time(0));
	Calculate(res);
	PrintResult(res);
	getchar();
	getchar();
}
