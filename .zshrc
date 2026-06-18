alias cleartrack='pkill -f /dev/location && free -h'
alias runmesh='pkill -f location; pkill -f load_balancer; cat /dev/location > /dev/null & python3 load_balancer.py & python3 -m http.server 8080'
