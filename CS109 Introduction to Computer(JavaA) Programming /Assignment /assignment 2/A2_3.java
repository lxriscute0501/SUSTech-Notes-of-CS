import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        int m = scanner.nextInt();
        int n = scanner.nextInt();
        int[][] a = new int[15][55];

        for (int i = 1; i <= m; i++)
            for (int j = 1; j <= n; j++)
                a[i][j] = scanner.nextInt();

        int x = scanner.nextInt();
        int y = scanner.nextInt();

        if (a[x][y] != -1) {
            System.out.println(a[x][y]);
            System.exit(0);
        }

        double lcoe = 0, rcoe = 0, upcoe = 0, downcoe = 0;
        boolean bol = y > 1 && a[x][y - 1] != -1;
        boolean bor = y < n && a[x][y + 1] != -1;
        boolean boup = x > 1 && a[x - 1][y] != -1;
        boolean bodown = x < m && a[x + 1][y] != -1;

        if (bol && bor && boup && bodown) {
            lcoe = 0.3;
            rcoe = 0.3;
            upcoe = 0.2;
            downcoe = 0.2;
        } else if (boup && bodown && bol == !bor) {
            lcoe = 0.3;
            rcoe = 0.3;
            upcoe = 0.35;
            downcoe = 0.35;
        } else if (bol && bor && boup == !bodown) {
            lcoe = 0.4;
            rcoe = 0.4;
            upcoe = 0.2;
            downcoe = 0.2;
        } else if (bol == !bor && boup == !bodown) {
            lcoe = 0.6;
            rcoe = 0.6;
            upcoe = 0.4;
            downcoe = 0.4;
        } else if (!bol && !bor && boup && bodown) {
            upcoe = 0.5;
            downcoe = 0.5;
        } else if (bol && bor && !boup && !bodown) {
            lcoe = 0.4;
            rcoe = 0.6;
        } else {
            System.out.println("DATA CANNOT BE REPAIRED");
            return;
        }

        double sum = 0;
        if (bol) sum += lcoe * a[x][y - 1];
        if (bor) sum += rcoe * a[x][y + 1];
        if (boup) sum += upcoe * a[x - 1][y];
        if (bodown) sum += downcoe * a[x + 1][y];

        int num = (int) sum;
        System.out.println(num);
    }
}
