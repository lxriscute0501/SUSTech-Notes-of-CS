import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        char ch = scanner.next().charAt(0);
        double temp = scanner.nextDouble();
        double cnt = 99999999;
        int ans = 0;

        if (ch == 'C') {
            temp = temp * 1.80 + 32;
        } else if (ch == 'K') {
            temp = (temp - 273.15) * 1.80 + 32;
        }

        int n = scanner.nextInt();
        for (int i = 1; i <= n; i++)
        {
            char inputCh = scanner.next().charAt(0);
            double x = scanner.nextDouble();

            if (inputCh == 'C') {
                x = x * 1.80 + 32;
            } else if (inputCh == 'K') {
                x = (x - 273.15) * 1.80 + 32;
            }

            if (Math.abs(x - temp) < cnt) {
                cnt = Math.abs(x - temp);
                ans = i;
            }
        }

        System.out.println(ans);
    }
}
