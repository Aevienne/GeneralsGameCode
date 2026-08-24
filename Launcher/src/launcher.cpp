#include <windows.h>
#include <string>
#include <vector>

static std::wstring GetExeDir() {
    wchar_t buf[MAX_PATH];
    GetModuleFileNameW(nullptr, buf, MAX_PATH);
    wchar_t* slash = wcsrchr(buf, L'\\');
    if (slash) *slash = 0;
    return buf;
}

static bool FileExists(const std::wstring& p) {
    DWORD a = GetFileAttributesW(p.c_str());
    return a != INVALID_FILE_ATTRIBUTES && !(a & FILE_ATTRIBUTE_DIRECTORY);
}

static bool Is64BitOS() {
    BOOL wow = FALSE;
    if (IsWow64Process(GetCurrentProcess(), &wow)) return wow != FALSE;
    SYSTEM_INFO si{}; GetNativeSystemInfo(&si);
    return si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64
        || si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_ARM64;
}

static std::wstring Join(const std::wstring& dir, const std::wstring& file) {
    if (dir.empty()) return file;
    if (dir.back() == L'\\' || dir.back() == L'/') return dir + file;
    return dir + L"\\" + file;
}

int WINAPI wWinMain(HINSTANCE, HINSTANCE, PWSTR cmdLine, int) {
    std::wstring gameDir = GetExeDir();
    bool want64 = false;
    bool force32 = wcsstr(cmdLine, L"-x86") != nullptr;
    bool force64 = wcsstr(cmdLine, L"-x64") != nullptr;
    if (force64) want64 = true;
    else if (force32) want64 = false;
    else want64 = false;

    std::wstring exe64a = Join(gameDir, L"x64\\generalszh.exe");
    std::wstring exe64b = Join(gameDir, L"generalszh_x64.exe");
    std::wstring exe32  = Join(gameDir, L"generalszh.exe");

    std::wstring target;
    if (want64) {
        if (FileExists(exe64a)) target = exe64a;
        else if (FileExists(exe64b)) target = exe64b;
        else if (FileExists(exe32)) target = exe32;
    } else {
        if (FileExists(exe32)) target = exe32;
    }
    if (target.empty()) {
        if (FileExists(exe64a)) target = exe64a;
        else if (FileExists(exe64b)) target = exe64b;
        else if (FileExists(exe32)) target = exe32;
    }
    if (target.empty()) {
        MessageBoxW(nullptr, L"Could not find generalszh.exe.\nExpected x64\\generalszh.exe or generalszh.exe next to launcher.", L"Generals Launcher", MB_ICONERROR);
        return 1;
    }

    std::wstring args;
    if (cmdLine && *cmdLine) {
        args = L"\"";
        args += target;
        args += L"\" ";
        args += cmdLine;
    } else {
        args = L"\"";
        args += target;
        args += L"\" -win -noFPSLimit";
    }

    STARTUPINFOW si{}; si.cb = sizeof(si);
    PROCESS_INFORMATION pi{};
    std::vector<wchar_t> cmdBuf(args.begin(), args.end());
    cmdBuf.push_back(0);

    BOOL ok = CreateProcessW(target.c_str(), cmdBuf.data(), nullptr, nullptr, FALSE, 0, nullptr, gameDir.c_str(), &si, &pi);
    if (!ok) {
        wchar_t msg[512];
        wsprintfW(msg, L"Failed to launch %s\nError %lu", target.c_str(), GetLastError());
        MessageBoxW(nullptr, msg, L"Generals Launcher", MB_ICONERROR);
        return 1;
    }
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    return 0;
}
