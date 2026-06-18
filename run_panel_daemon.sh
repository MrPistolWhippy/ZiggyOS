#!/bin/sh
while true; do
    python3 fix_dashboard.py > /dev/null 2>&1
    bash ./bin/build_dash.sh > /dev/null 2>&1
    sleep 60
done
