#!/bin/ash
# ZIGGY-OS THREE-PANEL COMMAND CENTER LAYOUT AUTOMATION
SESSION="control_center"

# 1. Force kill any existing background layout structures to prevent conflicts
tmux kill-session -t "$SESSION" 2>/dev/null

# 2. Configure high-visibility teal/cyan window frames and status bar styling profiles
tmux new-session -d -s "$SESSION" '/root/ram_matrix_sensor.sh'
tmux set-option -t "$SESSION" status-bg black
tmux set-option -t "$SESSION" status-fg cyan
tmux set-option -t "$SESSION" pane-border-style fg=brightblack
tmux set-option -t "$SESSION" pane-active-border-style fg=cyan

# 3. Inject the Top Visual Toolbar Layout Line (Shows date, active port, and status flags)
tmux set-option -t "$SESSION" status-left "[Z-OS MESH ACTIVE] "
tmux set-option -t "$SESSION" status-right " | PORT: 8080 | RECOVERY: DETACHED | %H:%M:%S "

# 4. Slice the screen space to build the right-hand stack panels
tmux split-window -h -t "$SESSION:0.0" -p 45 'ash'
tmux split-window -v -t "$SESSION:0.1" -p 50 'ranger /root/'

# 5. Lock focus onto your active interactive command prompt panel and launch
tmux select-pane -t "$SESSION:0.1"
tmux attach-session -t "$SESSION"
