import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        int[] dx = {0, 1, 0, -1};
        int[] dy = {1, 0, -1, 0};

        Scanner scanner = new Scanner(System.in);
        int n = scanner.nextInt();
        int m = scanner.nextInt();

        char[][] a = new char[101][101];
        char[][] b = new char[101][101];
        boolean[][] va = new boolean[101][101];
        boolean[][] vb = new boolean[101][101];

        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
            {
                va[i][j] = true;
                vb[i][j] = true;
            }

        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                a[i][j] = scanner.next().charAt(0);

        String sold = scanner.next();
        String snew = scanner.next();
        String ans = new String();
        int x = 0, y = 0, dir = 0;
        ans += a[0][0];
        va[0][0] = false;

        for (int i = 1; i < n * m; i++)
        {
            if (x + dx[dir] < 0 || x + dx[dir] >= n || y + dy[dir] < 0 || y + dy[dir] >= m || !va[x + dx[dir]][y + dy[dir]]) dir = (dir + 1) % 4;
            x += dx[dir]; y += dy[dir];
            ans += a[x][y];
            va[x][y] = false;
        }

        for (int i = 0; i < n * m; i++)
        {
            int t = 0;
            while (t < sold.length() && i + t < ans.length() && ans.charAt(i + t) == sold.charAt(t)) t ++;
            if (t == sold.length()) ans = ans.substring(0, i) + snew + ans.substring(i + t);
        }

        x = 0; y = 0; dir = 0;
        b[0][0] = ans.charAt(0);
        vb[0][0] = false;

        for (int i = 1; i < n * m; i++)
        {
            if (x + dx[dir] < 0 || x + dx[dir] >= n || y + dy[dir] < 0 || y + dy[dir] >= m || !vb[x + dx[dir]][y + dy[dir]]) dir = (dir + 1) % 4;
            x += dx[dir]; y += dy[dir];
            b[x][y] = ans.charAt(i);
            vb[x][y] = false;
        }

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
                System.out.print(b[i][j] + " ");
            System.out.println();
        }
    }
}
