$path = Get-ItemProperty -Path "HKLM:\SYSTEM\CurrentControlSet\Control\NetworkProvider\Order" -Name PROVIDERORDER
$UpdatedValue = $Path.PROVIDERORDER + ",password"
Set-ItemProperty -Path $Path.PSPath -Name "PROVIDERORDER" -Value $UpdatedValue

New-Item -Path HKLM:\SYSTEM\CurrentControlSet\Services\password
New-Item -Path HKLM:\SYSTEM\CurrentControlSet\Services\password\NetworkProvider
New-ItemProperty -Path HKLM:\SYSTEM\CurrentControlSet\Services\password\NetworkProvider -Name "Class" -Value 2
New-ItemProperty -Path HKLM:\SYSTEM\CurrentControlSet\Services\password\NetworkProvider -Name "Name" -Value password
New-ItemProperty -Path HKLM:\SYSTEM\CurrentControlSet\Services\password\NetworkProvider -Name "ProviderPath" -PropertyType ExpandString -Value "%SystemRoot%\System32\password.dll"
