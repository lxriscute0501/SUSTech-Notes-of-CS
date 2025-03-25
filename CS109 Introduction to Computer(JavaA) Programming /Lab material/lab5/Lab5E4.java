import java.util.Scanner;

public class Lab5E4 {

	public static void main(String[] args) {
		Scanner in = new Scanner(System.in);
		int[][] result = null;
		System.out.print("Please enter the number of matrixes: ");
		int T = in.nextInt();
		for (int t = 1; t <= T; t++) {
			System.out.printf("Enter the number of row and column of matrix %d: ", t);
			int m = in.nextInt();
			int n = in.nextInt();
			int[][] matrix = new int[m][n];
			System.out.println("Enter the elements of the matrix: ");
			for (int i = 0; i < m; i++) {
				for (int j = 0; j < n; j++) {
					matrix[i][j] = in.nextInt();
				}
			}
			result = matrixProduct(result, matrix);
		}
		printMatrix(result);
		in.close();
	}

	public static void printMatrix(int[][] a) {
		int m = a.length, n = a[0].length;
		System.out.println("The result: ");
		for (int i = 0; i < m; i++) {
			for (int j = 0; j < n; j++) {
				System.out.printf("%-5d", a[i][j]);
			}
			System.out.println();
		}
	}

	// suppose a is of m*p and b is p*n
	// c is the result matrix of m*n
	public static int[][] matrixProduct(int[][] a, int[][] b) {
		if (a == null)
			return b;
		int m = a.length, n = b[0].length, p = b.length;
		int[][] c = new int[m][n];
		for (int i = 0; i < m; i++) {
			for (int j = 0; j < n; j++) {
				for (int k = 0; k < p; k++) {
					c[i][j] += a[i][k] * b[k][j];
				}
			}
		}
		return c;
	}

}
