import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        int n, m;
        Scanner scanner = new Scanner(System.in);
        n = scanner.nextInt();
        m = scanner.nextInt();
        int a = n / 100, b = (n / 10) % 10, c = n % 10, t;

        if (a > b) {
            t = a; a = b; b = t;
        }
        if (a > c) {
            t = a; a = c; c = t;
        }
        if (b > c) {
            t = b; b = c; c = t;
        }

        if (m == 1) System.out.printf("%d%d%d%n", a, b, c); else System.out.printf("%d%d%d%n", c, b, a);
    }
}
