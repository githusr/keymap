/*
cl /O2 /GL /EHsc /DUNICODE /D_UNICODE kanata_launcher.cpp `
  /link /SUBSYSTEM:WINDOWS /LTCG /OPT:REF /OPT:ICF /INCREMENTAL:NO `
  /OUT:kanata-launcher.exe; `
  if ($LASTEXITCODE -eq 0) { Remove-Item -Force kanata_launcher.obj }

Usage:
  Put kanata-launcher.exe in the SAME directory as:
    - kanata.exe      (non-GUI/console build)
    - kanata.kbd      (your config)
  Run kanata-launcher.exe (e.g., via Task Scheduler “At log on”).
*/

#include <wchar.h>
#include <windows.h>

static void GetSelfDir(wchar_t *dir, size_t cap) {
  DWORD n = GetModuleFileNameW(nullptr, dir, (DWORD)cap);
  if (n == 0 || n >= cap) {
    dir[0] = L'.';
    dir[1] = L'\0';
    return;
  }
  wchar_t *p = wcsrchr(dir, L'\\');
  if (p) *p = L'\0';
}

int WINAPI wWinMain(HINSTANCE, HINSTANCE, PWSTR, int) {
  wchar_t dir[MAX_PATH];
  GetSelfDir(dir, _countof(dir));

  // Absolute paths (same directory as launcher)
  wchar_t exe[MAX_PATH];
  wchar_t cfg[MAX_PATH];
  swprintf_s(exe, _countof(exe), L"%s\\kanata.exe", dir);
  swprintf_s(cfg, _countof(cfg), L"%s\\kanata.kbd", dir);

  // Ensure predictable relative-path behavior (if kanata config references
  // relative files).
  SetCurrentDirectoryW(dir);

  // Mutable command line buffer required by CreateProcessW.
  // Include the quoted exe path as argv[0] to avoid edge-case argv parsing
  // differences.
  wchar_t cmd[2 * MAX_PATH + 64];
  swprintf_s(cmd, _countof(cmd), L"\"%s\" --cfg \"%s\"", exe, cfg);

  STARTUPINFOW si{};
  si.cb = sizeof(si);
  PROCESS_INFORMATION pi{};

  const DWORD flags = CREATE_NO_WINDOW | DETACHED_PROCESS;

  BOOL ok = CreateProcessW(
      exe,  // lpApplicationName (absolute path; avoids PATH searching)
      cmd,  // lpCommandLine (mutable)
      nullptr, nullptr, FALSE, flags, nullptr,
      dir,  // lpCurrentDirectory
      &si, &pi);

  if (ok) {
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    return 0;
  }
  // Silent failure by design (no logs/popups as requested).
  return 1;
}
