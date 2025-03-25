import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        int n = scanner.nextInt();
        int m = scanner.nextInt();
        int p = scanner.nextInt();
        int k = scanner.nextInt();

        int[][] a = new int[n][m];
        int[][] b = new int[m][p];
        int[][] c = new int[n][p];

        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                a[i][j] = scanner.nextInt();

        for (int i = 0; i < m; i++)
            for (int j = 0; j < p; j++)
                b[i][j] = scanner.nextInt();

        for (int i = 0; i < n; i++)
            for (int j = 0; j < p; j++)
            {
                c[i][j] = 0;
                for (int x = 0; x < m; x++)
                    if ((x + 1) % k == 0) c[i][j] -= a[i][x] * b[x][j];
                    else c[i][j] += a[i][x] * b[x][j];
            }

        for (int i = 0; i < p; i++)
        {
            for (int j = 0; j < n; j++)
                System.out.print(c[j][i] + " ");
            System.out.println();
        }
    }
}
