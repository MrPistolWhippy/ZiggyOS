#!/usr/bin/env python3
import sys
import math

def process_route(msg, key, mode, action):
    cols = key
    if action == "encrypt":
        rows = math.ceil(len(msg) / cols)
        msg += "~" * (rows * cols - len(msg))
        grid = [msg[i:i+cols] for i in range(0, len(msg), cols)]
        cipher_parts = []
        for c in range(cols):
            row_range = reversed(range(rows)) if mode == "inverted" else (range(rows) if (mode != "snake" or c % 2 == 0) else reversed(range(rows)))
            cipher_parts.append("".join(grid[r][c] for r in row_range))
        return "".join(cipher_parts).encode('utf-8').hex()
    else:  # decrypt
        try:
            msg = bytes.fromhex(msg).decode('utf-8')
        except Exception:
            pass
        rows = len(msg) // cols
        grid = [["" for _ in range(cols)] for _ in range(rows)]
        idx = 0
        for c in range(cols):
            row_range = reversed(range(rows)) if mode == "inverted" else (range(rows) if (mode != "snake" or c % 2 == 0) else reversed(range(rows)))
            for r in row_range:
                if idx < len(msg):
                    grid[r][c] = msg[idx]
                    idx += 1
        res = "".join("".join(row) for row in grid)
        # Force decode hex if the internal layer wrapped it
        try:
            if all(ch in "0123456789abcdefABCDEF" for ch in res):
                res = bytes.fromhex(res).decode('utf-8')
        except Exception:
            pass
        return res

if __name__ == "__main__":
    action = sys.argv[1].lower()
    cols_key = int(sys.argv[2])
    route_mode = sys.argv[3].lower()
    text_input = " ".join(sys.argv[4:])
    print(process_route(text_input, cols_key, route_mode, action))
