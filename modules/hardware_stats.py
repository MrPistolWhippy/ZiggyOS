import sys, os, time, platform
try:
    print("=========================================")
    print("      IPAD HARDWARE ENVIRONMENT DATA     ")
    print("=========================================")
    print(f"OS Architecture : {platform.system()} {platform.machine()}")
    print(f"Python Core     : CPython v{platform.python_version()}")
    print(f"System Epoch    : {int(time.time())}")
    
    # Trace execution sandbox layer variables
    user = os.environ.get("USER", "root")
    home = os.environ.get("HOME", "/root")
    print(f"User Privilege  : {user}")
    print(f"Sandbox Root    : {home}")
    
    # Calculate disk usage allocation metrics natively
    st = os.statvfs('/')
    free_gb = (st.f_bavail * st.f_frsize) / (1024**3)
    total_gb = (st.f_blocks * st.f_frsize) / (1024**3)
    print(f"FAT Storage Free: {free_gb:.2f} GB / {total_gb:.2f} GB")
    print("=========================================")
except Exception as e:
    print(f'{{"error": "Hardware readout interrupted: {str(e)}"}}')
