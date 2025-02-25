import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        int n = scanner.nextInt();
        while (n > 0)
        {
            int x = scanner.nextInt();
            System.out.println(x % 10);
            n --;
        }
    }
}
