// TestATL_3.cpp: определяет точку входа для консольного приложения.
//


#include <iostream>
#include <objbase.h>
#include "lab5_i.h"

using namespace std;

const int N = 3;
double matr[N][N] =
{
	{ 1, 2, 3 },
	{ 2, 3, 4 },
	{ -2, 3, 1 }
};

void trace(const char* msg) { cout << "Client: \t\t" << msg << endl; }

//
// main function
//
int main()
{
	// Initialize COM Library
	CoInitialize(NULL);

	trace("Call CoCreateInstance to create");
	trace("  component and get interface IEquationSolver.");
	IEquationSolver * pIEqSolver = NULL;
	HRESULT hr = ::CoCreateInstance(CLSID_EquationSolver, NULL, CLSCTX_INPROC_SERVER,
		IID_IEquationSolver, (void**)&pIEqSolver);
	cout << "Call done" << endl;

	if (SUCCEEDED(hr))
	{
		cout << "Component created" << endl;
	}
	else
	{
		cout << "Failed to create component" << endl;
		CoUninitialize();
		return 1;
	}

	if (pIEqSolver == nullptr)
	{
		CoUninitialize();
		return 1;
	}

	IEquationSolver* pIEquationSolver = nullptr;
	hr = pIEqSolver->QueryInterface(IID_IEquationSolver, reinterpret_cast<void**>(&pIEquationSolver));
	pIEqSolver->Release();
	IEquationPrinter* pIEquationPrinter = nullptr;
	pIEqSolver->QueryInterface(IID_IEquationPrinter, reinterpret_cast<void**>(&pIEquationPrinter));

	if (SUCCEEDED(hr))
	{
		trace("IEquationPrinter succeeded");
		double **m = new double*[N];
		for (int i = 0; i < N; i++)
		{
			m[i] = new double[N];
			for (int j = 0; j < N; j++)
			{
				m[i][j] = matr[i][j];
			}
		}

		pIEquationSolver->LoadMatrix(m, N);
		double *result = new double[N];
		double *bVector = new double[N] {14, 20, 7};
		pIEquationSolver->SolveWithVector(bVector, result);
		if (pIEquationPrinter != nullptr)
		{
			cout << "L matrix: " << endl;
			pIEquationPrinter->PrintLMatrix();
			cout << endl << "U matrix:" << endl;
			pIEquationPrinter->PrintUMatrix();
			pIEquationPrinter->Release();
		}

		cout << "Result: " << endl;
		for (int i = 0; i < N; i++)
			cout << result[i] << endl;
		for (int i = 0; i < N; i++)
			delete[] m[i];
		delete[] m;
		delete[] result;
		delete[] bVector;
		pIEquationSolver->Release();
	}

	// Uninitialize COM Library
	CoUninitialize();

	return 0;
}