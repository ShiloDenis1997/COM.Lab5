using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using lab5Lib;
using System.Runtime.InteropServices;

namespace csharpClient
{
    class Program
    {
        static void Main(string[] args)
        {

            unsafe
            {

                double[,] ar = new[,] {
                    { 1.0, 2, 3 },
                    { 2, 3, 4 },
                    { -2, 3, 1 } };
                IEquationSolver solver = new EquationSolver();

                double[] bVector = new double[] { 14, 20, 7 };
                object ansObj;
                solver.LoadMatrix(ar, ar.GetLength(0));
                solver.SolveWithVector(bVector, out ansObj, bVector.Length);
                double[] ans = ansObj as double[];

                Console.WriteLine("Result: ");
                for (int i = 0; i < ans.Length; i++)
                {
                    Console.WriteLine(ans[i]);
                }
            }

        }
    }
}
