import numpy as np
import time
from typing import List, Tuple, Optional
from numba import njit
from pathlib import Path
import tomli


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
    'begin': (3, 6, 0, 2),
    'middle': (2, 3, 0, 6),
    'end': (1, 1, 8, 1)
}

RWEIGHT_BOARD = np.array([
            [-8, 0,  -4, -1, -1, -4, 0,  -8],
            [0,  -2, -4, -2, -2, -4, -2, 0],
            [-4, -4, -2, -2, -2, -2, -4, -4],
            [-1, -2, -2, 0,  0,  -2, -2, -1],
            [-1, -2, -2, 0,  0,  -2, -2, -1],
            [-4, -4, -2, -2, -2, -2, -4, -4],
            [0,  -2, -4, -2, -2, -4, -2, 0],
            [-8, 0,  -4, -1, -1, -4, 0,  -8],
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
        r = row + dr; c = col + dc
        if r < 0 or r >= n or c < 0 or c >= n or board[r, c] != -color:
            continue
        count = 0
        while 0 <= r < n and 0 <= c < n and board[r, c] == -color:
            count += 1
            r += dr; c += dc
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
def nb_has_any_valid_move(chessboard, color) -> bool:
    for r, c in nb_get_possible_moves(chessboard, color):
        ok, flips = nb_is_valid_move(chessboard, r, c, color)
        if ok and flips.shape[0] > 0:
            return True
    return False

@njit()
def nb_count_legal_moves(chessboard, color) -> int:
    cnt = 0
    for r, c in nb_get_possible_moves(chessboard, color):
        ok, flips = nb_is_valid_move(chessboard, r, c, color)
        if ok and flips.shape[0] > 0:
            cnt += 1
    return cnt

@njit()
def nb_flip_inplace(board, flips, color):
    for i in range(flips.shape[0]):
        r = flips[i, 0]
        c = flips[i, 1]
        board[r, c] = color

@njit()
def nb_candidates_with_flips_csr(board, color):
    possible = nb_get_possible_moves(board, color)
    K = possible.shape[0]
    cand_tmp = np.empty((K, 2), dtype=np.int16)
    valid = 0
    total = 0
    for i in range(K):
        r = possible[i, 0]; c = possible[i, 1]
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
        r = possible[i, 0]; c = possible[i, 1]
        ok, flips = nb_is_valid_move(board, r, c, color)
        if ok and flips.shape[0] > 0:
            k = flips.shape[0]
            flips_buf[write_ptr:write_ptr + k, :] = flips
            write_ptr += k
            vi += 1
            offsets[vi] = write_ptr
    return moves, flips_buf, offsets

@njit()
def nb_get_stable_disk(chessboard) -> int:
    n = chessboard.shape[0]
    stable = np.zeros((n, n), dtype=np.uint16)

    corners_r = np.array([0, 0, n - 1, n - 1], dtype=np.int16)
    corners_c = np.array([0, n - 1, 0, n - 1], dtype=np.int16)

    for i in range(4):
        cr = int(corners_r[i]); cc = int(corners_c[i])
        color = int(chessboard[cr, cc])
        if color == 0:
            continue
        for j in range(3):
            dr = int(CORNER_DIRS[i, j, 0]); dc = int(CORNER_DIRS[i, j, 1])
            r = cr; c = cc
            while 0 <= r < n and 0 <= c < n and int(chessboard[r, c]) == color:
                stable[r, c] = 1
                r += dr; c += dc

    s = int(np.sum(chessboard.astype(np.int16) * stable.astype(np.int16)))
    return s


class AI(object):
    def __init__(self, chessboard_size, color, time_out=4.9, config_path=None):
        self.chessboard_size = chessboard_size
        # You are white or black
        self.color = color
        # the max time you should use, your algorithm's run
        # time must not exceed the time limit.
        self.time_out = time_out
        # You need to add your decision to your candidate_list.
        # The system will get the end of your candidate_list as your decision.
        self.candidate_list = []

        self.huristic_weights = HURISTIC_WEIGHTS.copy()
        self.rweight_board = RWEIGHT_BOARD.copy()

        # Load custom weights from config if provided
        if config_path is not None:
            self.load_weights_from_config(config_path)

        self.max_depth = 64
        self.pv_moves: List[Optional[Tuple[int,int]]] = [None] * (self.max_depth)

        global _NUMBA_WARMED_UP
        if not _NUMBA_WARMED_UP:
            try:
                self._warmup_numba()
            except Exception:
                pass
            _NUMBA_WARMED_UP = True
    
    def load_weights_from_config(self, config_path):
        if tomli is None:
            raise ImportError("tomli package required for config loading. Install with: pip install tomli")
        
        config_file = Path(config_path)
        if not config_file.exists():
            raise FileNotFoundError(f"Config file not found: {config_path}")
        
        with open(config_file, 'rb') as f:
            config = tomli.load(f)

        self.huristic_weights = {
            'begin': tuple(config['HURISTIC_WEIGHTS']['begin']),
            'middle': tuple(config['HURISTIC_WEIGHTS']['middle']),
            'end': tuple(config['HURISTIC_WEIGHTS']['end'])
        }
        self.rweight_board = np.array([
            config['RWEIGHT_BOARD'][f'row{i}'] for i in range(8)
        ], dtype=np.int16)
            
    def _warmup_numba(self):
        b = np.zeros((8, 8), dtype=np.int16)
        b[3, 3] = COLOR_WHITE; b[4, 4] = COLOR_WHITE
        b[3, 4] = COLOR_BLACK; b[4, 3] = COLOR_BLACK

        for color in (COLOR_BLACK, COLOR_WHITE):
            _ = nb_get_possible_moves(b, color)
            _ = nb_count_legal_moves(b, color)
            moves, flips_buf, offsets = nb_candidates_with_flips_csr(b, color)
            if moves.shape[0] > 0:
                r0 = int(moves[0, 0]); c0 = int(moves[0, 1])
                ok, flips = nb_is_valid_move(b, r0, c0, color)
                if ok and flips.shape[0] > 0:
                    b[r0, c0] = color
                    nb_flip_inplace(b, flips, color)
                    nb_flip_inplace(b, flips, -color)
                    b[r0, c0] = COLOR_NONE
        _ = nb_get_stable_disk(b)

    def get_candidate_reversed_list(self, chessboard, color) -> Tuple[List[Tuple[int, int]], List[np.ndarray]]:
        moves, flips_buf, offsets = nb_candidates_with_flips_csr(chessboard, color)
        k = moves.shape[0]
        candidate_list: List[Tuple[int,int]] = [(int(moves[i, 0]), int(moves[i, 1])) for i in range(k)]
        reversed_list: List[np.ndarray] = [flips_buf[offsets[i]:offsets[i+1], :] for i in range(k)]
        return candidate_list, reversed_list
        
    def evaluate(self, chessboard, isterminal, me_color) -> float:
        board_rel = chessboard.astype(np.int16) * np.int16(me_color)
        weighted_rel = float(np.sum(self.rweight_board * board_rel))

        mobility_me = nb_count_legal_moves(chessboard, me_color)
        mobility_opp = nb_count_legal_moves(chessboard, -me_color)
        mobility_rel = float(mobility_opp - mobility_me)

        stable_abs = nb_get_stable_disk(chessboard)
        stable_rel = float(stable_abs * me_color)

        piece_abs = int(np.sum(chessboard))
        piece_rel = float(piece_abs * me_color)

        if isterminal:
            if piece_rel < 0:
                return INFINITY
            elif piece_rel > 0:
                return -INFINITY
            else:
                return 0.0

        piece_count = int(np.sum(chessboard != COLOR_NONE))
        if piece_count <= 20:
            w1, w2, w3, w4 = self.huristic_weights['begin']
        elif piece_count <= 40:
            w1, w2, w3, w4 = self.huristic_weights['middle']
        else:
            w1, w2, w3, w4 = self.huristic_weights['end']

        score = (w1 * weighted_rel) + (-w2 * stable_rel) + (-w3 * piece_rel) + (w4 * mobility_rel)
        return score

    def minimax_search(self, chessboard, depth_limit, deadline) -> Tuple[float, Tuple[int, int], bool, List[Tuple[int,int]]]:
        def time_exceeded() -> bool:
            return time.time() > deadline

        def reorder_with_previous(cands, revs, depth):
            move = self.previous_moves[depth] if depth < len(self.previous_moves) else None
            if move is None or not cands:
                return
            try:
                i = cands.index(move)
                if i != 0:
                    cands[0], cands[i] = cands[i], cands[0]
                    revs[0],  revs[i]  = revs[i],  revs[0]
            except ValueError:
                pass

        def max_value(board, depth, alpha, beta, curr_color) -> Tuple[float, Optional[Tuple[int, int]], bool, List[Tuple[int,int]]]:
            if time_exceeded():
                return self.evaluate(board, False, curr_color), None, True, []
            if depth == depth_limit:
                return self.evaluate(board, False, curr_color), None, False, []

            candidate_list, reversed_list = self.get_candidate_reversed_list(board, curr_color)
            reorder_with_previous(candidate_list, reversed_list, depth)
            if len(candidate_list) > 1:
                pairs = [(candidate_list[i], reversed_list[i]) for i in range(1, len(candidate_list))]
                pairs.sort(key=lambda x: (x[1].shape[0], -int(self.rweight_board[x[0][0], x[0][1]])))
                candidate_list = [candidate_list[0]] + [p[0] for p in pairs]
                reversed_list  = [reversed_list[0]]  + [p[1] for p in pairs]

            if not candidate_list:
                opp_cands, _ = self.get_candidate_reversed_list(board, -curr_color)
                if not opp_cands:
                    return self.evaluate(board, True, curr_color), None, False, []
                v_child, _, timed_out, child_pv = min_value(board, depth + 1, -beta, -alpha, -curr_color)
                return -v_child, None, timed_out, child_pv

            best, move = -INFINITY, None
            best_pv: List[Tuple[int,int]] = []
            a = alpha
            for candidate, reversed_opponents in zip(candidate_list, reversed_list):
                if time_exceeded():
                    return (best if move is not None else self.evaluate(board, False, curr_color)), move, True, best_pv

                r0, c0 = candidate
                board[r0, c0] = curr_color
                k = reversed_opponents.shape[0]
                if k:
                    nb_flip_inplace(board, reversed_opponents, curr_color)

                v_child, _, timed_out, child_pv = min_value(board, depth + 1, -beta, -a, -curr_color)

                if k:
                    nb_flip_inplace(board, reversed_opponents, -curr_color)
                board[r0, c0] = COLOR_NONE

                if timed_out:
                    return (best if move is not None else self.evaluate(board, False, curr_color)), move, True, best_pv

                v = -v_child
                if v > best:
                    best, move = v, candidate
                    best_pv = [candidate] + child_pv

                if best > a:
                    a = best
                if a >= beta:
                    break
            return best, move, False, best_pv

        def min_value(board, depth, alpha, beta, curr_color) -> Tuple[float, Optional[Tuple[int, int]], bool, List[Tuple[int,int]]]:
            if time_exceeded():
                return self.evaluate(board, False, curr_color), None, True, []
            if depth == depth_limit:
                return self.evaluate(board, False, curr_color), None, False, []

            candidate_list, reversed_list = self.get_candidate_reversed_list(board, curr_color)
            reorder_with_previous(candidate_list, reversed_list, depth)
            if len(candidate_list) > 1:
                pairs = [(candidate_list[i], reversed_list[i]) for i in range(1, len(candidate_list))]
                pairs.sort(key=lambda x: (x[1].shape[0], -int(self.rweight_board[x[0][0], x[0][1]])))
                candidate_list = [candidate_list[0]] + [p[0] for p in pairs]
                reversed_list  = [reversed_list[0]]  + [p[1] for p in pairs]

            if not candidate_list:
                opp_cands, _ = self.get_candidate_reversed_list(board, -curr_color)
                if not opp_cands:
                    return self.evaluate(board, True, curr_color), None, False, []
                v_child, _, timed_out, child_pv = max_value(board, depth + 1, -beta, -alpha, -curr_color)
                return -v_child, None, timed_out, child_pv

            best, move = -INFINITY, None
            best_pv: List[Tuple[int,int]] = []
            a = alpha
            for candidate, reversed_opponents in zip(candidate_list, reversed_list):
                if time_exceeded():
                    return (best if move is not None else self.evaluate(board, False, curr_color)), move, True, best_pv

                r0, c0 = candidate
                board[r0, c0] = curr_color
                k = reversed_opponents.shape[0]
                if k:
                    nb_flip_inplace(board, reversed_opponents, curr_color)

                v_child, _, timed_out, child_pv = max_value(board, depth + 1, -beta, -a, -curr_color)

                if k:
                    nb_flip_inplace(board, reversed_opponents, -curr_color)
                board[r0, c0] = COLOR_NONE

                if timed_out:
                    return (best if move is not None else self.evaluate(board, False, curr_color)), move, True, best_pv

                v = -v_child
                if v > best:
                    best, move = v, candidate
                    best_pv = [candidate] + child_pv

                if best > a:
                    a = best
                if a >= beta:
                    break
            return best, move, False, best_pv

        return max_value(chessboard, 0, -INFINITY, INFINITY, self.color)

    def iterative_deepening(self, chessboard, start_time) -> Optional[Tuple[int,int]]:
        best_move = None
        self.previous_moves: List[Optional[Tuple[int,int]]] = [None] * self.max_depth
        deadline = start_time + min(self.time_out, 4.9)

        for depth in range(1, self.max_depth + 1):
            if time.time() > deadline:
                break
            _, move, timed_out, best_pv = self.minimax_search(chessboard, depth, deadline)
            if timed_out:
                break
            if move is not None:
                best_move = move
                self.previous_moves[:len(best_pv)] = best_pv
        return best_move
    
    def go(self, chessboard):
        start_time = time.time()
        self.candidate_list.clear()
        chessboard = chessboard.astype(np.int16, copy=False)
        self.candidate_list, _ = self.get_candidate_reversed_list(chessboard, self.color)

        if not self.candidate_list:         
            return self.candidate_list
        best_move = self.iterative_deepening(chessboard, start_time)
        if best_move is not None:
            self.candidate_list.append(best_move)
        return self.candidate_list
