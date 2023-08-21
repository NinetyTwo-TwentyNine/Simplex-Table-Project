#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <string>

#include "SimplexTable.cpp"

using namespace std;

void printGoalEquasion(vector <int>& equas, bool type)
{
	if (type)
	{
		cout << "maxF = ";
	}
	else
	{
		cout << "minF = ";
	}
	for (int i = 0; i < equas.size(); i++)
	{
		if (equas[i] >= 0 && i > 0)
		{
			cout << "+";
		}
		cout << equas[i] << "X" << i + 1;
	}
	cout << endl;
}

void printBorderEquasion(vector <int>& equas, int result, bool type)
{
	for (int i = 0; i < equas.size(); i++)
	{
		if (equas[i] >= 0 && i > 0)
		{
			cout << "+";
		}
		cout << equas[i] << "X" << i + 1;
	}
	if (type)
	{
		cout << "<=";
	}
	else
	{
		cout << "=>";
	}
	cout << result << endl;
}

vector<int> getBasicVariables(vector <vector <int>>& borders)
{
	vector <bool> checkArray(borders[0].size());
	vector <int> checkArray_appearNum(borders[0].size());
	for (int i = 0; i < checkArray.size(); i++)
	{
		checkArray[i] = true;
		checkArray_appearNum[i] = 0;
	}

	for (int i = 0; i < borders.size(); i++)
	{
		for (int j = 0; j < borders[i].size(); j++)
		{
			if (!checkArray[j])
			{
				continue;
			}
			if (borders[i][j] != 0)
			{
				checkArray_appearNum[j]++;
				if (borders[i][j] != 1 || checkArray_appearNum[j] > 1)
				{
					checkArray[j] = false;
				}
			}
		}
	}

	vector <int> basics;
	for (int i = 0; i < checkArray.size(); i++)
	{
		if (checkArray[i] && checkArray_appearNum[i] > 0)
		{
			basics.push_back(i);
		}
	}
	
	return basics;
}

SimplexTable createInitialTable(vector <int>& goalEquas, vector <vector <int>>& borderArray, vector <int>& resultArray, bool& func_type)
{
	vector <int> initialBasis = getBasicVariables(borderArray);

	if (initialBasis.size() < borderArray.size())
	{
		for (int i = 0; i < borderArray.size() - initialBasis.size(); i++)
		{
			goalEquas.push_back(0);
			for (int j = 0; j < borderArray.size(); j++)
			{
				if (j == i)
				{
					borderArray[j].push_back(1);
				}
				else
				{
					borderArray[j].push_back(0);
				}
			}
		}
	}
	initialBasis = getBasicVariables(borderArray);
	SimplexTable testTable(goalEquas, borderArray, resultArray, initialBasis, func_type);
	testTable.DisplaySelf();

	return testTable;
}

int main()
{
	setlocale(LC_ALL, "");
	bool func_type;
	int n_goal, n_border;
	cout << "Введите количество переменных в уравнении целевой функции: ";
	cin >> n_goal;
	while (n_goal <= 0)
	{
		cout << "Введено невозможное кол-во переменных целевой функции. Введите это чило ещё раз: ";
		cin >> n_goal;
	}
	
	vector <int> indexArray_goal(n_goal);
	for (int i = 0; i < n_goal; i++)
	{
		cout << "Введите индекс " << i+1 << "-й переменной в уравнении целевой функции: ";
		cin >> indexArray_goal[i];
	}

	cout << "Найти максимум или минимум функции? (1/0): ";
	cin >> func_type;

	cout << "Введите количество ограничительных уравнений: ";
	cin >> n_border;
	while (n_border <= 0)
	{
		cout << "Введено невозможное кол-во ограничительных уравнений. Введите это чило ещё раз: ";
		cin >> n_border;
	}

	vector <vector <int>> indexArray_border(n_border);
	vector <int> resultArray_border(n_border);
	vector <int> typeArray_border(n_border);

	for (int i = 0; i < n_border; i++)
	{
		indexArray_border[i] = vector <int>(n_goal);
		for (int j = 0; j < n_goal; j++)
		{
			cout << "Введите индекс " << j + 1 << "-й переменной " << i + 1 << "-го ограничительного уравнения: ";
			cin >> indexArray_border[i][j];
		}
		cout << "Введите ограничение " << i + 1 << "-го уравнения: ";
		cin >> resultArray_border[i];
		if (resultArray_border[i] > 0)
		{
			typeArray_border[i] = func_type;
		}
		else
		{
			typeArray_border[i] = !func_type;
			resultArray_border[i] = resultArray_border[i] * -1;
			for (int j = 0; j < indexArray_border[i].size(); j++)
			{
				indexArray_border[i][j] = indexArray_border[i][j] * -1;
			}
		}
	}

	printGoalEquasion(indexArray_goal, func_type);
	for (int i = 0; i < n_border; i++)
	{
		printBorderEquasion(indexArray_border[i], resultArray_border[i], typeArray_border[i]);
	}

	SimplexTable table = createInitialTable(indexArray_goal, indexArray_border, resultArray_border, func_type);
	table.RebuildSelf();
	return 0;
}
