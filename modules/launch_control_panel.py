import sys, os, time, select
logs = ['[SYSTEM] Link Active.', '[FREQ] 999,999,999,432 Hz.']
def render(t, msg):
    os.system('clear')
    print('================== ZIGGY-OS COMMAND COCKPIT ==================')
    print(f' Ticks: {t:04d} | Freq: 999,999,999,432Hz | Status: ONLINE')
    print('==============================================================')
    for l in logs[-5:]: print(f' {l}')
    print('==============================================================')
    print(f' COMMAND INPUT: {msg}', end='', flush=True)
def main():
    t, buf = 0, ''
    while True:
        t += 1; render(t, buf)
        r, _, _ = select.select([sys.stdin], [], [], 0.5)
        if r:
            c = sys.stdin.read(1)
            if c == '\n':
                cmd = buf.strip()
                if cmd.lower() in ['exit','quit']: break
                logs.append(f'[COMMAND] {cmd}')
                buf = ''
            elif ord(c) == 127: buf = buf[:-1]
            else: buf += c
if __name__ == '__main__': main()
