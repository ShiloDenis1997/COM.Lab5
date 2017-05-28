Imports lab5Lib

Module MainModule

    Sub Main()
        Console.WriteLine("Hello from Visual Basic client!")
        Dim ar As Double(,) = New Double(,) {{1.0, 2, 3}, {2, 3, 4}, {-2, 3, 1}}
        Dim solver As IEquationSolver = New EquationSolver()

        Dim bVector As Double() = New Double() {14, 20, 7}
        Dim ansObj As Object
        solver.LoadMatrix(ar, ar.GetLength(0))
        solver.SolveWithVector(bVector, ansObj, bVector.Length)
        Dim ans As Double() = TryCast(ansObj, Double())

        Console.WriteLine("Result: ")
        For i As Integer = 0 To ans.Length - 1
            Console.WriteLine(ans(i))
        Next

        Dim printer As IEquationPrinter = TryCast(solver, IEquationPrinter)
        Console.WriteLine("L matrix:")
        printer.PrintLMatrix()
        Console.WriteLine("U matrix:")
        printer.PrintUMatrix()
    End Sub

End Module
