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

// FNV-1a 64-bit hash for a stable mutex name derived from the kanata.exe full
// path.
static unsigned long long HashPath64(const wchar_t *s) {
  unsigned long long h = 14695981039346656037ull;
  while (*s) {
    h ^= (unsigned long long)(unsigned short)(*s++);
    h *= 1099511628211ull;
  }
  return h;
}

static void BuildMutexName(const wchar_t *exePath, wchar_t *out, size_t cap) {
  unsigned long long h = HashPath64(exePath);
  swprintf_s(out, cap, L"Local\\kanata-launcher-%016llx", h);
}

int WINAPI wWinMain(HINSTANCE, HINSTANCE, PWSTR, int) {
  wchar_t dir[MAX_PATH];
  GetSelfDir(dir, _countof(dir));

  // Absolute paths (same directory as launcher)
  wchar_t exe[MAX_PATH];
  wchar_t cfg[MAX_PATH];
  swprintf_s(exe, _countof(exe), L"%s\\kanata.exe", dir);
  swprintf_s(cfg, _countof(cfg), L"%s\\kanata.kbd", dir);

  // Idempotency gate: if already running, do nothing.
  HANDLE hGate = nullptr;
  {
    wchar_t mname[64];
    BuildMutexName(exe, mname, _countof(mname));
    hGate = CreateMutexW(nullptr, FALSE, mname);
    if (hGate) {
      if (GetLastError() == ERROR_ALREADY_EXISTS) {
        CloseHandle(hGate);
        return 0;
      }
      // Let kanata.exe inherit this handle, so the mutex persists after
      // launcher exits.
      SetHandleInformation(hGate, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);
    }
  }

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
      nullptr, nullptr,
      (hGate != nullptr) ? TRUE
                         : FALSE,  // inherit gate handle if we created it
      flags, nullptr,
      dir,  // lpCurrentDirectory
      &si, &pi);

  if (hGate) CloseHandle(hGate);

  if (ok) {
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    return 0;
  }
  // Silent failure by design (no logs/popups as requested).
  return 1;
}
