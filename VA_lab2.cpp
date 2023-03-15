#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
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
	int maxIter = 500;

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
		}

		numberIter++;

		if (isReady(x_old, x, eps))
		{
			break;
		}
		if (numberIter == 500)
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

	int k, p;
	double eps;

	vector <vector <double> > matrix;
	vector <double> x1, x2;

	if (readFile(matrix, eps))
	{
		return -1;
	}

	if (check(matrix))
	{
		cout << "На главной диагонали есть нули!" << endl;
		return 1;
	}

	int numberIter = jacobiSolution(matrix, x1, eps);

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