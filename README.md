# WinFilter
Winlogon and LSA Notification Password Filters 

## Reference
The primary code for each of the filters were pulled from:
- https://github.com/gtworek/PSBits/tree/master/PasswordStealing
- https://github.com/3gstudent/PasswordFilter

## Usage
### Filters
Modify the IP and Port that the filters will call back to in the `kindtime_key.ps1` script. 

Modify and utilize each of the filter's PowerShell install scripts in order to set up the filters. 

`sed -i 's/password/your_dll_name/g' install_script.ps1`

### Credentials Receiver
To receive creds and set up the server, run:

`python3 winfilter.py`

By default, the IP will be '0.0.0.0' and the port will be '80'. You can give a specific ip or port using `--ip <ip_addr>` or `--port <port_num>`.

Screenshot Example:
```python3 winfilter.py --port 6006```

![photo](photos/photo.png)

Credentials will also be written to a file in the `creds` directory. Use the `--clean` option to clear out the directory.

### Sharing Creds
Credentials are written to the `creds` directory. Start a Python HTTP server in this directory, and have other use the command below.

`curl <red_ip>:8000/<blue_ip>`

Discord Webhook functionality is also available. Uncomment the `discordWH` function call in the `winfilter.py` file and modify the url variable.
