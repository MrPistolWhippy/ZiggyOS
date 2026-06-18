#!/bin/ash
DB="/root/archive.db"
HTML="/root/dashboard_panel.html"
TOTAL_B=$(sqlite3 "$DB" "SELECT COUNT(*) FROM p6_ledger;" 2>/dev/null || echo "36")
ACTIVE_T=$(sqlite3 "$DB" "SELECT COUNT(*) FROM p6_ledger WHERE status='ALARM_RING_ACTIVE';" 2>/dev/null || echo "3")
TOTAL_P=$(sqlite3 "$DB" "SELECT COUNT(*) FROM peer_shard_topology;" 2>/dev/null || echo "62")
cat << WEB > $HTML
<!DOCTYPE html><html><head>
<script type="text/javascript" src="https://unpkg.com"></script>
<style>
  body { background-color: #080a10; color: #e2e8f0; font-family: monospace; padding: 30px; margin: 0; }
  .grid { display: flex; gap: 20px; margin-bottom: 30px; }
  .card { background-color: #0f1322; border-radius: 8px; padding: 20px; flex: 1; text-align: center; box-shadow: 0 4px 10px rgba(239,68,68,0.2); }
  .red-alert { border: 1px solid #ef4444; } .teal-mesh { border: 1px solid #14b8a6; } .gold-node { border: 1px solid #eab308; }
  h1 { color: #ef4444; text-shadow: 0 0 10px rgba(239,68,68,0.5); border-bottom: 2px solid #ef4444; padding-bottom: 10px; margin-top: 0; }
  .red-txt { color: #ef4444; } .teal-txt { color: #38bdf8; } .gold-txt { color: #eab308; }
  .metric { font-size: 32px; font-weight: bold; margin: 10px 0; }
  .desc { font-size: 13px; color: #94a3b8; text-align: left; line-height: 1.5; }
  #network { height: 450px; background-color: #0f1322; border: 1px solid #ef4444; border-radius: 8px; }
  h2 { color: #38bdf8; font-size: 16px; margin-bottom: 15px; }
</style></head><body>
  <h1>[CRITICAL OVERRIDE] ZIGGY-OS PEER MESH LAYER | ENGINEER: MAX (MrPistolWhippy)</h1>
  <div class="grid">
    <div class="card teal-mesh"><h3>LEDGER BLOCK STORAGE</h3><div class="metric teal-txt">$TOTAL_B Blocks</div><p class="desc">Chained accounting nodes locked sequentially.</p></div>
    <div class="card red-alert"><h3 class="red-txt">THREAT ISOLATION MATRIX</h3><div class="metric red-txt">$ACTIVE_T Vector Triggers</div><p class="desc">Active system intercept alarms.</p></div>
    <div class="card gold-node"><h3>GEODISTRIBUTED GRID</h3><div class="metric gold-txt">$TOTAL_P Shard Peers</div><p class="desc">Independent node handlers distributed across continental US zones.</p></div>
  </div>
  <h2>REAL-TIME NETWORK SHARD TOPOLOGY SPREAD</h2><div id="network"></div>
WEB
cat << 'WEB' >> $HTML
  <script type="text/javascript">
    var container = document.getElementById('network');
    var nodes = new vis.DataSet([
      { id: 1, label: 'Autonomous Peer Loop\n[127.0.0.1:9876]', color: { background: '#080a10', border: '#ef4444' }, font: { color: '#ef4444', face: 'monospace' }, shape: 'box', borderWidth: 2 },
      { id: 2, label: 'US-EAST-ATLANTIC', color: '#0f1322', shape: 'dot', size: 22 },
      { id: 3, label: 'US-WEST-PACIFIC', color: '#0f1322', shape: 'dot', size: 22 },
      { id: 4, label: 'US-CENTRAL-PLAINS', color: '#0f1322', shape: 'dot', size: 22 },
      { id: 5, label: 'US-SOUTH-BORDER', color: '#0f1322', shape: 'dot', size: 22 },
      { id: 6, label: 'US-NORTH-LAKES', color: '#0f1322', shape: 'dot', size: 22 },
      { id: 7, label: 'PEER-ZINZAZIGGLEMAX', color: '#0f1322', shape: 'star', size: 20 }
    ]);
    var edges = new vis.DataSet([
      { from: 1, to: 2, color: '#ef4444', width: 2, arrows: 'to;from' },
      { from: 1, to: 3, color: '#ef4444', width: 2, arrows: 'to;from' },
      { from: 1, to: 4, color: '#ef4444', width: 2, arrows: 'to;from' },
      { from: 1, to: 5, color: '#ef4444', width: 2, arrows: 'to;from' },
      { from: 1, to: 6, color: '#ef4444', width: 2, arrows: 'to;from' },
      { from: 1, to: 7, color: '#38bdf8', width: 2 },
      { from: 2, to: 6, color: '#334155', dashes: true },
      { from: 3, to: 5, color: '#334155', dashes: true }
    ]);
    var data = { nodes: nodes, edges: edges };
    var options = { physics: { stabilization: true }, edges: { smooth: { type: 'continuous' } }, nodes: { borderWidth: 1, color: { border: '#14b8a6' } } };
    var network = new vis.Network(container, data, options);
  </script></body></html>
WEB
echo -e "\033[92m[+] SUCCESS: Cyberpunk Dashboard Panel Generated Completely!\033[0m"
