// EquationSolver.h : Declaration of the CEquationSolver

#pragma once
#include "resource.h"       // main symbols



#include "lab5_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;


// CEquationSolver
void compTrace(const char* msg) { printf("Component: %s\n", msg); }

class ATL_NO_VTABLE CEquationSolver :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CEquationSolver, &CLSID_EquationSolver>,
	public IEquationSolver,
	public IEquationPrinter
{
public:
	CEquationSolver()
	{
		m_N = 0;
		m_LUmatr = nullptr;
		m_pRow = nullptr;
		m_pCol = nullptr;
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_EQUATIONSOLVER)

	DECLARE_NOT_AGGREGATABLE(CEquationSolver)

	BEGIN_COM_MAP(CEquationSolver)
		COM_INTERFACE_ENTRY(IEquationSolver)
		COM_INTERFACE_ENTRY(IEquationPrinter)
	END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
		compTrace("Release component");
	}

private:
	bool MakeLU(double **a, int* pr, int* pc, int n) const;
	void SolveL(double* y) const;
	void SolveU(double* x) const;
	void DeleteMatrix();
	static void swapColumns(double **a, int c1, int c2, int n);
	int m_N;
	int *m_pRow;
	int *m_pCol;
	double **m_LUmatr;

	// IEquationPrinter Methods
public:
	STDMETHOD(PrintLMatrix)();
	STDMETHOD(PrintUMatrix)();

	// IEquationTrickSolver Methods
public:
	STDMETHOD(LoadMatrix)(VARIANT a, int n);
	STDMETHOD(SolveWithVector)(VARIANT b, VARIANT* result, int n);
};

OBJECT_ENTRY_AUTO(__uuidof(EquationSolver), CEquationSolver)
