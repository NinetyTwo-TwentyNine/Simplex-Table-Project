#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <string>

using namespace std;

string basicSpace = "	";

class SimplexTable
{
private:
	bool type;
	vector <int> C_arr;
	vector <int> basis;
	vector <int> goalEquasion;
	vector <vector <float>> workingField;

public:
	SimplexTable(vector <int>& goalEquas, vector <vector <int>>& borderArray, vector <int>& resultArray, vector <int>& basicVars, bool& func_type)
	{
		cout << "Got to creating the first table!" << endl;

		type = func_type;
		basis = basicVars;
		goalEquasion = goalEquas;

		C_arr = vector <int>(borderArray.size());
		for (int i = 0; i < borderArray.size(); i++)
		{
			C_arr[i] = goalEquas[basis[i]];
		}
		//cout << "C_arr created properly." << endl;

		workingField = vector <vector <float>>(borderArray.size() + 1);
		for (int i = 0; i < workingField.size(); i++)
		{
			workingField[i] = vector <float>(borderArray[0].size() + 1);

			if (i == workingField.size() - 1)
			{
				workingField[i][0] = 0;
				for (int j = 0; j < C_arr.size(); j++)
				{
					workingField[i][0] += C_arr[j] * workingField[j][0];
				}
			}
			else
			{
				workingField[i][0] = resultArray[i];
			}

			for (int j = 1; j < workingField[i].size(); j++)
			{
				if (i == workingField.size() - 1)
				{
					workingField[i][j] = 0;
					for (int k = 0; k < C_arr.size(); k++)
					{
						workingField[i][j] += C_arr[k] * workingField[k][j];
					}
					workingField[i][j] -= goalEquasion[j - 1];
				}
				else
				{
					workingField[i][j] = borderArray[i][j - 1];
				}
			}
		}
		//cout << "Working Field created successfully." << endl;
	}
	void DisplaySelf()
	{
		cout << fixed;
		cout << setprecision(2);

		cout << basicSpace << basicSpace << basicSpace << basicSpace;
		for (int i = 0; i < goalEquasion.size(); i++)
		{
			cout << "X" << i + 1 << basicSpace;
		}
		cout << endl;

		cout << basicSpace << "BV" << basicSpace << "C" << basicSpace << "A" << basicSpace;
		for (int i = 0; i < goalEquasion.size(); i++)
		{
			cout << goalEquasion[i] << basicSpace;
		}
		cout << endl;

		for (int i = 0; i < workingField.size(); i++)
		{
			if (i != workingField.size() - 1)
			{
				cout << basicSpace << "X" << basis[i] + 1 << basicSpace << C_arr[i] << basicSpace;
			}
			else
			{
				cout << basicSpace << basicSpace << basicSpace;
			}
			for (int j = 0; j < workingField[i].size(); j++)
			{
				cout << workingField[i][j] << basicSpace;
			}
			cout << endl;
		}
	}
	void RebuildSelf()
	{
		vector <int> checkVarIndexArray;
		for (int i = 1; i < workingField[workingField.size() - 1].size(); i++)
		{
			if (workingField[workingField.size() - 1][i] < 0 && type)
			{
				checkVarIndexArray.push_back(i);
			}
			if (workingField[workingField.size() - 1][i] > 0 && !type)
			{
				checkVarIndexArray.push_back(i);
			}
		}

		if (checkVarIndexArray.empty())
		{
			cout << "Wasn't able to get proper variables in the index row. Ending this attempt." << endl;
			return;
		}

		int XPos = 0;
		for (int i = 0; i < checkVarIndexArray.size(); i++)
		{
			if (abs(workingField[workingField.size() - 1][checkVarIndexArray[XPos]]) < abs(workingField[workingField.size() - 1][checkVarIndexArray[i]]))
			{
				XPos = i;
			}
		}
		XPos = checkVarIndexArray[XPos];
		cout << "Solution column: " << XPos << endl;

		bool infiniteCheck = true;
		for (int i = 0; i < workingField.size(); i++)
		{
			if (workingField[i][XPos] > 0)
			{
				infiniteCheck = false;
				break;
			}
		}
		if (infiniteCheck)
		{
			cout << "No proper values in the solution column. Ending this attempt." << endl;
			return;
		}

		int YPos = 0;
		for (int i = 0; i < workingField.size() - 1; i++)
		{
			if (workingField[i][XPos] != 0)
			{
				if (workingField[YPos][XPos] == 0)
				{
					YPos = i;
				}
				else if (workingField[i][0] / workingField[i][XPos] > 0)
				{
					if (workingField[i][0] / workingField[i][XPos] < workingField[YPos][0] / workingField[YPos][XPos])
					{
						YPos = i;
					}
				}
			}
		}
		cout << "Solution row: " << YPos << endl;

		if (workingField[YPos][XPos] == 0)
		{
			cout << "Got zero as a solution value. Ending this attempt." << endl;
			return;
		}
		double solutionVar = workingField[YPos][XPos];

		basis[YPos] = XPos - 1;
		C_arr[YPos] = goalEquasion[XPos - 1];
		cout << "Solution value: " << solutionVar << endl;


		vector <vector <float>> oldWorkingField(workingField.size());
		for (int i = 0; i < oldWorkingField.size(); i++)
		{
			oldWorkingField[i] = vector <float>(workingField[i].size());
			for (int j = 0; j < oldWorkingField[i].size(); j++)
			{
				oldWorkingField[i][j] = workingField[i][j];
			}
		}

		for (int i = 0; i < workingField.size(); i++)
		{
			for (int j = 0; j < workingField[i].size(); j++)
			{
				if (i == YPos)
				{
					workingField[i][j] = oldWorkingField[i][j] / solutionVar;
				}
				else if (j == XPos)
				{
					workingField[i][j] = 0;
				}
				else
				{
					workingField[i][j] = (oldWorkingField[i][j] * oldWorkingField[YPos][XPos] - oldWorkingField[YPos][j] * oldWorkingField[i][XPos]) / solutionVar;
				}
			}
		}
		DisplaySelf();

		bool canStillDoBetter = false;
		for (int i = 0; i < workingField[workingField.size() - 1].size(); i++)
		{
			if (workingField[workingField.size() - 1][i] < 0 && type)
			{
				canStillDoBetter = true;
				break;
			}
			if (workingField[workingField.size() - 1][i] > 0 && !type)
			{
				canStillDoBetter = true;
				break;
			}
		}

		if (canStillDoBetter)
		{
			RebuildSelf();
		}
		else
		{
			cout << "Reached final table." << endl;
		}
	}
};