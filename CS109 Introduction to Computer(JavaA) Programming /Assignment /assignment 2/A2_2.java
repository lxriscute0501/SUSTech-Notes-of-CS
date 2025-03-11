import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        int n, m;
        int[] a = new int[51];
        int[] b = new int[51];

        n = scanner.nextInt();
        for (int i = 1; i <= n; i++)
            a[i] = scanner.nextInt();

        for (int i = 1; i <= n; i++)
            b[i] = scanner.nextInt();

        m = scanner.nextInt();
        if (n > 1) {
            if (a[m] == -1 && a[m + 1] != -1 && m == 1) {
                a[1] = a[2] * 4 / 5;
            } else if (a[m] == -1 && a[m - 1] != -1 && m == n) {
                a[m] = a[m - 1] * 4 / 5;
            } else if (a[m] == -1 && m >= 1 && m < n) {
                if (a[m - 1] != -1 && a[m + 1] != -1) a[m] = (a[m - 1] * 2 + a[m + 1] * 3) / 5;
            }

            if (b[m] == -1 && b[m + 1] != -1 && m == 1) {
                b[1] = b[2] * 4 / 5;
            } else if (b[m] == -1 && b[m - 1] != -1 && m == n) {
                b[m] = b[m - 1] * 4 / 5;
            } else if (b[m] == -1 && m >= 1 && m < n) {
                if (b[m - 1] != -1 && b[m + 1] != -1) b[m] = (b[m - 1] * 2 + b[m + 1] * 3) / 5;
            }
        }

        if (a[m] == -1 && b[m] == -1) System.out.printf("ALL DATA LOST");
        else if (a[m] == -1) System.out.printf("CAT DATA LOST");
        else if (b[m] == -1) System.out.printf("DOG DATA LOST");
        else System.out.println(a[m] + b[m]);
    }
}
