import numpy as np
import time
from typing import List, Tuple, Optional
from numba import njit

COLOR_BLACK = -1
COLOR_WHITE = 1
COLOR_NONE = 0
INFINITY = float('inf')

_NUMBA_WARMED_UP = False

DIRS = np.array([
    (-1, -1), (-1, 0), (-1, 1),
    (0, -1) ,          (0, 1),
    (1, -1) , (1, 0),  (1, 1)
], dtype=np.int16)

CORNER_DIRS = np.array([
    [[0,  1], [1,  0], [1,  1]],
    [[0, -1], [1,  0], [1, -1]],  
    [[0,  1], [-1, 0], [-1, 1]], 
    [[0, -1], [-1, 0], [-1, -1]], 
], dtype=np.int16)

HURISTIC_WEIGHTS = {
    'begin': (3, 5, 0, 2),
    'middle': (2, 3, 0, 5),
    'end': (1, 1, 7, 1)
}

RWEIGHT_BOARD = np.array([
    [-8, 0, -4, -1, -1, -4, 0, -8],
    [0, -2, -4, -2, -2, -4, -2, 0],
    [-4, -4, -2, -2, -2, -2, -4, -4],
    [-1, -2, -2, 0,  0, -2, -2, -1],
    [-1, -2, -2, 0,  0, -2, -2, -1],
    [-4, -4, -2, -2, -2, -2, -4, -4],
    [0, -2, -4, -2, -2, -4, -2, 0],
    [-8, 0, -4, -1, -1, -4, 0, -8],
], dtype=np.int16)


@njit()
def nb_get_possible_moves(board, color):
    opp = (board == -color)
    neighbor = np.zeros_like(opp)

    neighbor[:-1, :] |= opp[1:, :]
    neighbor[1:,  :] |= opp[:-1, :]
    neighbor[:, :-1] |= opp[:, 1:]
    neighbor[:, 1: ] |= opp[:, :-1]

    neighbor[:-1, :-1] |= opp[1:, 1:]
    neighbor[:-1, 1: ] |= opp[1:, :-1]
    neighbor[1:,  :-1] |= opp[:-1, 1:]
    neighbor[1:,  1: ] |= opp[:-1, :-1]

    possible = neighbor & (board == 0)
    rows, cols = np.nonzero(possible)
    k = rows.shape[0]
    coords = np.empty((k, 2), dtype=np.int16)
    coords[:, 0] = rows
    coords[:, 1] = cols
    return coords


@njit()
def nb_is_valid_move(board, row, col, color):
    n = board.shape[0]
    if board[row, col] != 0:
        return False, np.empty((0, 2), dtype=np.int16)

    flips = np.empty((64, 2), dtype=np.int16)
    idx = 0
    for dr, dc in DIRS:
        r = row + dr
        c = col + dc
        if r < 0 or r >= n or c < 0 or c >= n or board[r, c] != -color:
            continue
        count = 0
        while 0 <= r < n and 0 <= c < n and board[r, c] == -color:
            count += 1
            r += dr
            c += dc
        if count > 0 and 0 <= r < n and 0 <= c < n and board[r, c] == color:
            for t in range(1, count + 1):
                rr = row + dr * t
                cc = col + dc * t
                flips[idx, 0] = rr
                flips[idx, 1] = cc
                idx += 1
    if idx == 0:
        return False, np.empty((0, 2), dtype=np.int16)
    return True, flips[:idx]


@njit()
def nb_count_legal_moves(board, color):
    cnt = 0
    for r, c in nb_get_possible_moves(board, color):
        ok, flips = nb_is_valid_move(board, r, c, color)
        if ok and flips.shape[0] > 0:
            cnt += 1
    return cnt

@njit()
def nb_flip_inplace(board, flips, color):
    for i in range(flips.shape[0]):
        board[flips[i, 0], flips[i, 1]] = color


