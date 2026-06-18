const http = require('http');
const fs = require('fs');
const path = require('path');

const server = http.createServer((req, res) => {
    res.setHeader('Access-Control-Allow-Origin', '*');
    
    // API Router Hook: Fetch active telemetry profile state
    if (req.url === '/api/status' || req.url === '/') {
        let currentSeed = "0xAB8D";
        try { currentSeed = fs.readFileSync('/tmp/current_seed.txt', 'utf8').trim(); } catch(e){}
        
        res.writeHead(200, { 'Content-Type': 'application/json' });
        return res.end(JSON.stringify({
            system: "THEOSI ZIGGYOS OMNI-DECK V180.0",
            node_runtime: process.version,
            entropy_matrix_seed: currentSeed,
            workspace_status: "ACTIVE_ISOLATED"
        }, null, 4));
    }
    
    // Source Stream Hook: Stream source snapshots directly (e.g., http://192.168.4)
    if (req.url.startsWith('/src/')) {
        const targetFile = path.basename(req.url);
        const targetPath = path.join('/root/workspace/src', targetFile);
        
        if (fs.existsSync(targetPath)) {
            res.writeHead(200, { 'Content-Type': 'text/plain' });
            return fs.createReadStream(targetPath).pipe(res);
        }
    }

    res.writeHead(404, { 'Content-Type': 'text/plain' });
    res.end("404: Node out of workspace boundary bounds.");
});

server.listen(3000, () => {
    console.log('[+] Advanced Workspace API streaming framework active on port 3000.');
});
