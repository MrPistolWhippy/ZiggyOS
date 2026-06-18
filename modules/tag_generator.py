#!/usr/bin/env python3
import sys, hashlib
print(hashlib.sha256(" ".join(sys.argv[1:]).encode() if len(sys.argv)>1 else b"ZIGGYOS_DEFAULT").hexdigest())
