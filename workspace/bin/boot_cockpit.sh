#!/bin/sh
# 1. Spawn a fresh background tmux layout session titled 'ZIGGYOS'
tmux new-session -d -s ZIGGYOS

# 2. Configure our custom neon cyan, magenta, and yellow color grid styles
tmux set -t ZIGGYOS status-style bg=black,fg=cyan
tmux set -t ZIGGYOS status-left "#[fg=magenta,bold][ZIGGYOS]#[fg=cyan] #[fg=yellow,bold]#S #[fg=cyan]• "
tmux set -t ZIGGYOS window-status-current-style fg=brightwhite,bold,bg=magenta
tmux set -t ZIGGYOS window-status-current-format " #[fg=white]#I:#W* "
tmux set -t ZIGGYOS status-right "#[fg=cyan]• #[fg=yellow,bold]%H:%M:%S #[fg=magenta,bold][DECK-V180.0]"

# 3. Create a clean vertical layout split window pane
tmux split-window -t ZIGGYOS:0 -h

# 4. Automate the active runtime modules to launch inside their targeted windows
tmux send-keys -t ZIGGYOS:0.1 "source /root/workspace/bin/deck_fix.sh && clear && htop" C-m
tmux send-keys -t ZIGGYOS:0.0 "source /root/workspace/bin/deck_fix.sh && master_automate.sh" C-m

# 5. Connect user straight into the operational cockpit matrix interface
tmux attach-session -t ZIGGYOS
