#define _CRT_SECURE_NO_WARNINGS
#define _CRT_RAND_S
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <conio.h>
#include <stdlib.h>
using namespace std;

//чтение файла
bool readFile(vector <vector <double> >& matrix, double &eps)
{
	int str_start = 0, str_end = 1, str_num = -1;
	double number;
	bool result = false, end = false;
	string str;

	//открытие файла с исходными данными
	ifstream f("input.txt");

	//файл для чтения не открылся
	if (!f)
	{
		cout << "Не удалось открыть файл 'input.txt'" << endl;
		result = true;
	}

	if (!result)
	{
		//заполнение матрицы системы уравнений
		while (str_start != str_end && !end)
		{
			str_start = f.tellg();
			getline(f, str);
			str_end = f.tellg();
			str_end -= 2;
			f.seekg(str_start);

			if (str_start != str_end)
			{
				matrix.push_back(vector <double>());
				str_num++;
			}

			while (f.tellg() < str_end)
			{
				f >> number;
				matrix[str_num].push_back(number);
			}

			getline(f, str);

			if (str_num > matrix[0].size()) {
				end = true;
			}
		}

		if (!end)
		{
			str_num = 0;
			number = 0;
			eps = 0;

			//заполнение столбца свободных членов
			while (!f.eof() && eps == 0)
			{
				f >> number;
				if (str_num == matrix.size())
				{
					eps = number;
				}
				else
				{
					matrix[str_num++].push_back(number);
				}
			}

			//проверка числа строк и столбцов
			for (int i = 1; i < str_num && !end; i++)
			{
				if (matrix[0].size() != matrix[i].size())
					end = true;
			}
		}

		f.close();

		if (end)
		{
			cout << "Неверный формат данных!" << endl;
			result = true;
		}
	}

	return result;
}

//вывод вектора
void printVector(vector <double> vec)
{
	int k = vec.size();

	for (int i = 0; i < k; i++)
	{
		cout << vec[i] << " " << endl;
	}

	cout << endl;

	return;
}

//вывод вектора
void printVector(vector <vector <double> > matrix, int column)
{
	for (int i = 0; i < column; i++)
	{
		cout << matrix[i][column] << " " << endl;
	}

	cout << endl;

	return;
}

//вывод матрицы
void printMatrix(vector <vector <double> > matrix)
{
	int k = matrix.size();

	for (int i = 0; i < k; i++)
	{
		for (int j = 0; j < k; j++)
		{
			cout << matrix[i][j] << " ";
		}

		cout << endl;
	}

	return;
}

//проверка на наличие нулей на главной диагонали
bool check(vector <vector <double> > matrix)
{
	bool result = false;
	int k = matrix.size();

	for (int i = 0; i < k; i++)
	{
		if (matrix[i][i] == 0)
		{
			result = true;
		}
	}

	return result;
}

bool isReady(vector <double> x_old, vector <double> x_new, double eps)
{
	bool result = false;
	int k = x_old.size();
	double max = 0, a;

	for (int i = 0; i < k && max < eps; i++)
	{
		a = abs(x_new[i] - x_old[i]);

		if (a > max)
		{
			max = a;
		}
	}

	if (max < eps)
	{
		result = true;
	}

	return result;
}

//метод Якоби
int jacobiSolution(vector <vector <double> > matrix, vector <double> &x, double eps)
{
	int numberIter = 0;
	int maxIter = 50000;

	int k = matrix.size();

	vector <double> x_old;

	for (int i = 0; i < k; i++)
	{
		x.push_back(matrix[i][k] / matrix[i][i]);
	}

	while (numberIter < maxIter && numberIter >= 0)
	{
		x_old = x;

		for (int i = 0; i < k; i++)
		{
			double sum = 0;

			for (int j = 0; j < k; j++)
			{
				if (i != j)
				{
					sum += matrix[i][j] * x_old[j];
				}
			}

			x[i] = (matrix[i][k] - sum) / matrix[i][i];

			if (x[i] != x[i])
			{
				numberIter = maxIter - 1;
			}
		}

		numberIter++;

		if (isReady(x_old, x, eps))
		{
			break;
		}
		if (numberIter == maxIter)
		{
			numberIter = -1;
		}
	}

	return numberIter;
}

