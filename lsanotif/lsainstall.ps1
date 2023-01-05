$dllname = "dllname" # no .dll extension

$path = Get-ItemProperty -Path "HKLM:\SYSTEM\CurrentControlSet\Control\Lsa" -Name "Notification Packages"

if (!$path."Notification Packages".Contains($dllname))
{
    $UpdatedValue = $Path."Notification Packages" + $dllname
    Set-ItemProperty -Path $Path.PSPath -Name "Notification Packages" -Value $UpdatedValue -Force
}

