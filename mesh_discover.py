#!/usr/bin/env python3
import sys, os, random
try:
    from master_cipher import NODE_ID
except ImportError:
    NODE_ID = "0000X0000"

def lookup():
    nodes = ["Node_Alpha", "Node_Beta"]
    for _ in range(random.randint(2, 5)):
        nodes.append(f"Node_iPad_{random.randint(10,99)}")
    return len(nodes), ", ".join(nodes)

if __name__ == "__main__":
    count, peers = lookup()
    print(f"ID:{NODE_ID} | PEERS:{count} | SEEDS:[{peers}]")