//метод Зейделя
int seidelSolution(vector <vector <double> > matrix, vector <double>& x, double eps)
{
	int numberIter = 0;
	int maxIter = 50000;

	int k = matrix.size();

	vector <double> x_old;

	for (int i = 0; i < k; i++)
	{
		x.push_back(matrix[i][k] / matrix[i][i]);
	}

	while (numberIter < maxIter && numberIter >= 0)
	{
		x_old = x;

		for (int i = 0; i < k; i++)
		{
			double sum = 0;

			for (int j = 0; j < k; j++)
			{
				if (i != j)
				{
					sum += matrix[i][j] * x[j];
				}
			}

			x[i] = (matrix[i][k] - sum) / matrix[i][i];

			if (x[i] != x[i])
			{
				numberIter = maxIter - 1;
				break;
			}
		}

		numberIter++;

		if (isReady(x_old, x, eps))
		{
			break;
		}
		if (numberIter == maxIter)
		{
			numberIter = -1;
		}
	}

	return numberIter;
}

int main()
{
	setlocale(LC_ALL, "Rus");

	int n;
	double eps = 0;
	char choice;

	vector <vector <double> > matrix;
	vector <double> x1, x2;

	cout << "Выберете вариант получения исходных данных: " << endl;
	cout << "1 - Чтение из файла" << endl;
	cout << "2 - Автоматическая генерация" << endl << endl;

	cout << "Введите номер варианта: ";

	do
	{
		choice = _getch();
	} while (choice != 49 && choice != 50);

	if (choice == 49)
	{
		system("cls");

		cout << "Чтение исходных данных из файла..." << endl;

		if (readFile(matrix, eps))
		{
			return -1;
		}

		cout << "Данные успешно считаны" << endl << endl;

		if (check(matrix))
		{
			cout << "На главной диагонали есть нули!" << endl;
			return 1;
		}
	}
	else
	{
		do
		{
			system("cls");
			cout << "Введите количество строк в матрице: ";
			cin >> n;

		} while (n < 1);

		for (int i = 0; i < n; i++)
		{
			double value;
			matrix.push_back(vector <double>());

			for (int j = 0; j <= n; j++)
			{
				unsigned int v1;
				rand_s(&v1);

				do
				{
					value = (int)(((double)v1 / ((double)UINT_MAX + 1) * 100) - 50);
				} while (i == j && value == 0);

				matrix[i].push_back(value);
				cout << value << " " ;
			}

			cout << endl;
			if (i % 5 == 0)
			{
				cout << endl;
			}
		}

		do
		{
			system("cls");
			cout << "Введите точность: ";
			cin >> eps;

		} while (eps <= 0 || eps >= 1);

		system("cls");

		cout << "Сгенерированная матрица: " << endl;

		printMatrix(matrix);

		cout << endl << "Столбец свободных членов: " << endl;

		printVector(matrix, n);

		cout << endl << "Точность: " << eps << endl << endl;
	}

	int numberIter = jacobiSolution(matrix, x1, eps);

	cout << "Метод Якоби" << endl;

	if (numberIter < 0)
	{
		cout << "Решение расходится!" << endl;
	}
	else
	{
		cout << "Количество итераций: " << numberIter << endl;

		cout << "X: " << endl;

		printVector(x1);
	}

	numberIter = seidelSolution(matrix, x2, eps);

	cout << endl << "Метод Зейделя" << endl;

	if (numberIter < 0)
	{
		cout << "Решение расходится!" << endl;
	}
	else
	{
		cout << "Количество итераций: " << numberIter << endl;

		cout << "X: " << endl;

		printVector(x2);
	}

	ofstream result("result.txt");
	if (!result)
	{
		cout << "Не удалось открыть файл для записи результата!" << endl;
		return -1;
	}

	return 0;
}