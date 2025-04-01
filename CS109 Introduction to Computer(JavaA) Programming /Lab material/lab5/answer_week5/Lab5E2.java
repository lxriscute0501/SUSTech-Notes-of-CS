import java.util.InputMismatchException;
import java.util.Scanner;

public class Lab6E2 {

    public static void main(String[] args) {
        try (Scanner in = new Scanner(System.in)) {
            System.out.println("Please input two numbers for bottom and height:");
            double bottom = in.nextDouble();
            double height = in.nextDouble();
            System.out.printf("The area is %.3f\n", MyTriangle.area(bottom, height));
            System.out.println("Please input two numbers for a and b:");
            double a = in.nextDouble();
            double b = in.nextDouble();
            System.out.println("Please input a number in (0, 180) for angle (angle is a float variable):");
            int angle = Math.round(in.nextFloat());
            System.out.printf("The area is %.3f\n", MyTriangle.area(a, b, angle));
        } catch (InputMismatchException e) {
            System.err.println("Invalid input :(");
        }
    }

}