import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        int t = scanner.nextInt();
        for (int i = 0; i < t; i++) 
        {
            int n = scanner.nextInt();
            node fake = new node(0 , -111);
            node cur = fake;
            for (int j = 0 ; j < n ; j++)
            {
                node temp = new node(scanner.nextInt(), scanner.nextInt());
                cur.next = temp;
                cur = cur.next;
            }
            node tail = new node(0 , 2000000000);
            cur.next = tail;
            cur = fake;
            int m = scanner.nextInt();
            for(int j = 0 ; j < m ; j++)
            {
                int coe = scanner.nextInt();
                int exp = scanner.nextInt();
                while(cur.next.exp <= exp) {
                    cur = cur.next;
                }
                if (cur.exp == exp) cur.coe += coe;
                else {
                    node temp = new node(coe , exp);
                    temp.next = cur.next;
                    cur.next = temp;
                }
            }
            int q = scanner.nextInt();
            cur = fake.next;
            for (int j = 0 ; j < q ; j++)
            {
                int search = scanner.nextInt();
                while(cur.next.exp <= search) {
                    cur = cur.next;
                }
                if (cur.exp == search) {
                    System.out.print(cur.coe +" ");
                }
                else {
                    System.out.print(0 + " ");
                }
            }
        }
    }

}
class node{
    int coe;
    int exp;
    node next;
    public node(int coe , int exp){
        this.coe = coe;
        this.exp = exp;
    }
}
