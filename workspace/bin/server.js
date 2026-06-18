const http = require('http');
const fs = require('fs');
const path = require('path');
const { exec } = require('child_process');

const server = http.createServer((req, res) => {
    // Enable Cross-Origin Resource Sharing for interactive control blocks
    res.setHeader('Access-Control-Allow-Origin', '*');
    res.setHeader('Access-Control-Allow-Methods', 'GET, POST, OPTIONS');
    res.setHeader('Access-Control-Allow-Headers', 'Content-Type');

    if (req.method === 'OPTIONS') {
        res.writeHead(204);
        return res.end();
    }

    // Serve Interactive HTML Dashboard Panel
    if ((req.url === '/' || req.url === '/dashboard') && req.method === 'GET') {
        res.writeHead(200, { 'Content-Type': 'text/html' });
        return fs.createReadStream('/root/workspace/web/dashboard_panel.html').pipe(res);
    }

    // Core Status JSON API
    if (req.url === '/api/status' && req.method === 'GET') {
        let currentSeed = "0xAB8D";
        try { currentSeed = fs.readFileSync('/tmp/current_seed.txt', 'utf8').trim(); } catch(e){}
        res.writeHead(200, { 'Content-Type': 'application/json' });
        return res.end(JSON.stringify({
            system: "THEOSI ZIGGYOS OMNI-DECK V180.0",
            node_runtime: process.version,
            entropy_matrix_seed: currentSeed,
            workspace_status: "ACTIVE_ISOLATED",
            timestamp: new Date().toISOString()
        }, null, 4));
    }
    
    // Plain-Text Log Stream API
    if (req.url === '/api/logs' && req.method === 'GET') {
        const logPath = '/root/workspace/logs/swarm_cluster.log';
        res.writeHead(200, { 'Content-Type': 'text/plain' });
        if (fs.existsSync(logPath)) {
            return fs.createReadStream(logPath).pipe(res);
        } else {
            return res.end("[*] Log stream empty or initialization pending.");
        }
    }

    // Interactive Action Ingestion Endpoint
    if (req.url === '/api/action' && req.method === 'POST') {
        let body = '';
        req.on('data', chunk => { body += chunk.toString(); });
        req.on('end', () => {
            try {
                const parsed = JSON.parse(body);
                let commandString = '';

                if (parsed.action === 'automate') {
                    commandString = '/root/workspace/bin/master_automate.sh --fast';
                } else if (parsed.action === 'probe') {
                    commandString = '/root/workspace/bin/run_kernel.sh';
                }

                if (commandString) {
                    exec(commandString, (err, stdout, stderr) => {
                        // Append execution output directly to tracking logs so UI updates instantly
                        const logMessage = `\n[WEB_ACT] Triggered: ${parsed.action}\n${stdout || stderr}\n`;
                        fs.appendFileSync('/root/workspace/logs/swarm_cluster.log', logMessage);
                        
                        res.writeHead(200, { 'Content-Type': 'application/json' });
                        res.end(JSON.stringify({ status: "SUCCESS", message: "Command executed cleanly." }));
                    });
                } else {
                    res.writeHead(400, { 'Content-Type': 'application/json' });
                    res.end(JSON.stringify({ status: "ERROR", message: "Invalid action." }));
                }
            } catch (e) {
                res.writeHead(500, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify({ status: "ERROR", message: "Parsing fault." }));
            }
        });
        return;
    }

    res.writeHead(404, { 'Content-Type': 'text/plain' });
    res.end("404: Route out of dashboard workspace boundaries.");
});

server.listen(3000, () => {
    console.log('[+] Interactive Command API Matrix listening on port 3000.');
});
