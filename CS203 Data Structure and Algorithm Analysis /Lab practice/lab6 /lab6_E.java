import java.util.ArrayList;
import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        int n = scanner.nextInt();
        node[] nodes = new node[n + 1];

        for (int i=1; i<=n; i++)
            nodes[i] = new node(i);
        int [] vv = new int[n+1];
        int ans = 0;

        for (int i=1; i<n; i++)
        {
            int u = scanner.nextInt();
            int v = scanner.nextInt();

            nodes[u].sons.add(nodes[v]);
            nodes[v].sons.add(nodes[u]);
        }

        int m = scanner.nextInt();
        for (int i=0; i<m; i++)
            vv[i] = scanner.nextInt();

        nodes[1].path = -1;
        nodes[1].isvisited = true;
        for (int j = 0; j < nodes[1].sons.size(); j++)
        {
            node[] q = new node[n + 1];
            int front=0, rear=0;
            q[rear ++] = nodes[1].sons.get(j);
            nodes[1].sons.get(j).path = 0;
            nodes[1].sons.get(j).isvisited = true;
            while (front != rear) {
                node head = q[front++];
                for (int i = 0; i < head.sons.size(); i++)
                {
                    if (head.sons.get(i).isvisited) continue;
                    q[rear++] = head.sons.get(i);
                    head.sons.get(i).path = head.path + 1;
                    head.sons.get(i).isvisited = true;
                }
            }
            int k = 0;
            int [] a = new int[n+1];
            for (int i=0; i<m; i++)
            {
                if (nodes[vv[i]].isvisited) {
                    k ++;
                    a[k] = nodes[vv[i]].path;
                    nodes[vv[i]].isvisited = false;
                }
            }

            mergesort(a, 1, k);

            for (int i=2; i<=k; i++)
                if (a[i-1] > 0 && a[i] <= a[i - 1]) a[i] = a[i - 1] + 1;

            if (a[k] + 1 > ans) ans = a[k] + 1;
        }

        System.out.println(ans);
    }

    public static void merge(int[] a, int low, int mid, int high) {
        int[] b = new int[high - low + 2];
        int i = low, j = mid + 1, k = 1;

        while (i <= mid && j <= high) {
            if (a[i] <= a[j]) {
                b[k] = a[i];
                k++;
                i++;
            } else {
                b[k] = a[j];
                k++;
                j++;
            }
        }

        while (i <= mid) {
            b[k] = a[i];
            k++;
            i++;
        }

        while (j <= high) {
            b[k] = a[j];
            k++;
            j++;
        }

        k = 0;
        for (int m = low; m <= high; m++) {
            k++;
            a[m] = b[k];
        }
    }

    public static void mergesort(int[] a, int low, int high) {
        if (low < high) {
            int mid = (low + high) / 2;
            mergesort(a, low, mid);
            mergesort(a, mid + 1, high);
            merge(a, low, mid, high);
        }
    }
}


class node {
    ArrayList<node> sons = new ArrayList<>();
    int path;
    boolean isvisited;

    public node(int val)
    {
        this.path = 0;
        this.isvisited = false;
    }
}
