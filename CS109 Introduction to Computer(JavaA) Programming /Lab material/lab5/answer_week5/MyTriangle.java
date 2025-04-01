import java.util.Scanner;

public class MyTriangle {
    public static void main(String[] args) {
        double a, b, c;
        Scanner in = new Scanner(System.in);
        while (true) {
            System.out.println("Please input three numbers for a, b, c:");
            a = in.nextDouble();
            if (a == -1) {
                System.out.println("Bye~");
                return;
            }
            b = in.nextDouble();
            c = in.nextDouble();
            if (!isValid(a, b, c)) {
                System.out.println("The input is invalid.");
                continue;
            }
            System.out.printf("The area is %.3f\n", MyTriangle.area(a, b, c));
            System.out.printf("The perimeter is %.3f\n", MyTriangle.perimeter(a, b, c));
        }

    }

    public static double area(double a, double b, double c) {
        if (isValid(a, b, c)) {
            double p = 0.5 * (a + b + c);
            return Math.sqrt(p * (p - a) * (p - b) * (p - c));
        } else {
            return -1;
        }
    }

    public static double perimeter(double a, double b, double c) {
        return a + b + c;
    }

    public static boolean isValid(double side1, double side2, double side3) {
        // a+b>c and a+c>b ==> a>0
        return (side1 + side2 > side3) && (side2 + side3 > side1) && (side1 + side3 > side2);
    }

    public static double area(double bottom, double height) {
        return 0.5 * bottom * height;
    }

    public static double area(double a, double b, int angleOfAandB) {
        return 0.5 * a * b * Math.sin(Math.PI / 180 * angleOfAandB);
    }

    public static void displayTriangle(int lineofHeight) {
        for (int i = 1; i <= lineofHeight; ++i) {
            for (int j = 2 * (lineofHeight - i) + 1; j > 0; --j)
                System.out.print(' ');
            for (int j = 2 * i - 1; j > 0; --j)
                System.out.printf("%2d", i);
            System.out.println();
        }
    }

    public static void displayTriangle(int lineofHeight, char ch) {
        for (int i = 1; i <= lineofHeight; ++i) {
            for (int j = 2 * (lineofHeight - i) + 1; j > 0; --j)
                System.out.print(' ');
            for (int j = 2 * i - 1; j > 0; --j)
                System.out.print(" " + ch);
            System.out.println();
        }
    }

}