// ==WindhawkMod==
// @id              auto-alias-folder-menu
// @name            Auto Alias & Pin Folder Menu
// @description     Menambahkan menu klik kanan pada folder untuk membuat alias di C:\Alias dan menyematkannya ke Quick Access.
// @version         1.2
// @author          enjunglipret
// @github          https://github.com/zulfakar404/auto-alias-folder-menu
// @include         explorer.exe
// ==/WindhawkMod==

// ==WindhawkModReadme==
/*
Mod ini menambahkan opsi "Buat Alias & Pin ke Quick Access" pada klik kanan folder di Windows Explorer.
Junction alias akan otomatis disimpan di dalam folder C:\Alias.
*/
// ==/WindhawkModReadme==

#include <windows.h>
#include <string>

const HKEY HKEY_ROOT = HKEY_CURRENT_USER;
const LPCWSTR REG_PATH_SHELL = L"Software\\Classes\\Directory\\shell\\WindhawkAutoAlias";
const LPCWSTR REG_PATH_CMD = L"Software\\Classes\\Directory\\shell\\WindhawkAutoAlias\\command";

BOOL Wh_ModInit() {
    HKEY hKeyShell = NULL;
    HKEY hKeyCmd = NULL;
    
    if (RegCreateKeyExW(HKEY_ROOT, REG_PATH_SHELL, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKeyShell, NULL) == ERROR_SUCCESS) {
        LPCWSTR menuText = L"Buat Alias & Pin ke Quick Access";
        RegSetValueExW(hKeyShell, NULL, 0, REG_SZ, (const BYTE*)menuText, (wcslen(menuText) + 1) * sizeof(WCHAR));
        
        LPCWSTR iconPath = L"imageres.dll,-315"; 
        RegSetValueExW(hKeyShell, L"Icon", 0, REG_SZ, (const BYTE*)iconPath, (wcslen(iconPath) + 1) * sizeof(WCHAR));

        if (RegCreateKeyExW(hKeyShell, L"command", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKeyCmd, NULL) == ERROR_SUCCESS) {
            LPCWSTR cmdText = L"powershell.exe -WindowStyle Hidden -NoProfile -Command \"Add-Type -AssemblyName Microsoft.VisualBasic; $a = [Microsoft.VisualBasic.Interaction]::InputBox('Masukkan nama alias folder:', 'Buat Alias'); if (![string]::IsNullOrWhiteSpace($a)) { $t = '%1'; $p = 'C:\\Alias'; if (!(Test-Path $p)) { New-Item -ItemType Directory -Path $p -Force | Out-Null }; $l = Join-Path $p $a; Start-Process cmd.exe -ArgumentList ('/c mklink /J \\\"{0}\\\" \\\"{1}\\\"' -f $l, $t) -Verb RunAs -WindowStyle Hidden -Wait; $s = New-Object -ComObject Shell.Application; $f = $s.Namespace($p); $i = $f.ParseName($a); if ($i) { $i.InvokeVerb('pintohome') } }\"";
            RegSetValueExW(hKeyCmd, NULL, 0, REG_SZ, (const BYTE*)cmdText, (wcslen(cmdText) + 1) * sizeof(WCHAR));
            RegCloseKey(hKeyCmd);
        }
        RegCloseKey(hKeyShell);
    }
    return TRUE;
}

void Wh_ModUninit() {
    RegDeleteKeyW(HKEY_ROOT, REG_PATH_CMD);
    RegDeleteKeyW(HKEY_ROOT, REG_PATH_SHELL);
}
