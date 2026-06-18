#!/bin/ash
H="/root/dashboard_panel.html"
echo "<!DOCTYPE html><html><head><script src=\"https://unpkg.com\"></script><script src=\"https://tailwindcss.com\"></script></head><body class=\"bg-slate-950 text-cyan-400 p-6\">" > $H
echo "<h1 class=\"text-2xl font-bold border-b border-cyan-500 pb-2 uppercase tracking-widest text-shadow-md\">[BLUE RAY HARMONY] ZIGGY-OS MESH</h1>" >> $H
echo "<p class=\"text-xs text-cyan-200 mt-1\">MASTER ENGINEER: MAX | FREQUENCY: 144.777</p>" >> $H
echo "<main class=\"grid grid-cols-3 gap-4 my-6 text-center\"><div class=\"border border-cyan-500 p-4 rounded bg-slate-900\"><h3>LEDGER</h3><div class=\"text-xl font-bold text-cyan-300\">144 Blocks</div></div><div class=\"border border-cyan-500 p-4 rounded bg-slate-900\"><h3>SCOUTER</h3><div class=\"text-xl font-bold text-red-400\">PL: 144777</div></div><div class=\"border border-cyan-500 p-4 rounded bg-slate-900\"><h3>GLOBAL PEERS</h3><div class=\"text-xl font-bold text-sky-300\">75 Shards</div></div></main>" >> $H
echo "<div id=\"net\" style=\"height:300px;\" class=\"border border-cyan-500 rounded bg-[#0b1329]\"></div>" >> $H
echo "<script>var n=new vis.DataSet([{id:1,label:\"Loop Loop\"},{id:2,label:\"US-EAST\"},{id:3,label:\"PEER-NZ\"},{id:4,label:\"PEER-NL\"}]);var e=new vis.DataSet([{from:1,to:2,color:\"#14b8a6\"},{from:1,to:3,color:\"#38bdf8\"},{from:1,to:4,color:\"#38bdf8\"}]);var net=new vis.Network(document.getElementById(\"net\"),{nodes:n,edges:e},{physics:{stabilization:true}});</script></body></html>\"" >> $H
cp /root/advanced_sql_suite.sh /root/advanced_sql_suite.txt 2>/dev/null
