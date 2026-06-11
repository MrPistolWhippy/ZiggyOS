import sys,select;print("[*] Booting ZiggyOS Live!");W=sys.stdin;O=sys.stdout;[O.write(W.read(1)) or O.flush() for _ in iter(lambda:select.select([W],[],[],0.05),[]) if _]
