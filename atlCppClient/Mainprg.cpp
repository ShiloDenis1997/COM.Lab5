// TestATL_3.cpp: определяет точку входа для консольного приложения.
//


#include <iostream>
#include <objbase.h>
#include "lab5_i.h"
#include <atlsafe.h>

using namespace std;

const int N = 3;
double matr[N][N] =
{
	{ 1, 2, 3 },
	{ 2, 3, 4 },
	{ -2, 3, 1 }
};

void trace(const char* msg) { cout << "Client: \t\t" << msg << endl; }

void FromVariantToArray(VARIANT Var, double* Vec, int n)
{
	CComSafeArray<double> SafeArray;
	SafeArray.Attach(Var.parray);
	for (LONG Index = 0; Index < n; Index++)
	{
		Vec[Index] = SafeArray.GetAt(Index);
	}
}

void FromVariantToMatrix(VARIANT Var, double** Vec, int n)
{
	CComSafeArray<double> SafeArray;
	SafeArray.Attach(Var.parray);
	LONG aIndex[2];
	for (LONG i = 0; i < n; i++)
		for (LONG j = 0; j < n; j++)
		{
			aIndex[0] = i;
			aIndex[1] = j;
			SafeArray.MultiDimGetAt(aIndex, Vec[i][j]);
		}
}

void ToVariant(double *Vec, VARIANT* var, int n)
{
	var->vt = VT_ARRAY | VT_R8;

	SAFEARRAYBOUND rgsabound[1];
	rgsabound[0].cElements = n;
	rgsabound[0].lLbound = 0;

	CComSafeArray<double> *safeArray = new CComSafeArray<double>(rgsabound, 1);
	for (int i = 0; i < n; i++)
		{
			safeArray->SetAt(i, Vec[i]);
		}
	safeArray->CopyTo(&(var->parray));
}

void ToVariant(double **Vec, VARIANT* var, int n)
{
	var->vt = VT_ARRAY | VT_R8;

	SAFEARRAYBOUND rgsabound[2];
	rgsabound[0].cElements = n;
	rgsabound[0].lLbound = 0;
	rgsabound[1].cElements = n;
	rgsabound[1].lLbound = 0;

	CComSafeArray<double> *safeArray = new CComSafeArray<double>(rgsabound, 2);
	LONG aIndex[2];
	//var->parray = SafeArrayCreate(VT_R8, 1, rgsabound);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
		{
			aIndex[0] = i;
			aIndex[1] = j;
			safeArray->MultiDimSetAt(aIndex, Vec[i][j]);
		}
	safeArray->CopyTo(&(var->parray));	
}


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

		VARIANT matrA, vectorB, vectorResult;
		ToVariant(m, &matrA, N);
		pIEquationSolver->LoadMatrix(matrA, N);
		double *result = new double[N];
		double *bVector = new double[N] {14, 20, 7};

		ToVariant(bVector, &vectorB, N);
		pIEquationSolver->SolveWithVector(vectorB, &vectorResult, N);
		if (pIEquationPrinter != nullptr)
		{
			cout << "L matrix: " << endl;
			pIEquationPrinter->PrintLMatrix();
			cout << endl << "U matrix:" << endl;
			pIEquationPrinter->PrintUMatrix();
			pIEquationPrinter->Release();
		}

		FromVariantToArray(vectorResult, result, N);
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