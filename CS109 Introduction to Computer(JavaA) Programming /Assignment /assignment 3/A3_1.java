import java.util.Scanner;

public class Main {

    public static int numbers(String ss) {
        int n = ss.length(), p = 1, sum = 0;
        for (int i = 0; i < n; i++)
        {
            if (i * 2 + 1 == n) continue;
            sum += (ss.charAt(i) - '0') * p;
            p *= 10;
        }
        return sum;
    }

    public static int letters(String ss) {
        int n = ss.length(), sum = 0;
        for (int i = 0; i < n; i++)
        {
            if (i * 2 + 1 == n) continue;
            sum += ss.charAt(i);
        }
        return sum;
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        String s = scanner.next();
        int ans = 0;
        String ss = "";
        ss += s.charAt(0);
        boolean bo = s.charAt(0) >= '0' && s.charAt(0) <= '9';

        for (int i = 1; i < s.length(); i++)
        {
            if (s.charAt(i) >= '0' && s.charAt(i) <= '9' && !bo) {
                ans += letters(ss);
                ss = "" + s.charAt(i);
                bo = true;
                continue;
            }

            if (!(s.charAt(i) >= '0' && s.charAt(i) <= '9') && bo) {
                ans += numbers(ss);
                ss = "" + s.charAt(i);
                bo = false;
                continue;
            }

            ss += s.charAt(i);
        }

        if (bo) ans += numbers(ss); else ans += letters(ss);
        System.out.println(ans);
    }
}
