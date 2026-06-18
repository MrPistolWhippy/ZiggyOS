const http = require('http');
const fs = require('fs');

const server = http.createServer((req, res) => {
    let currentSeed = "0x0000";
    try {
        currentSeed = fs.readFileSync('/tmp/current_seed.txt', 'utf8').trim();
    } catch (e) {
        currentSeed = "0xAB8D"; 
    }

    res.writeHead(200, { 
        'Content-Type': 'application/json',
        'Access-Control-Allow-Origin': '*'
    });
    res.end(JSON.stringify({
        system: "THEOSI ZIGGYOS OMNI-DECK V180.0",
        node_runtime: process.version,
        entropy_matrix_seed: currentSeed,
        sandbox_partition: "ACTIVE",
        timestamp: new Date().toISOString()
    }, null, 4));
});

server.listen(3000, () => {
    console.log('[+] Node.js API stream broadcasting live entropy on port 3000.');
});
