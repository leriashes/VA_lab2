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

			if (str_num + 1 != matrix[0].size())
				end = true;
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

//вывод вектора в файл
void fprintVector(vector <double> vec, ofstream& file)
{
	int k = vec.size();

	for (int i = 0; i < k; i++)
	{
		file << vec[i] << " " << endl;
	}

	file << endl;

	return;
}

//вывод вектора в файл
void fprintVector(vector <vector <double> > matrix, int column, ofstream& file)
{
	for (int i = 0; i < column; i++)
	{
		file << matrix[i][column] << " " << endl;
	}

	file << endl;

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

//вывод матрицы в файл
void fprintMatrix(vector <vector <double> > matrix, ofstream& file)
{
	int k = matrix.size();

	for (int i = 0; i < k; i++)
	{
		for (int j = 0; j < k; j++)
		{
			file << matrix[i][j] << " ";
		}

		file << endl;
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

//проверка условия по точности
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

//определение невязок
vector <double> residual(vector <vector <double> > matrix, vector <double> x)
{
	vector <double> r;
	int k = matrix.size();

	for (int i = 0; i < k; i++)
	{
		r.push_back(matrix[i][k]);

		for (int j = 0; j < k; j++)
		{
			r[i] -= matrix[i][j] * x[j];
		}
	}

	return r;
}

//определение нормы
double norma(vector<double> vec)
{
	double n = fabs(vec[0]);
	int k = vec.size();

	for (int i = 1; i < k; i++)
	{
		if (fabs(vec[i]) > n)
		{
			n = fabs(vec[i]);
		}
	}

	return n;
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

			if (x[i] != x[i] || x[i] == INFINITY)
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

			if (x[i] != x[i] || x[i] == INFINITY)
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
	double eps = 0, nor;
	char choice;

	vector <vector <double> > matrix;
	vector <double> x1, x2, r;

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
					value = (int)(((double)v1 / ((double)UINT_MAX + 1) * 50) - 15);
				} while (i == j && value == 0);

				matrix[i].push_back(value * (1 + (n - 1) * (i == j)));
			}
		}

		do
		{
			system("cls");
			cout << "Введите точность: ";
			cin >> eps;

		} while (eps <= 0 || eps >= 1);

		system("cls");
	}

	ofstream result("result.txt");
	if (!result)
	{
		cout << "Не удалось открыть файл для записи результата!" << endl;
		return -1;
	}

	n = matrix.size();


	//вывод матрицы A
	cout << "Матрица A" << endl;
	printMatrix(matrix);

	result << "Матрица A" << endl;
	fprintMatrix(matrix, result);

	//вывод вектора B
	cout << "\nВектор B" << endl;
	printVector(matrix, n);

	result << "\nВектор B" << endl;
	fprintVector(matrix, n, result);

	cout << endl << "Точность: " << eps << endl << endl;
	result << endl << "Точность: " << eps << endl << endl;

	int numberIter = jacobiSolution(matrix, x1, eps);

	cout << "Метод Якоби" << endl;
	result << "Метод Якоби" << endl;

	if (numberIter < 0)
	{
		cout << "Решение расходится!" << endl;
		result << "Решение расходится!" << endl;
	}
	else
	{
		cout << "Количество итераций: " << numberIter << endl;
		cout << "X: " << endl;
		printVector(x1);

		result << "Количество итераций: " << numberIter << endl;
		result << "X: " << endl;
		fprintVector(x1, result);

		r = residual(matrix, x1);

		cout << "Невязки: " << endl;
		printVector(r);

		result << "Невязки: " << endl;
		fprintVector(r, result);

		nor = norma(r);

		cout << "Норма невязок: " << nor << endl << endl;
		result << "Норма невязок: " << nor << endl << endl;
	}

	numberIter = seidelSolution(matrix, x2, eps);

	cout << "\nМетод Зейделя" << endl;
	result << "\nМетод Зейделя" << endl;

	if (numberIter < 0)
	{
		cout << "Решение расходится!" << endl;
		result << "Решение расходится!" << endl;
	}
	else
	{
		cout << "Количество итераций: " << numberIter << endl;
		cout << "X: " << endl;
		printVector(x2);

		result << "Количество итераций: " << numberIter << endl;
		result << "X: " << endl;
		fprintVector(x2, result);

		r = residual(matrix, x2);

		cout << "Невязки: " << endl;
		printVector(r);

		result << "Невязки: " << endl;
		fprintVector(r, result);

		nor = norma(r);

		cout << "Норма невязок: " << nor << endl << endl;
		result << "Норма невязок: " << nor << endl << endl;
	}

	return 0;
}