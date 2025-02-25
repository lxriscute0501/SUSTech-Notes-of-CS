import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        int left = scanner.nextInt();
        int right = scanner.nextInt();
        for (int i = left; i <= right; i++)
        {
            int sum = 0;
            for (int j = 1; j <= i/2; j++)
            {
                if (i % j == 0) sum += j;
                if (sum > i) break;
            }
            if (sum == i) {
                System.out.println(i);
                System.exit(0);
            }
        }
        System.out.println(-1);
    }
}
