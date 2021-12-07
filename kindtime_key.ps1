New-Item -Path "HKLM:\Software\WOW6432Node\kindtime" -Force
New-ItemProperty -Path "HKLM:\Software\WOW6432Node\kindtime" -Name "ipa" -PropertyType String -Value "192.168.1.1" -Force
New-ItemProperty -Path "HKLM:\Software\WOW6432Node\kindtime" -Name "pt" -PropertyType DWORD -Value 80 -Force

$ethIP = (Get-NetIpAddress -AddressFamily IPv4 -InterfaceAlias "Ethernet0").IPAddress
New-ItemProperty -Path "HKLM:\Software\WOW6432Node\kindtime" -Name "eip" -PropertyType String -Value $ethIP -Force
