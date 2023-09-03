#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <string>

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