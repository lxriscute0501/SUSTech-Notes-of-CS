#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

int domino_num = 0;

void printBoard(const vector<vector<int>>& board) {
    for (const auto& row : board)
    {
        for (int val : row)
            if (val == -1) cout << "X\t"; else cout << val << "\t";
        cout << "\n";
    }
}

void placeDomino(vector<vector<int>>& board, int row1, int col1, int row2, int col2, int row3, int col3) {
    domino_num ++;
    board[row1][col1] = domino_num;
    board[row2][col2] = domino_num;
    board[row3][col3] = domino_num;
}

void coverBoard(vector<vector<int>>& board, int size, int top_row, int left_col, int missing_row, int missing_col) {
    if (size == 2) {
        domino_num ++;
        for (int i = top_row; i < top_row + 2; i++)
            for (int j = left_col; j < left_col + 2; j++)
                if (i != missing_row or j != missing_col) board[i][j] = domino_num;
        return;
    }

    int half = size / 2;

    int quadrant;
    if (missing_row < top_row + half and missing_col < left_col + half) {
        quadrant = 1;
    } else if (missing_row < top_row + half and missing_col >= left_col + half) {
        quadrant = 2;
    } else if (missing_row >= top_row + half and missing_col < left_col + half) {
        quadrant = 3;
    } else quadrant = 4;

    if (quadrant != 1) {
        board[top_row + half - 1][left_col + half - 1] = domino_num;
    }
    if (quadrant != 2) {
        board[top_row + half - 1][left_col + half] = domino_num;
    }
    if (quadrant != 3) {
        board[top_row + half][left_col + half - 1] = domino_num;
    }
    if (quadrant != 4) {
        board[top_row + half][left_col + half] = domino_num;
    }
    
    domino_num ++;

    if (quadrant == 1) {
        coverBoard(board, half, top_row, left_col, missing_row, missing_col);
    } else {
        coverBoard(board, half, top_row, left_col, top_row + half - 1, left_col + half - 1);
    }

    if (quadrant == 2) {
        coverBoard(board, half, top_row, left_col + half, missing_row, missing_col);
    } else {
        coverBoard(board, half, top_row, left_col + half, top_row + half - 1, left_col + half);
    }

    if (quadrant == 3) {
        coverBoard(board, half, top_row + half, left_col, missing_row, missing_col);
    } else {
        coverBoard(board, half, top_row + half, left_col, top_row + half, left_col + half - 1);
    }

    if (quadrant == 4) {
        coverBoard(board, half, top_row + half, left_col + half, missing_row, missing_col);
    } else {
        coverBoard(board, half, top_row + half, left_col + half, top_row + half, left_col + half);
    }
}

int main()
{
    int n;
    cin >> n;

    int size = pow(2, n);
    vector<vector<int>> board(size, vector<int>(size, 0));
    int missing_row, missing_col;
    cin >> missing_row >> missing_col;
    
    board[missing_row][missing_col] = -1;
    coverBoard(board, size, 0, 0, missing_row, missing_col);
    printBoard(board);
    
    return 0;
}
