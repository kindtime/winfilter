$ipa = "192.168.1.1" # change
$pt = 80 # change

New-Item -Path "HKLM:\Software\WOW6432Node\kindtime" -Force
New-ItemProperty -Path "HKLM:\Software\WOW6432Node\kindtime" -Name "ipa" -PropertyType String -Value $ipa -Force
New-ItemProperty -Path "HKLM:\Software\WOW6432Node\kindtime" -Name "pt" -PropertyType DWORD -Value $pt -Force

# $ethIP = (Get-NetIpAddress -AddressFamily IPv4 -InterfaceAlias "Ethernet0").IPAddress  # old method
$ethIP = (Get-NetIPAddress -AddressFamily ipv4).ipaddress | select-object -first 1 # new method
New-ItemProperty -Path "HKLM:\Software\WOW6432Node\kindtime" -Name "eip" -PropertyType String -Value $ethIP -Force
