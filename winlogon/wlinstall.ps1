$dllname = "dllname" # no .dll extension

$path = Get-ItemProperty -Path "HKLM:\SYSTEM\CurrentControlSet\Control\NetworkProvider\Order" -Name PROVIDERORDER

if (!$path.ProviderOrder.Contains($dllname)) # check if script has already been run
{
    $UpdatedValue = $Path.PROVIDERORDER + "," + $dllname
    Set-ItemProperty -Path $Path.PSPath -Name "PROVIDERORDER" -Value $UpdatedValue

    $servicepath1 = "HKLM:\SYSTEM\CurrentControlSet\Services\"+$dllname
    $servicepath2 = "HKLM:\SYSTEM\CurrentControlSet\Services\"+$dllname+"\NetworkProvider"
    $dllpath = "%SystemRoot%\System32\"+$dllname+".dll"

    New-Item -Path $servicepath1
    New-Item -Path $servicepath2
    New-ItemProperty -Path $servicepath2 -Name "Class" -Value 2
    New-ItemProperty -Path $servicepath2 -Name "Name" -Value $dllname
    New-ItemProperty -Path $servicepath2 -Name "ProviderPath" -PropertyType ExpandString -Value $dllpath
} 


