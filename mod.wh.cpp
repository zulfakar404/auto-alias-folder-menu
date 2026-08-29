// ==WindhawkMod==
// @id              auto-alias-folder-menu
// @name            Auto Alias & Pin Folder Menu
// @description     Menambahkan menu klik kanan pada folder untuk membuat alias dan menu file untuk membuat shortcut Start Menu.
// @version         1.4
// @author          enjunglipret
// @github          https://github.com/zulfakar404/auto-alias-folder-menu
// @include         explorer.exe
// ==/WindhawkMod==

// ==WindhawkModReadme==
/*
Mod ini menambahkan opsi "Create Alias Folder & Pin to Quick Access" pada klik kanan folder,
dan "Create Start Menu Shortcut" pada klik kanan file.
*/
// ==/WindhawkModReadme==

#include <windows.h>
#include <string>

const HKEY HKEY_ROOT = HKEY_CURRENT_USER;
const LPCWSTR REG_PATH_SHELL_DIR = L"Software\\Classes\\Directory\\shell\\WindhawkAutoAlias";
const LPCWSTR REG_PATH_CMD_DIR = L"Software\\Classes\\Directory\\shell\\WindhawkAutoAlias\\command";
const LPCWSTR REG_PATH_SHELL_FILE = L"Software\\Classes\\*\\shell\\WindhawkStartMenuShortcut";
const LPCWSTR REG_PATH_CMD_FILE = L"Software\\Classes\\*\\shell\\WindhawkStartMenuShortcut\\command";

BOOL Wh_ModInit() {
    HKEY hKeyShellDir = NULL;
    HKEY hKeyCmdDir = NULL;
    
    if (RegCreateKeyExW(HKEY_ROOT, REG_PATH_SHELL_DIR, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKeyShellDir, NULL) == ERROR_SUCCESS) {
        LPCWSTR menuTextDir = L"Create Alias Folder && Pin to Quick Access";
        RegSetValueExW(hKeyShellDir, NULL, 0, REG_SZ, (const BYTE*)menuTextDir, (wcslen(menuTextDir) + 1) * sizeof(WCHAR));
        
        LPCWSTR iconPathDir = L"imageres.dll,-5100"; 
        RegSetValueExW(hKeyShellDir, L"Icon", 0, REG_SZ, (const BYTE*)iconPathDir, (wcslen(iconPathDir) + 1) * sizeof(WCHAR));

        if (RegCreateKeyExW(hKeyShellDir, L"command", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKeyCmdDir, NULL) == ERROR_SUCCESS) {
            LPCWSTR cmdTextDir = L"powershell.exe -WindowStyle Hidden -NoProfile -Command \"Add-Type -AssemblyName Microsoft.VisualBasic; $a = [Microsoft.VisualBasic.Interaction]::InputBox('Masukkan nama alias folder:', 'Buat Alias'); if (![string]::IsNullOrWhiteSpace($a)) { $t = '%1'; $p = 'C:\\Alias'; if (!(Test-Path $p)) { New-Item -ItemType Directory -Path $p -Force | Out-Null }; $l = Join-Path $p $a; Start-Process cmd.exe -ArgumentList ('/c mklink /J \\\"{0}\\\" \\\"{1}\\\"' -f $l, $t) -Verb RunAs -WindowStyle Hidden -Wait; $s = New-Object -ComObject Shell.Application; $f = $s.Namespace($p); $i = $f.ParseName($a); if ($i) { $i.InvokeVerb('pintohome') } }\"";
            RegSetValueExW(hKeyCmdDir, NULL, 0, REG_SZ, (const BYTE*)cmdTextDir, (wcslen(cmdTextDir) + 1) * sizeof(WCHAR));
            RegCloseKey(hKeyCmdDir);
        }
        RegCloseKey(hKeyShellDir);
    }

    HKEY hKeyShellFile = NULL;
    HKEY hKeyCmdFile = NULL;
    
    if (RegCreateKeyExW(HKEY_ROOT, REG_PATH_SHELL_FILE, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKeyShellFile, NULL) == ERROR_SUCCESS) {
        LPCWSTR menuTextFile = L"Create Start Menu Shortcut";
        RegSetValueExW(hKeyShellFile, NULL, 0, REG_SZ, (const BYTE*)menuTextFile, (wcslen(menuTextFile) + 1) * sizeof(WCHAR));
        
        LPCWSTR iconPathFile = L"imageres.dll,-5100"; 
        RegSetValueExW(hKeyShellFile, L"Icon", 0, REG_SZ, (const BYTE*)iconPathFile, (wcslen(iconPathFile) + 1) * sizeof(WCHAR));

        if (RegCreateKeyExW(hKeyShellFile, L"command", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKeyCmdFile, NULL) == ERROR_SUCCESS) {
            LPCWSTR cmdTextFile = L"powershell.exe -WindowStyle Hidden -NoProfile -Command \"Start-Process powershell.exe -ArgumentList '-NoProfile -WindowStyle Hidden -Command \\\"$s=(New-Object -COM WScript.Shell).CreateShortcut(''C:\\ProgramData\\Microsoft\\Windows\\Start Menu\\Programs\\''+(Get-Item ''%1'').BaseName+''.lnk'');$s.TargetPath=''%1'';$s.Save()\\\"' -Verb RunAs\"";
            RegSetValueExW(hKeyCmdFile, NULL, 0, REG_SZ, (const BYTE*)cmdTextFile, (wcslen(cmdTextFile) + 1) * sizeof(WCHAR));
            RegCloseKey(hKeyCmdFile);
        }
        RegCloseKey(hKeyShellFile);
    }
    return TRUE;
}

void Wh_ModUninit() {
    RegDeleteKeyW(HKEY_ROOT, REG_PATH_CMD_DIR);
    RegDeleteKeyW(HKEY_ROOT, REG_PATH_SHELL_DIR);
    RegDeleteKeyW(HKEY_ROOT, REG_PATH_CMD_FILE);
    RegDeleteKeyW(HKEY_ROOT, REG_PATH_SHELL_FILE);
}