@njit()
def nb_candidates_with_flips_csr(board, color):
    possible = nb_get_possible_moves(board, color)
    K = possible.shape[0]
    cand_tmp = np.empty((K, 2), dtype=np.int16)
    valid = 0
    total = 0
    for i in range(K):
        r = possible[i, 0]
        c = possible[i, 1]
        ok, flips = nb_is_valid_move(board, r, c, color)
        if ok and flips.shape[0] > 0:
            cand_tmp[valid, 0] = r
            cand_tmp[valid, 1] = c
            total += flips.shape[0]
            valid += 1

    moves = cand_tmp[:valid].copy()
    flips_buf = np.empty((total, 2), dtype=np.int16)
    offsets = np.empty(valid + 1, dtype=np.int32)

    offsets[0] = 0
    write_ptr = 0
    vi = 0

    for i in range(K):
        r = possible[i, 0]
        c = possible[i, 1]
        ok, flips = nb_is_valid_move(board, r, c, color)
        if ok and flips.shape[0] > 0:
            k = flips.shape[0]
            flips_buf[write_ptr:write_ptr + k] = flips
            write_ptr += k
            vi += 1
            offsets[vi] = write_ptr
    return moves, flips_buf, offsets


@njit()
def nb_get_stable_disk(board):
    n = board.shape[0]
    stable = np.zeros((n, n), dtype=np.uint16)

    corners_r = np.array([0, 0, n - 1, n - 1], dtype=np.int16)
    corners_c = np.array([0, n - 1, 0, n - 1], dtype=np.int16)

    for i in range(4):
        cr = corners_r[i]
        cc = corners_c[i]
        color = board[cr, cc]
        if color == 0:
            continue
        for j in range(3):
            dr = CORNER_DIRS[i, j, 0]
            dc = CORNER_DIRS[i, j, 1]
            r = cr
            c = cc
            while 0 <= r < n and 0 <= c < n and board[r, c] == color:
                stable[r, c] = 1
                r += dr
                c += dc

    return int(np.sum(board.astype(np.int16) * stable.astype(np.int16)))

