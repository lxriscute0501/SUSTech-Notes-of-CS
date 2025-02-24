import java.util.ArrayList;
import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        int n = scanner.nextInt();
        node[] nodes = new node[n + 1];

        int num = scanner.nextInt();
        for (int i=1; i<=n; i++)
            nodes[i] = new node(i);
        int count;
        count = 0;
        for (int i=1; i<n; i++)
        {
            int u = scanner.nextInt();
            int v = scanner.nextInt();
            int wei = scanner.nextInt();

            nodes[u].sons.add(nodes[v]);
            nodes[u].w.add(wei);
            nodes[v].sons.add(nodes[u]);
            nodes[v].w.add(wei);
        }

        node[] q = new node[n + 1];
        int front=0, rear=0;
        q[rear ++] = nodes[1];
        nodes[1].path = 0;
        nodes[1].isvisited = true;
        while (front != rear) {
            node head = q[front++];
            for (int i = 0; i < head.sons.size(); i++) {
                if (head.sons.get(i).isvisited) continue;
                q[rear++] = head.sons.get(i);
                head.sons.get(i).path = head.path + head.w.get(i);
                head.sons.get(i).isvisited = true;
            }
        }
        for (int i=2; i <= n; i++)
            if (nodes[i].path == num && nodes[i].sons.size() == 1) count ++;
        System.out.println(count);
    }
}


class node {
    ArrayList<node> sons = new ArrayList<>();
    ArrayList<Integer> w = new ArrayList<>();
    int path;
    boolean isvisited;

    public node(int val)
    {
        this.path = 0;
        this.isvisited = false;
    }
}
