import os
import struct
import fcntl

TUNSETIFF = 0x400454ca
IFF_TUN   = 0x0001
IFF_NO_PI = 0x1000

def init_satellite_tunnel(interface_name="sat0"):
    try:
        tun_fd = os.open('/dev/net/tun', os.O_RDWR)
        ifr = struct.pack('16sH', interface_name.encode('utf-8'), IFF_TUN | IFF_NO_PI)
        fcntl.ioctl(tun_fd, TUNSETIFF, ifr)
        print(f"[+] Bare-Metal Interface '{interface_name}' Socket Online.")
        return tun_fd
    except PermissionError:
        print("[-] Root privileges required to bind raw network sockets.")
    except FileNotFoundError:
        print("[-] Virtual device node missing. Ensure you are on native Linux hardware.")

init_satellite_tunnel()
