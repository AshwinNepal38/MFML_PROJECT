#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX 20
#define EPSILON 1e-9

// Function to print the augmented matrix
void printMatrix(double matrix[MAX][MAX + 1], int n)
{
    int i, j;

    for (i = 0; i < n; i++)
    {
        printf("[ ");

        for (j = 0; j < n; j++)
        {
            printf("%8.2lf ", matrix[i][j]);
        }

        printf("| %8.2lf ]\n", matrix[i][n]);
    }
}

// Function to swap two rows
void swapRows(double matrix[MAX][MAX + 1], int row1, int row2, int n)
{
    int j;
    double temp;

    for (j = 0; j <= n; j++)
    {
        temp = matrix[row1][j];
        matrix[row1][j] = matrix[row2][j];
        matrix[row2][j] = temp;
    }
}

// Function to perform Gaussian elimination
int gaussianElimination(double matrix[MAX][MAX + 1], int n)
{
    int i, j, k;
    int pivotRow;

    double maxValue;
    double factor;

    for (i = 0; i < n - 1; i++)
    {
        // Assume current row has the largest pivot
        pivotRow = i;
        maxValue = fabs(matrix[i][i]);

        // Find the row with the largest pivot
        for (k = i + 1; k < n; k++)
        {
            if (fabs(matrix[k][i]) > maxValue)
            {
                maxValue = fabs(matrix[k][i]);
                pivotRow = k;
            }
        }

        // Check if the pivot is zero
        if (maxValue < EPSILON)
        {
            printf("\nZero pivot encountered.\n");
            printf("The system does not have a unique solution.\n");

            return 0;
        }

        // Swap rows if a better pivot is found
        if (pivotRow != i)
        {
            swapRows(matrix, i, pivotRow, n);

            printf("\nAfter row interchange:\n");
            printMatrix(matrix, n);
        }

        // Eliminate the elements below the pivot
        for (j = i + 1; j < n; j++)
        {
            factor = matrix[j][i] / matrix[i][i];

            for (k = i; k <= n; k++)
            {
                matrix[j][k] =
                    matrix[j][k] - factor * matrix[i][k];
            }
        }
    }

    // Check the last pivot
    if (fabs(matrix[n - 1][n - 1]) < EPSILON)
    {
        printf("\nZero pivot encountered in the last row.\n");
        printf("The system does not have a unique solution.\n");

        return 0;
    }

    return 1;
}

// Function to perform back substitution
int backSubstitution(
    double matrix[MAX][MAX + 1],
    double solution[MAX],
    int n)
{
    int i, j;
    double sum;

    // Start from the last equation
    for (i = n - 1; i >= 0; i--)
    {
        // Check for zero pivot
        if (fabs(matrix[i][i]) < EPSILON)
        {
            return 0;
        }

        // Start with the right hand side value
        sum = matrix[i][n];

        // Subtract the values of already calculated variables
        for (j = i + 1; j < n; j++)
        {
            sum = sum - matrix[i][j] * solution[j];
        }

        // Calculate the current variable
        solution[i] = sum / matrix[i][i];
    }

    return 1;
}

// Function to verify the solution
void verifySolution(
    double coefficient[MAX][MAX],
    double rhs[MAX],
    double solution[MAX],
    int n)
{
    int i, j;
    double calculated;
    double error;

    printf("\nVerification:\n");

    for (i = 0; i < n; i++)
    {
        calculated = 0;

        // Calculate the left hand side of the equation
        for (j = 0; j < n; j++)
        {
            calculated =
                calculated + coefficient[i][j] * solution[j];
        }

        // Calculate the error
        error = fabs(calculated - rhs[i]);

        printf("Equation %d: Calculated = %.6lf, RHS = %.6lf",
               i + 1, calculated, rhs[i]);

        if (error < EPSILON)
        {
            printf(" -> Verified\n");
        }
        else
        {
            printf(" -> Not Verified\n");
        }
    }
}

// Function to take input and solve one system
void solveSystem(int n)
{
    double matrix[MAX][MAX + 1];
    double coefficient[MAX][MAX];
    double rhs[MAX];
    double solution[MAX];

    int i, j;

    // Input the coefficient matrix
    printf("\nEnter the coefficient matrix:\n");

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            printf("A[%d][%d] = ", i + 1, j + 1);
            scanf("%lf", &coefficient[i][j]);

            matrix[i][j] = coefficient[i][j];
        }
    }

    // Input the right hand side vector
    printf("\nEnter the RHS vector:\n");

    for (i = 0; i < n; i++)
    {
        printf("b[%d] = ", i + 1);
        scanf("%lf", &rhs[i]);

        matrix[i][n] = rhs[i];
    }

    // Display the coefficient matrix
    printf("\nInput coefficient matrix:\n");

    for (i = 0; i < n; i++)
    {
        printf("[ ");

        for (j = 0; j < n; j++)
        {
            printf("%8.2lf ", coefficient[i][j]);
        }

        printf("]\n");
    }

    // Display the RHS vector
    printf("\nRHS vector:\n");

    for (i = 0; i < n; i++)
    {
        printf("[ %.2lf ]\n", rhs[i]);
    }

    // Display the augmented matrix
    printf("\nAugmented matrix:\n");
    printMatrix(matrix, n);

    // Perform forward elimination
    if (!gaussianElimination(matrix, n))
    {
        printf("\nCannot perform back substitution.\n");
        return;
    }

    // Display the matrix after elimination
    printf("\nMatrix after forward elimination:\n");
    printMatrix(matrix, n);

    // Perform back substitution
    if (!backSubstitution(matrix, solution, n))
    {
        printf("\nBack substitution failed.\n");
        printf("The system does not have a unique solution.\n");

        return;
    }

    // Display the solution
    printf("\nSolution:\n");

    for (i = 0; i < n; i++)
    {
        printf("x%d = %.6lf\n", i + 1, solution[i]);
    }

    // Verify the solution
    verifySolution(coefficient, rhs, solution, n);
}

int main()
{
    int n;
    int numberOfSystems;
    int systemNumber;

    // Ask how many systems the user wants to solve
    printf("Enter the number of systems to solve: ");
    scanf("%d", &numberOfSystems);

    // Check if the number of systems is valid
    if (numberOfSystems <= 0)
    {
        printf("Invalid number of systems.\n");
        return 1;
    }

    // Solve each system
    for (systemNumber = 1;
         systemNumber <= numberOfSystems;
         systemNumber++)
    {
        printf("\nSystem %d\n", systemNumber);

        // Ask for the size of the matrix
        printf("Enter the number of variables/equations: ");
        scanf("%d", &n);

        // Check if the matrix size is valid
        if (n <= 0 || n > MAX)
        {
            printf("Invalid matrix dimension.\n");
            printf("Enter a size between 1 and %d.\n", MAX);

            return 1;
        }

        // Solve the system
        solveSystem(n);
    }

    printf("\nProgram completed.\n");

    return 0;
}