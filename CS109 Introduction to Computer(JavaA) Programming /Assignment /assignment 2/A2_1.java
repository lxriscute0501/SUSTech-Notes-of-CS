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
        
        int num = (a[m] == -1 ? 1 : 0) + (b[m] == -1 ? 1 : 0);
        System.out.println(num);
    }
}
