// EquationSolver.cpp : Implementation of CEquationSolver

#include "stdafx.h"
#include "EquationSolver.h"
#include <iostream>

// CEquationSolver

HRESULT CEquationSolver::PrintLMatrix()
{
	if (this->m_LUmatr == nullptr)
	{
		compTrace("No matrix provided");
		return E_FAIL;
	}

	for (int i = 0; i < this->m_N; i++)
	{
		for (int j = 0; j < this->m_N; j++)
		{
			if (j > i)
			{
				std::cout << 0 << " ";
			}
			else if (j < i)
			{
				std::cout << this->m_LUmatr[i][j] << " ";
			}
			else
			{
				std::cout << "1 ";
			}
		}

		std::cout << std::endl;
	}
	return S_OK;
}

HRESULT CEquationSolver::PrintUMatrix()
{
	if (this->m_LUmatr == nullptr)
	{
		std::cout << TEXT("No matrix provided") << std::endl;
		return E_FAIL;
	}

	for (int i = 0; i < this->m_N; i++)
	{
		for (int j = 0; j < this->m_N; j++)
		{
			if (j < i)
			{
				std::cout << 0 << " ";
			}
			else if (j >= i)
			{
				std::cout << this->m_LUmatr[i][j] << " ";
			}
		}

		std::cout << std::endl;
	}
}

HRESULT CEquationSolver::LoadMatrix(double** a, int n)
{
	DeleteMatrix();
	m_N = n;
	m_LUmatr = new double*[n];
	for (int i = 0; i < n; i++)
	{
		m_LUmatr[i] = new double[n];
		for (int j = 0; j < n; j++)
		{
			m_LUmatr[i][j] = a[i][j];
		}
	}
	m_pRow = new int[n];
	m_pCol = new int[n];
	if (MakeLU(m_LUmatr, m_pRow, m_pCol, n))
		return S_OK;
	return E_FAIL;
}

HRESULT CEquationSolver::SolveWithVector(double* b, double* result)
{
	double *temp = new double[m_N];
	for (int i = 0; i < m_N; i++)
	{
		temp[i] = b[m_pRow[i]];
	}
	SolveL(temp);
	SolveU(temp);
	for (int i = 0; i < m_N; i++)
	{
		result[m_pCol[i]] = temp[i];
	}
	delete[] temp;
	return S_OK;
}

bool CEquationSolver::MakeLU(double ** a, int * pr, int * pc, int n) const
{
	for (int i = 0; i < n; i++)
		pr[i] = pc[i] = i;
	for (int k = 0; k <= n - 2; k++)
	{
		double mx = fabs(a[k][k]);
		int psr = k, psc = k;
		for (int i = k; i <= n - 1; i++)
			for (int j = k; j <= n - 1; j++)
			{
				if (fabs(a[i][k]) > mx)
				{
					mx = fabs(a[i][k]);
					psr = i;
					psc = j;
				}
			}
		if (mx < 1e-15)
			return false;
		if (psr != k || psc != k)
		{
			std::swap(a[psr], a[k]);
			swapColumns(a, psc, k, n);
			std::swap(pr[psr], pr[k]);
			std::swap(pc[psc], pc[k]);
		}
		for (int i = k + 1; i <= n - 1; i++)
		{
			a[i][k] /= a[k][k];
			for (int j = k + 1; j <= n - 1; j++)
				a[i][j] -= a[k][j] * a[i][k];
		}
	}
	return true;
}

void CEquationSolver::SolveL(double* y) const
{
	for (int i = 0; i < m_N; i++)
	{
		for (int j = 0; j < i; j++)
		{
			y[i] -= y[j] * m_LUmatr[i][j];
		}
	}
}

void CEquationSolver::SolveU(double* x) const
{
	for (int i = m_N - 1; i >= 0; i--)
	{
		for (int j = m_N - 1; j > i; j--)
		{
			x[i] -= x[j] * m_LUmatr[i][j];
		}
		x[i] /= m_LUmatr[i][i];
	}
}

void CEquationSolver::DeleteMatrix()
{
	compTrace("\t\tCEquationComponent: Delete matrix");
	if (m_LUmatr == nullptr)
		return;

	for (int i = 0; i < m_N; i++)
	{
		delete[] m_LUmatr[i];
	}

	delete[] m_LUmatr;
	delete[] m_pCol;
	delete[] m_pRow;
	m_LUmatr = nullptr;
	compTrace("\t\tCEquationComponent: Deleted");
}

void CEquationSolver::swapColumns(double** a, int c1, int c2, int n)
{
	if (c1 == c2)
		return;
	for (int i = 0; i < n; i++)
	{
		std::swap(a[i][c1], a[i][c2]);
	}
}