// TestATL_3.cpp: определяет точку входа для консольного приложения.
//


#include <iostream>
#include <objbase.h>
#include "lab5_i.h"

using namespace std;

void trace(const char* msg) { cout << "Client: \t\t" << msg << endl; }

//
// main function
//
int main()
{
	// Initialize COM Library
	CoInitialize(NULL);

	trace("Call CoCreateInstance to create");
	trace("  component and get interface IX.");
	IEquationSolver * pIEqSolver = NULL;
	HRESULT hr = ::CoCreateInstance(CLSID_EquationSolver, NULL, CLSCTX_INPROC_SERVER,
		IID_IEquationSolver, (void**)&pIEqSolver);
	if (SUCCEEDED(hr))
	{
		/*trace("Succeeded getting ILoadDataK.");
		BSTR bstr = SysAllocString(OLESTR("in.txt"));

		HRESULT hr1 = pIEqSolver->LoadD(bstr);
		if (SUCCEEDED(hr1))
			cout << "SUC" << endl;
		else
			cout << "NO SUC" << endl;*/


		//pIX->Fx();          // Use interface IX.

		trace("Ask for interface IComputeK.");
		IEquationPrinter * pIEqPrinter = NULL;
		HRESULT hr2 = pIEqSolver->QueryInterface(IID_IEquationPrinter, (void**)&pIEqPrinter);
		if (SUCCEEDED(hr2))
		{
			trace("Succeeded getting IComputeK.");
			pIEqPrinter->PrintLMatrix();
			pIEqPrinter->Release();
			trace("Release IComputeK interface.");

		}
		else
		{
			trace("Could not get interface IComputeK.");
		}


		trace("Release ILoadDataK interface.");
		pIEqSolver->Release();
	}
	else
	{
		cout << "Client: \t\tCould not create component. hr = "
			<< hex << hr << endl;
	}

	// Uninitialize COM Library
	CoUninitialize();

	return 0;
}