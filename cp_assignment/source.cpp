#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

struct MACHINE
{
	string machine;
	int number;
};

void validateSensors(int machine_no, float temp[], float rpm[], float vibration_lvl[], bool check_temp[], bool check_rpm[], bool check_vibration_lvl[]);

int main()
{
	cout << "CNC MILLING MACHINE PERFORMANCE MONITORING (Max 10 machines)" << endl;

	//Get input from input.txt file
	ifstream machine_input("input.txt", ios::in);
	float temp[10], rpm[10], vibration_lvl[10];
	int machine_no = 0;
	
	if (machine_input)	
	{
		for (int h = 0; machine_input >> temp[h]; h++)	//For loop runs as long as input from machine_input stream returns TRUE
		{
			machine_input >> rpm[h] >> vibration_lvl[h];
			machine_no++;
		}
	}
	else	//If machine_input did not open a file, output error
	{
		cout << endl << "Error reading input file" << endl;
		return 0;
	}

	machine_input.close();

	//Check if inputs for each machine are in normal range and assign TRUE(normal) / FALSE(abnormal) value
	bool check_temp[10], check_rpm[10], check_vibration_lvl[10];
	
	//Function Call for Sensor Validation
	validateSensors(machine_no, temp, rpm, vibration_lvl, check_temp, check_rpm, check_vibration_lvl);

	int fault_count[10] = { 0 };
	cout << endl << endl << "STATUS REPORT PER MACHINE" << endl << "---------------------------------";
	for (int j = 0; j < machine_no; j++)
	{
		cout << endl << "Machine " << j + 1 << ":" << endl;
		if (check_temp[j] && check_rpm[j] && check_vibration_lvl[j])	//If all condition is TRUE(normal), outputs normal condition
		{
			cout << "In normal condition" << endl;
		}
		else	//Check for the different types of faulty condition and outputs its corresponding message
		{
			if (!check_temp[j])
			{
				cout << "Overheating" << endl;
				fault_count[j] += 1;
			}

			if (!check_rpm[j])
			{
				cout << "Drill tip spinning abnormally" << endl;
				fault_count[j] += 1;
			}

			if (!check_vibration_lvl[j])
			{
				cout << "Excessive machine vibration" << endl;
				fault_count[j] += 1;
			}
		}
	}

	//Assign vector with the particular machine number after checking for the worst performing machine in relevancy of total faulty condition
	
	vector<MACHINE> check_worst_performing; 
	for (int l = 0; l <= machine_no; l++)
	{
		if (fault_count[l] == fault_count[l - 1])	//If there is the same number of faulty conditions with the previous machine, add it to the vector
		{
			check_worst_performing.emplace_back("Machine", l + 1);
		}
		else if (fault_count[l] == 3)	//If there is a higher number of faulty condition when compare to the previous machine, clear the vector consisting the lower number of faulty conditions and add the machine with the higher number of faulty conditions
		{
			check_worst_performing.clear();
			check_worst_performing.emplace_back("Machine", l + 1);
		}
		else if ((fault_count[l] == 2) && (fault_count[l] > fault_count[l - 1]))
		{
			check_worst_performing.clear();
			check_worst_performing.emplace_back("Machine", l + 1);
		}
		else if ((fault_count[l] == 1) && (fault_count[l] > fault_count[l - 1]))
		{
			check_worst_performing.clear();
			check_worst_performing.emplace_back("Machine", l + 1);
		}
		else if ((fault_count[l] == 0) && (fault_count[l] > fault_count[l - 1]))
		{
			check_worst_performing.clear();
			check_worst_performing.emplace_back("Machine", l + 1);
		}
	}

	//Assigned the number of faulty machine detected
	int faulty_machine = 0;
	for (int k = 0; k <= machine_no; k++)
	{
		if (fault_count[k] > 0)
		{
			faulty_machine += 1;
		}
	}

	//Print the outputs
	cout << "---------------------------------" << endl;
	cout << endl << "Worst performing machine: ";
	for (auto count : check_worst_performing)	//Range-based for loop that runs the number of times corresponding to the total elements in the vector
	{
		cout << count.machine << " " << count.number << ", ";		//Outputs the first part then the second part of the vector with struct type
	}
	cout << endl << "Number of faulty machine: " << faulty_machine << endl;

	return 0 ;
}


void validateSensors(int machine_no, float temp[], float rpm[], float vibration_lvl[], bool check_temp[], bool check_rpm[], bool check_vibration_lvl[])
{
	for (int i = 0; i <= machine_no; i++)
	{
		if (temp[i] > 100)
		{
			check_temp[i] = false;
		}
		else
		{
			check_temp[i] = true;
		}

		if (rpm[i] < 4000 || rpm[i] > 8000)
		{
			check_rpm[i] = false;
		}
		else
		{
			check_rpm[i] = true;
		}

		if (vibration_lvl[i] >= 2.0)
		{
			check_vibration_lvl[i] = false;
		}
		else
		{
			check_vibration_lvl[i] = true;
		}
	}
}