import java.util.Scanner;

public class Lab5E5 {
    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        int T = in.nextInt();    //get the number of test cases
        int[][] sudoku = new int[9][9];

        Outer:
        while (T-- > 0) {
            boolean isValid = true;

            //Get input and check if there are invalid numbers
            for (int r = 0; r < 9; r++) {
                for (int c = 0; c < 9; c++) {
                    sudoku[r][c] = in.nextInt();
                    if(sudoku[r][c] > 9 || sudoku[r][c] < 1) {
                        System.out.println("No");//isValid = false;
                        continue Outer;
                    }
                }
            }

            //check if there are more than two same number in a row or a column
            for(int i = 0; i < 9; i++){
                if(!cheeckDuplicate(sudoku, i, 0) || !cheeckDuplicate(sudoku, i, 1)) {
                    System.out.println("No");//isValid = false;
                    continue Outer;
                }
            }

            //check if there are more than two same number in a sub-square
            for(int i=0;i<9;i=i+3){
                for(int j=0;j<9;j=j+3){
                    if(!cheeckSub(sudoku,i,j)){
                        System.out.println("No");//isValid = false;
                        continue Outer;
                    }
                }
            }

            System.out.println("Yes");
        }

    }
    /**
     * to check if rows or columns are duplicate
     *
     * @param sudoku sudoku you to be checked
     * @param k column or row index
     * @param rc 0 to check row, 1 to check column
     * @return if duplicate return false, else return true
     */
    public static boolean cheeckDuplicate(int[][] sudoku, int k, int rc){
        int[] existNum = new int[9];
        for(int i = 0; i < 9; i++){
            int current = (rc == 0) ?  sudoku[k][i] : sudoku[i][k];
            existNum[current-1]++;
        }
        int product=1;
        for(int i = 0; i < 9; i++){
            product*=existNum[i];
        }
        if(product==1)
            return true;
        return false;
    }

    public static boolean cheeckSub(int[][] sudoku, int i, int j){
        int[] existNum = new int[9];
        for (int r = 0; r < 3; r++) {
            for (int c = 0; c < 3; c++) {
                existNum[sudoku[i+r][j+c]-1]++;
            }
        }
        int product=1;
        for(int k = 0; k < 9; k++){
            product*=existNum[k];
        }
        if(product==1)
            return true;
        return false;
    }



}