class AI(object):
    def __init__(self, chessboard_size, color, time_out=4.9, config_path=None):
        self.chessboard_size = chessboard_size
        self.color = color
        self.time_out = time_out
        self.candidate_list = []

        self.huristic_weights = HURISTIC_WEIGHTS
        self.rweight_board = RWEIGHT_BOARD

        self.max_depth = 64
        self.pv_moves = [None] * self.max_depth

        global _NUMBA_WARMED_UP
        if not _NUMBA_WARMED_UP:
            try:
                self._warmup_numba()
            except:
                pass
            _NUMBA_WARMED_UP = True

    def load_weights_from_config(self, path):
        return

    def _warmup_numba(self):
        b = np.zeros((8, 8), dtype=np.int16)
        b[3, 3] = COLOR_WHITE
        b[4, 4] = COLOR_WHITE
        b[3, 4] = COLOR_BLACK
        b[4, 3] = COLOR_BLACK

        for color in (COLOR_BLACK, COLOR_WHITE):
            _ = nb_get_possible_moves(b, color)
            _ = nb_count_legal_moves(b, color)
            moves, flips, offs = nb_candidates_with_flips_csr(b, color)
            if moves.shape[0] > 0:
                r0, c0 = moves[0]
                ok, f = nb_is_valid_move(b, r0, c0, color)
                if ok:
                    b[r0, c0] = color
                    nb_flip_inplace(b, f, color)
                    nb_flip_inplace(b, f, -color)
                    b[r0, c0] = COLOR_NONE
        _ = nb_get_stable_disk(b)

    def get_candidate_reversed_list(self, board, color):
        moves, flips_buf, offsets = nb_candidates_with_flips_csr(board, color)
        k = moves.shape[0]
        cand = [(int(moves[i, 0]), int(moves[i, 1])) for i in range(k)]
        revs = [flips_buf[offsets[i]:offsets[i+1]] for i in range(k)]
        return cand, revs

    def evaluate(self, board, is_terminal, me):
        board_rel = board.astype(np.int16) * me
        weighted_rel = float(np.sum(self.rweight_board * board_rel))

        mobility_me = nb_count_legal_moves(board, me)
        mobility_op = nb_count_legal_moves(board, -me)
        mobility_rel = float(mobility_op - mobility_me)

        stable_abs = nb_get_stable_disk(board)
        stable_rel = float(stable_abs * me)

        piece_abs = int(np.sum(board))
        piece_rel = float(piece_abs * me)

        if is_terminal:
            if piece_rel < 0:
                return INFINITY
            elif piece_rel > 0:
                return -INFINITY
            else:
                return 0.0

        tot = int(np.sum(board != 0))
        if tot <= 20:
            w1, w2, w3, w4 = self.huristic_weights['begin']
        elif tot <= 40:
            w1, w2, w3, w4 = self.huristic_weights['middle']
        else:
            w1, w2, w3, w4 = self.huristic_weights['end']

        score = w1 * weighted_rel + (-w2) * stable_rel + (-w3) * piece_rel + (w4) * mobility_rel
        return score
    
    def minimax_search(self, board, depth_limit, deadline):
        def timeout():
            return time.time() > deadline

        def reorder(cands, revs, depth):
            mv = self.pv_moves[depth]
            if mv is None:
                return
            try:
                i = cands.index(mv)
                if i != 0:
                    cands[0], cands[i] = cands[i], cands[0]
                    revs[0], revs[i] = revs[i], revs[0]
            except:
                pass

        def max_value(board, depth, alpha, beta, cur):
            if timeout():
                return self.evaluate(board, False, cur), None, True, []

            if depth == depth_limit:
                return self.evaluate(board, False, cur), None, False, []

            cands, revs = self.get_candidate_reversed_list(board, cur)
            reorder(cands, revs, depth)

            if not cands:
                opp, _ = self.get_candidate_reversed_list(board, -cur)
                if not opp:
                    return self.evaluate(board, True, cur), None, False, []
                v2, _, td, pv = min_value(board, depth + 1, -beta, -alpha, -cur)
                return -v2, None, td, pv

            best, move = -INFINITY, None
            best_pv = []
            a = alpha

            for cand, revflip in zip(cands, revs):
                if timeout():
                    return self.evaluate(board, False, cur), None, True, best_pv

                r, c = cand
                board[r, c] = cur
                if revflip.shape[0] > 0:
                    nb_flip_inplace(board, revflip, cur)

                v2, _, td, pv = min_value(board, depth + 1, -beta, -a, -cur)

                if revflip.shape[0] > 0:
                    nb_flip_inplace(board, revflip, -cur)
                board[r, c] = 0

                if td:
                    return self.evaluate(board, False, cur), None, True, best_pv

                v = -v2
                if v > best:
                    best, move = v, cand
                    best_pv = [cand] + pv

                if best > a:
                    a = best
                if a >= beta:
                    break
            return best, move, False, best_pv

        def min_value(board, depth, alpha, beta, cur):
            return max_value(board, depth, alpha, beta, cur)

        return max_value(board, 0, -INFINITY, INFINITY, self.color)

    def iterative_deepening(self, board, start_time):
        best_move = None
        self.pv_moves = [None] * self.max_depth
        deadline = start_time + min(self.time_out, 4.8)

        for depth in range(1, self.max_depth + 1):
            if time.time() > deadline:
                break
            _, mv, timeout, pv = self.minimax_search(board, depth, deadline)
            if timeout:
                break
            if mv is not None:
                best_move = mv
                self.pv_moves[:len(pv)] = pv
        return best_move

    def go(self, chessboard):
        self.candidate_list.clear()
        board = chessboard.astype(np.int16)

        cands, _ = self.get_candidate_reversed_list(board, self.color)
        self.candidate_list = cands[:]

        if not cands:
            return cands

        mv = self.iterative_deepening(board, time.time())
        if mv is not None:
            self.candidate_list.append(mv)

        return self.candidate_list
