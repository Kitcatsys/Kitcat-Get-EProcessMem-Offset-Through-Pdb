#include "Main.h"

using namespace std;

//define

#define PHNT_MODE PHNT_MODE_USER 
#define PHNT_VERSION PHNT_WIN11 

#define NT_SUCCESS(Status)  (((NTSTATUS)(Status)) >= 0)
#define NtCurrentProcess() ((HANDLE)(LONG_PTR)-1)

#pragma comment(lib,"ntdll.lib")
#pragma comment(lib,"wininet.lib")
#pragma comment(lib, "C:\\Program Files\\Microsoft Visual Studio\\18\\Community\\DIA SDK\\lib\\amd64\\diaguids.lib")
#pragma comment(lib,"setupapi.lib")

// global ntapi function init
extern "C"
{
    LPVOID GetPEB();
    LPVOID GetLDR();
}

extern "C"
_Kernel_entry_
NTSYSCALLAPI
NTSTATUS
NTAPI
Sys_NtQuerySystemInformation(
    _In_ UINT SyscallId,
    _In_ SYSTEM_INFORMATION_CLASS SystemInformationClass,
    _Out_writes_bytes_opt_(SystemInformationLength) PVOID SystemInformation,
    _In_ ULONG SystemInformationLength,
    _Out_opt_ PULONG ReturnLength
);

extern "C"
_Must_inspect_result_
_When_(return == 0, __drv_allocatesMem(mem))
_Kernel_entry_
NTSYSCALLAPI
NTSTATUS
NTAPI
Sys_NtAllocateVirtualMemory(
    _In_ DWORD SyscallId,
    _In_ HANDLE ProcessHandle,
    _Inout_ _At_(*BaseAddress, _Readable_bytes_(*RegionSize) _Writable_bytes_(*RegionSize) _Post_readable_byte_size_(*RegionSize)) PVOID * BaseAddress,
    _In_ ULONG_PTR ZeroBits,
    _Inout_ PSIZE_T RegionSize,
    _In_ ULONG AllocationType,
    _In_ ULONG PageProtection
);

extern "C"
_Kernel_entry_
NTSYSCALLAPI
NTSTATUS
NTAPI
Sys_NtClose(
    _In_ DWORD SyscallId,
    _In_ _Post_ptr_invalid_ HANDLE Handle
);

extern "C"
_Kernel_entry_
NTSYSCALLAPI
NTSTATUS
NTAPI
Sys_NtQueryInformationProcess(
    _In_ DWORD SyscallId,
    _In_ HANDLE ProcessHandle,
    _In_ PROCESSINFOCLASS ProcessInformationClass,
    _Out_writes_bytes_(ProcessInformationLength) PVOID ProcessInformation,
    _In_ ULONG ProcessInformationLength,
    _Out_opt_ PULONG ReturnLength
);

extern "C"
_Kernel_entry_
NTSYSCALLAPI
NTSTATUS
NTAPI
Sys_NtReadVirtualMemory(
    _In_ DWORD SyscallId,
    _In_ HANDLE ProcessHandle,
    _In_opt_ PVOID BaseAddress,
    _Out_writes_bytes_to_(NumberOfBytesToRead, *NumberOfBytesRead) PVOID Buffer,
    _In_ SIZE_T NumberOfBytesToRead,
    _Out_opt_ PSIZE_T NumberOfBytesRead
);

extern "C"
_Kernel_entry_
NTSYSCALLAPI
NTSTATUS
NTAPI
Sys_NtFreeVirtualMemory(
    _In_ DWORD SyscallId,
    _In_ HANDLE ProcessHandle,
    _Inout_ __drv_freesMem(Mem) PVOID * BaseAddress,
    _Inout_ PSIZE_T RegionSize,
    _In_ ULONG FreeType
);

extern "C"
_Kernel_entry_
NTSYSCALLAPI
NTSTATUS
NTAPI
Sys_NtCreateFile(
    _In_ DWORD SyscallId,
    _Out_ PHANDLE FileHandle,
    _In_ ACCESS_MASK DesiredAccess,
    _In_ PCOBJECT_ATTRIBUTES ObjectAttributes,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_opt_ PLARGE_INTEGER AllocationSize,
    _In_ ULONG FileAttributes,
    _In_ ULONG ShareAccess,
    _In_ ULONG CreateDisposition,
    _In_ ULONG CreateOptions,
    _In_reads_bytes_opt_(EaLength) PVOID EaBuffer,
    _In_ ULONG EaLength
);

extern "C"
_Kernel_entry_
NTSYSCALLAPI
NTSTATUS
NTAPI
Sys_NtReadFile(
    _In_ DWORD SyscallId,
    _In_ HANDLE FileHandle,
    _In_opt_ HANDLE Event,
    _In_opt_ PIO_APC_ROUTINE ApcRoutine,
    _In_opt_ PVOID ApcContext,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _Out_writes_bytes_(Length) PVOID Buffer,
    _In_ ULONG Length,
    _In_opt_ PLARGE_INTEGER ByteOffset,
    _In_opt_ PULONG Key
);

extern "C"
_Kernel_entry_
NTSYSCALLAPI
NTSTATUS
NTAPI
Sys_NtQueryInformationFile(
    _In_ DWORD SyscallId,
    _In_ HANDLE FileHandle,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _Out_writes_bytes_(Length) PVOID FileInformation,
    _In_ ULONG Length,
    _In_ FILE_INFORMATION_CLASS FileInformationClass
);

extern "C"
_Kernel_entry_
NTSYSCALLAPI
NTSTATUS
NTAPI
Sys_NtWriteFile(
    _In_ DWORD SyscallId,
    _In_ HANDLE FileHandle,
    _In_opt_ HANDLE Event,
    _In_opt_ PIO_APC_ROUTINE ApcRoutine,
    _In_opt_ PVOID ApcContext,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_reads_bytes_(Length) PVOID Buffer,
    _In_ ULONG Length,
    _In_opt_ PLARGE_INTEGER ByteOffset,
    _In_opt_ PULONG Key
);

//global variable init

UINT NtQuerySystemInformation_SyscallId = 0;
UINT NtAllocateVirtualMemory_SyscallId = 0;
UINT NtClose_SyscallId = 0;
UINT NtQueryInformationProcess_SyscallId = 0;
UINT NtReadVirtualMemory_SyscallId = 0;
UINT NtFreeVirtualMemory_SyscallId = 0;
UINT NtCreateFile_SyscallId = 0;
UINT NtReadFile_SyscallId = 0;
UINT NtQueryInformationFile_SyscallId = 0;
UINT NtWriteFile_SyscallId = 0;

UINT ConsoleTimes = 0;
wchar_t FileNtoskrnlPath[] = L"C:\\Windows\\System32\\ntoskrnl.exe";
BOOL g_Init_Or_Not = FALSE;
BOOL _g_f_or_t = TRUE;

ACCESS_MASK g_AlldesiredAccess =
PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION |
PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ;

ACCESS_MASK g_PRdesiredAccess =
PROCESS_QUERY_INFORMATION | PROCESS_VM_READ |
PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_QUERY_LIMITED_INFORMATION;

//global normal function init from cpp header
BOOL g_f_or_t(BOOL bl)
{
    _g_f_or_t = bl;
    return bl ? bl : FALSE;
}
VOID MsgConsole(
    _In_ string text,
    _In_opt_ string text2,
    _In_ int code);

VOID  MsgConsoleEx(
    _In_ string text,
    _In_ int code);

VOID HexMsgConsole(
    _In_ string text,
    _In_opt_ string text2,
    _In_ LPVOID code);

VOID  ErrMsgConsole(
    _In_ string text,
    _In_opt_ string text2,
    _In_ int code);

void MsgConsole(
    _In_ string text,
    _In_opt_ string text2,
    _In_ int code)
{
    std::cout << ConsoleTimes
        << ". Message: " << left
        << setw(75) << text
        << setw(10) << text2
        << "|Code: " << code << "\n\n" << endl;
    ConsoleTimes++;
}

void MsgConsoleEx(
    _In_ string text,
    _In_ int code)
{
    std::cout << ConsoleTimes
        << ". Message: " << left
        << setw(85) << text
        << "|Code: " << code << "\n\n" << endl;
    ConsoleTimes++;
}

void HexMsgConsole(
    _In_ string text,
    _In_opt_ string text2,
    _In_ LPVOID code)
{
    std::cout << ConsoleTimes
        << ". Message: " << left
        << setw(75) << text
        << setw(10) << text2
        << "|Code: 0x" << code << "\n\n" << endl;
    ConsoleTimes++;
}
void ErrMsgConsole(
    _In_ string text,
    _In_opt_ string text2,
    _In_ int code)
{
    cerr << "=========================================================================================" << endl;

    cerr << "\n" << "ERROR!!!\n"
        << "Message: " << left
        << setw(75) << text
        << setw(10) << text2
        << "|Code: " << code << "\n" << endl;
    ConsoleTimes++;
}

string WstringToString(_In_ wstring wstr)
{

    if (wstr.empty()) return string();

    int sizeneeded = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);

    string str(sizeneeded, 0);

    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], sizeneeded, &str[0], sizeneeded, NULL, NULL);

    return str;

}


string MyGetTimeInfo()
{
    time_t now = time(NULL);
    struct tm timeinfo;
    localtime_s(&timeinfo, &now);

    char timeStr[20];
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &timeinfo);
    string str = timeStr;
    return str.c_str();
}

void FileWriteLog(_In_ wstring file, _Inout_ string context)
{
    stringstream ss = {};

    FILE* log = NULL;
    _wfopen_s(&log, file.c_str(), L"a+");
    if (log)
    {
        ss << "[ " << MyGetTimeInfo() << " ]" << " " << context << "\n";
        context = ss.str();
        fprintf(log, context.c_str());
        fclose(log);
    }

}

LONG WINAPI GlobalCrashHandler(_In_opt_ PEXCEPTION_POINTERS Exceptions)
{
    wchar_t desktopPath[MAX_PATH] = {};
    DWORD result = ExpandEnvironmentStringsW(L"%USERPROFILE%\\Desktop", desktopPath, MAX_PATH);
    wstring wstr = desktopPath;
    wstr += L"\\crash_report.log";
    string str = WstringToString(wstr);
    if (result > 0 && result <= MAX_PATH)
    {
        stringstream ss = {};
        ss << "Founded Desktop Path:" << str;
        str = ss.str();
    }
    DWORD code = Exceptions->ExceptionRecord->ExceptionCode;
    PBYTE ExceptionAddr = reinterpret_cast<PBYTE>(Exceptions->ExceptionRecord->ExceptionAddress);

    DWORD64 Rsi = Exceptions->ContextRecord->Rsi;
    DWORD64 Rip = Exceptions->ContextRecord->Rip;
    DWORD64 Rax = Exceptions->ContextRecord->Rax;
    DWORD64 Rbp = Exceptions->ContextRecord->Rbp;
    DWORD64 Rcx = Exceptions->ContextRecord->Rcx;
    DWORD64 Rdx = Exceptions->ContextRecord->Rdx;
    DWORD64 R8 = Exceptions->ContextRecord->R8;
    DWORD64 R9 = Exceptions->ContextRecord->R9;
    DWORD64 R10 = Exceptions->ContextRecord->R10;
    DWORD64 R11 = Exceptions->ContextRecord->R11;
    DWORD64 R12 = Exceptions->ContextRecord->R12;
    DWORD64 LastExceptionFromRip = Exceptions->ContextRecord->LastExceptionFromRip;
    DWORD64 LastExceptionToRip = Exceptions->ContextRecord->LastExceptionToRip;

    switch (code)
    {
    case EXCEPTION_ACCESS_VIOLATION:
    {
        g_f_or_t(FALSE);
        stringstream ss = {};
        ss << string('-', 30) << "EXCEPTION_ACCESS_VIOLATION" << string('-', 30)
            << left << setw(10) << "Status:" << "|"
            << left << setw(10) << to_string(Rsi) + ":" << "|\n"
            << left << setw(10) << to_string(Rip) + ":" << "|\n"
            << left << setw(10) << to_string(Rax) + ":" << "|\n"
            << left << setw(10) << to_string(Rbp) + ":" << "|\n"
            << left << setw(10) << to_string(Rcx) + ":" << "|\n"
            << left << setw(10) << to_string(Rdx) + ":" << "|\n"
            << left << setw(10) << to_string(R8) + ":" << "|\n"
            << left << setw(10) << to_string(R9) + ":" << "|\n"
            << left << setw(10) << to_string(R10) + ":" << "|\n"
            << left << setw(10) << to_string(R11) + ":" << "|\n"
            << left << setw(10) << to_string(R12) + ":" << "|\n"
            << left << setw(10) << to_string(LastExceptionFromRip) + ":" << "|\n"
            << left << setw(10) << to_string(LastExceptionToRip) + ":" << "|\n"
            << string('-', 70) << endl;
        FileWriteLog(wstr, ss.str());
        return EXCEPTION_EXECUTE_HANDLER;
    }break;

    case EXCEPTION_INT_DIVIDE_BY_ZERO:
    {
        g_f_or_t(FALSE);
        stringstream ss = {};
        ss << string('-', 30) << "EXCEPTION_INT_DIVIDE_BY_ZERO" << string('-', 30)
            << left << setw(10) << "Status:" << "|"
            << left << setw(10) << to_string(Rsi) + ":" << "|\n"
            << left << setw(10) << to_string(Rip) + ":" << "|\n"
            << left << setw(10) << to_string(Rax) + ":" << "|\n"
            << left << setw(10) << to_string(Rbp) + ":" << "|\n"
            << left << setw(10) << to_string(Rcx) + ":" << "|\n"
            << left << setw(10) << to_string(Rdx) + ":" << "|\n"
            << left << setw(10) << to_string(R8) + ":" << "|\n"
            << left << setw(10) << to_string(R9) + ":" << "|\n"
            << left << setw(10) << to_string(R10) + ":" << "|\n"
            << left << setw(10) << to_string(R11) + ":" << "|\n"
            << left << setw(10) << to_string(R12) + ":" << "|\n"
            << left << setw(10) << to_string(LastExceptionFromRip) + ":" << "|\n"
            << left << setw(10) << to_string(LastExceptionToRip) + ":" << "|\n"
            << string('-', 70) << endl;
        FileWriteLog(wstr, ss.str());
        return EXCEPTION_EXECUTE_HANDLER;
    }break;
    case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
    {
        g_f_or_t(FALSE);
        stringstream ss = {};
        ss << string('-', 30) << "EXCEPTION_ARRAY_BOUNDS_EXCEEDED" << string('-', 30)
            << left << setw(10) << "Status:" << "|"
            << left << setw(10) << to_string(Rsi) + ":" << "|\n"
            << left << setw(10) << to_string(Rip) + ":" << "|\n"
            << left << setw(10) << to_string(Rax) + ":" << "|\n"
            << left << setw(10) << to_string(Rbp) + ":" << "|\n"
            << left << setw(10) << to_string(Rcx) + ":" << "|\n"
            << left << setw(10) << to_string(Rdx) + ":" << "|\n"
            << left << setw(10) << to_string(R8) + ":" << "|\n"
            << left << setw(10) << to_string(R9) + ":" << "|\n"
            << left << setw(10) << to_string(R10) + ":" << "|\n"
            << left << setw(10) << to_string(R11) + ":" << "|\n"
            << left << setw(10) << to_string(R12) + ":" << "|\n"
            << left << setw(10) << to_string(LastExceptionFromRip) + ":" << "|\n"
            << left << setw(10) << to_string(LastExceptionToRip) + ":" << "|\n"
            << string('-', 70) << endl;
        FileWriteLog(wstr, ss.str());
        return EXCEPTION_EXECUTE_HANDLER;
    }break;
    case EXCEPTION_STACK_OVERFLOW:
    {
        g_f_or_t(FALSE);
        stringstream ss = {};
        ss << string('-', 30) << "EXCEPTION_STACK_OVERFLOW" << string('-', 30)
            << left << setw(10) << "Status:" << "|"
            << left << setw(10) << to_string(Rsi) + ":" << "|\n"
            << left << setw(10) << to_string(Rip) + ":" << "|\n"
            << left << setw(10) << to_string(Rax) + ":" << "|\n"
            << left << setw(10) << to_string(Rbp) + ":" << "|\n"
            << left << setw(10) << to_string(Rcx) + ":" << "|\n"
            << left << setw(10) << to_string(Rdx) + ":" << "|\n"
            << left << setw(10) << to_string(R8) + ":" << "|\n"
            << left << setw(10) << to_string(R9) + ":" << "|\n"
            << left << setw(10) << to_string(R10) + ":" << "|\n"
            << left << setw(10) << to_string(R11) + ":" << "|\n"
            << left << setw(10) << to_string(R12) + ":" << "|\n"
            << left << setw(10) << to_string(LastExceptionFromRip) + ":" << "|\n"
            << left << setw(10) << to_string(LastExceptionToRip) + ":" << "|\n"
            << string('-', 70) << endl;
        FileWriteLog(wstr, ss.str());
        return EXCEPTION_EXECUTE_HANDLER;
    }break;
    case EXCEPTION_DATATYPE_MISALIGNMENT:
    {
        g_f_or_t(FALSE);
        stringstream ss = {};
        ss << string('-', 30) << "EXCEPTION_STACK_OVERFLOW" << string('-', 30)
            << left << setw(10) << "Status:" << "|"
            << left << setw(10) << to_string(Rsi) + ":" << "|\n"
            << left << setw(10) << to_string(Rip) + ":" << "|\n"
            << left << setw(10) << to_string(Rax) + ":" << "|\n"
            << left << setw(10) << to_string(Rbp) + ":" << "|\n"
            << left << setw(10) << to_string(Rcx) + ":" << "|\n"
            << left << setw(10) << to_string(Rdx) + ":" << "|\n"
            << left << setw(10) << to_string(R8) + ":" << "|\n"
            << left << setw(10) << to_string(R9) + ":" << "|\n"
            << left << setw(10) << to_string(R10) + ":" << "|\n"
            << left << setw(10) << to_string(R11) + ":" << "|\n"
            << left << setw(10) << to_string(R12) + ":" << "|\n"
            << left << setw(10) << to_string(LastExceptionFromRip) + ":" << "|\n"
            << left << setw(10) << to_string(LastExceptionToRip) + ":" << "|\n"
            << string('-', 70) << endl;
        FileWriteLog(wstr, ss.str());
        return EXCEPTION_EXECUTE_HANDLER;
    }break;
    case EXCEPTION_INT_OVERFLOW:
    {
        g_f_or_t(FALSE);
        stringstream ss = {};
        ss << string('-', 30) << "EXCEPTION_INT_OVERFLOW" << string('-', 30)
            << left << setw(10) << "Status:" << "|"
            << left << setw(10) << to_string(Rsi) + ":" << "|\n"
            << left << setw(10) << to_string(Rip) + ":" << "|\n"
            << left << setw(10) << to_string(Rax) + ":" << "|\n"
            << left << setw(10) << to_string(Rbp) + ":" << "|\n"
            << left << setw(10) << to_string(Rcx) + ":" << "|\n"
            << left << setw(10) << to_string(Rdx) + ":" << "|\n"
            << left << setw(10) << to_string(R8) + ":" << "|\n"
            << left << setw(10) << to_string(R9) + ":" << "|\n"
            << left << setw(10) << to_string(R10) + ":" << "|\n"
            << left << setw(10) << to_string(R11) + ":" << "|\n"
            << left << setw(10) << to_string(R12) + ":" << "|\n"
            << left << setw(10) << to_string(LastExceptionFromRip) + ":" << "|\n"
            << left << setw(10) << to_string(LastExceptionToRip) + ":" << "|\n"
            << string('-', 70) << endl;
        FileWriteLog(wstr, ss.str());
        return EXCEPTION_EXECUTE_HANDLER;
    }break;
    }


    return EXCEPTION_CONTINUE_SEARCH;
}

//start ntapi coding

LPVOID MyGetModuleAddr(_In_ wstring ModuleName)
{
    if (!_g_f_or_t)
    {
        return 0;
    }

    PPEB_LDR_DATA pLdr = reinterpret_cast<PPEB_LDR_DATA>(GetLDR());
    PLIST_ENTRY pListHead = &pLdr->InMemoryOrderModuleList;
    PLIST_ENTRY pListEntry = pListHead->Flink;
    string temp = WstringToString(ModuleName);

    while (pListEntry != pListHead)
    {
        PMY_LDR_DATA_TABLE_ENTRY pEntry = CONTAINING_RECORD(pListEntry, MY_LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
        if (pEntry->BaseDllName.Buffer != NULL && pEntry->FullDllName.Buffer != NULL)
        {
            if (_wcsicmp(pEntry->BaseDllName.Buffer, ModuleName.c_str()) == 0)
            {
                return reinterpret_cast<LPVOID>(pEntry->DllBase);
            }
        }
        pListEntry = pListEntry->Flink;
    }
    return NULL;
}

LPVOID MyGetFuncAddr(_In_ PBYTE BaseAddr, _In_ string FuncName)
{
    if (!_g_f_or_t)
    {
        return 0;
    }
    PIMAGE_DOS_HEADER dos = reinterpret_cast<PIMAGE_DOS_HEADER>(BaseAddr);
    if (dos->e_magic != 0x5A4D)
    {
        g_f_or_t(FALSE);
        return NULL;
    }
    PIMAGE_NT_HEADERS nt = reinterpret_cast<PIMAGE_NT_HEADERS>(BaseAddr + dos->e_lfanew);
    if (nt->Signature != IMAGE_NT_SIGNATURE)
    {
        g_f_or_t(FALSE);
        return NULL;
    }
    IMAGE_DATA_DIRECTORY DataDirec = nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];

    if (DataDirec.VirtualAddress == 0 || DataDirec.Size == 0)
    {
        g_f_or_t(FALSE);
        return NULL;
    }

    PIMAGE_EXPORT_DIRECTORY ExpDirec = reinterpret_cast<PIMAGE_EXPORT_DIRECTORY>(BaseAddr + DataDirec.VirtualAddress);


    PDWORD AddrOfFunc = reinterpret_cast<PDWORD>(BaseAddr + ExpDirec->AddressOfFunctions);
    PDWORD AddrOfName = reinterpret_cast<PDWORD>(BaseAddr + ExpDirec->AddressOfNames);
    PWORD OrdinalsOfName = reinterpret_cast<PWORD>(BaseAddr + ExpDirec->AddressOfNameOrdinals);
    DWORD NumOfName = ExpDirec->NumberOfNames;
    DWORD RVA = 0;
    for (DWORD i = 0; i < NumOfName; i++)
    {
        char* _FuncName = reinterpret_cast<char*>(BaseAddr + AddrOfName[i]);
        if (strcmp(_FuncName, FuncName.c_str()) == 0)
        {
            DWORD temp = OrdinalsOfName[i];
            RVA = AddrOfFunc[temp];
            break;
        }
    }
    if (RVA == 0)
    {
        g_f_or_t(FALSE);
        return 0;
    }
    return BaseAddr + RVA;
}

DWORD MyGetSyscallID(_In_ PBYTE BaseNtDllAddr, _In_ string FuncName)
{
    if (!_g_f_or_t)
    {
        return 0;
    }
    if (BaseNtDllAddr == NULL)
    {
        return g_f_or_t(FALSE);
    }
    DWORD ssn = 0;
    BOOL bSsnFound = FALSE;
    PUCHAR pCw = reinterpret_cast<PUCHAR>
        (MyGetFuncAddr(BaseNtDllAddr, FuncName));
    if (pCw == nullptr)
    {
        return 0;
    }
    if (*pCw == 0x4C && *(pCw + 1) == 0x8B && *(pCw + 2) == 0xD1 && *(pCw + 3) == 0xB8)
    {
        ssn = *(reinterpret_cast<PDWORD>(pCw + 4));
        bSsnFound = TRUE;
        return ssn;
    }
    else
    {
        for (WORD idx = 1; idx <= 39; idx++)
        {
            PUCHAR pDown = pCw + (idx * 0x20);
            if (*pDown == 0x4C && *(pDown + 1) == 0x8B && *(pDown + 2) == 0xD1 && *(pDown + 3) == 0xB8)
            {
                ssn = (*(reinterpret_cast<PDWORD>(pDown + 4)) - idx);
                bSsnFound = TRUE;
                break;
            }

            PUCHAR pUp = pCw - (idx * 0x20);
            if (*pUp == 0x4C && *(pUp + 1) == 0x8B && *(pUp + 2) == 0xD1 && *(pUp + 3) == 0xB8)
            {
                ssn = *(reinterpret_cast<PDWORD>(pUp + 4)) + idx;
                bSsnFound = TRUE;
                break;
            }

        }
        if (!bSsnFound)
        {
            g_f_or_t(FALSE);
            return 0;
        }
        return bSsnFound ? ssn : 0;
    }
}

BOOL MyCheckNtStatusFunc(
    _In_ string WhereFunc,
    _In_ string ErrFuncName,
    _In_ NTSTATUS status)
{
    stringstream ss = {};
    if (!NT_SUCCESS(status))
    {
        ss << "[-] Func In " << WhereFunc << ", " << ErrFuncName << " Failed";
        ErrMsgConsole(ss.str(), "FALSE", status);
        return FALSE;
    }
    else {
        ss << "[+] Func In " << WhereFunc << ", " << ErrFuncName << " Success";
        MsgConsole(ss.str(), "TRUE", status);
        return TRUE;
    }
}



BOOL MyLoadFileToMem(
    _In_ const wchar_t* _FilePath,
    _Inout_ LPVOID* ToReadBufferAddr,
    _Out_ PSIZE_T pBufferSz)
{
    if (_g_f_or_t == FALSE || _FilePath[0] == NULL)
    {
        return FALSE;
    }
    *ToReadBufferAddr = nullptr;
    OBJECT_ATTRIBUTES obj = {};

    wstring ntDllPath = L"\\??\\" + wstring(_FilePath);
    UNICODE_STRING UniStr = {};

    UniStr.Length = static_cast<USHORT>(ntDllPath.length() * sizeof(wchar_t));
    UniStr.MaximumLength = static_cast<USHORT>((ntDllPath.length() + 1) * sizeof(wchar_t));
    UniStr.Buffer = const_cast<PWSTR>(ntDllPath.c_str());

    InitializeObjectAttributes(&obj, &UniStr, OBJ_CASE_INSENSITIVE, NULL, NULL);

    HANDLE hFile = nullptr;
    IO_STATUS_BLOCK ioStatusBlock = {};
    if (!MyCheckNtStatusFunc(
        "MyLoadFileToMem",
        "NtCreateFile",
        Sys_NtCreateFile(
            NtCreateFile_SyscallId,
            &hFile,
            FILE_GENERIC_READ,
            &obj,
            &ioStatusBlock,
            nullptr,
            FILE_ATTRIBUTE_NORMAL,
            FILE_SHARE_READ,
            FILE_OPEN,
            FILE_SYNCHRONOUS_IO_NONALERT,
            NULL, 0)))
    {
        return g_f_or_t(FALSE);
    }
    else { MsgConsole("[+] NtCreateFile Success", "TRUE", 0); }
    FILE_STANDARD_INFORMATION fileInfo = { 0 };
    IO_STATUS_BLOCK ioStatusBlock2 = { 0 };
    if (!MyCheckNtStatusFunc(
        "MyLoadFileToMem",
        "NtQueryInformationFile",
        Sys_NtQueryInformationFile(
            NtQueryInformationFile_SyscallId,
            hFile,
            &ioStatusBlock2,
            &fileInfo,
            sizeof(fileInfo),
            ((FILE_INFORMATION_CLASS)5))))
    {
        Sys_NtClose(NtClose_SyscallId, hFile);
        MsgConsole("[-] NtQueryInformationFile Failed", "FALSE", -1);
        return g_f_or_t(FALSE);
    }
    LARGE_INTEGER fileSize = fileInfo.EndOfFile;
    *pBufferSz = static_cast<SIZE_T>(fileSize.QuadPart);

    LPVOID pLocalBuffer = nullptr;
    if (!MyCheckNtStatusFunc(
        "MyLoadFileToMem",
        "NtAllocateVirtuaMemory",
        Sys_NtAllocateVirtualMemory(
            NtAllocateVirtualMemory_SyscallId,
            NtCurrentProcess(),
            &pLocalBuffer,
            0, pBufferSz,
            MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE)))
    {
        Sys_NtClose(NtClose_SyscallId, hFile);
        ErrMsgConsole("[-] NtAllocateVirtualMemory Failed", "FALSE", -1);
        return g_f_or_t(FALSE);
    }

    LARGE_INTEGER byteOffset = { 0 };
    ULONG readLength = static_cast<ULONG>(fileSize.QuadPart);

    if (!MyCheckNtStatusFunc(
        "MyLoadFileToMem",
        "NtReadFile",
        Sys_NtReadFile(
            NtReadFile_SyscallId,
            hFile,
            NULL, NULL, NULL,
            &ioStatusBlock,
            pLocalBuffer,
            readLength,
            &byteOffset,
            NULL)))
    {
        Sys_NtClose(NtClose_SyscallId, hFile);
        Sys_NtFreeVirtualMemory(NtFreeVirtualMemory_SyscallId, NtCurrentProcess(),
            ToReadBufferAddr, pBufferSz, MEM_RELEASE);
        ErrMsgConsole("[-] NtReadFile Failed", "FALSE", -1);
        return g_f_or_t(FALSE);
    }

    PIMAGE_DOS_HEADER dos = reinterpret_cast<PIMAGE_DOS_HEADER>(pLocalBuffer);
    if (dos->e_magic != 0x5A4D)
    {
        Sys_NtClose(NtClose_SyscallId, hFile);
        Sys_NtFreeVirtualMemory(NtFreeVirtualMemory_SyscallId, NtCurrentProcess(),
            ToReadBufferAddr, pBufferSz, MEM_RELEASE);
        ErrMsgConsole("[-] Dll File Doesnt Have MZ Signature", "FALSE", -1);
        return g_f_or_t(FALSE);
    }
    PIMAGE_NT_HEADERS nt = reinterpret_cast<PIMAGE_NT_HEADERS>((PBYTE)pLocalBuffer + dos->e_lfanew);
    if (nt->Signature != IMAGE_NT_SIGNATURE)
    {
        Sys_NtClose(NtClose_SyscallId, hFile);
        Sys_NtFreeVirtualMemory(NtFreeVirtualMemory_SyscallId, NtCurrentProcess(),
            ToReadBufferAddr, pBufferSz, MEM_RELEASE);
        ErrMsgConsole("[-] Dll File Doesnt Have NT Signature", "FALSE", -1);
        return g_f_or_t(FALSE);

    }
    *ToReadBufferAddr = pLocalBuffer;
    MsgConsole("[+] Size Of Ntoskrnl.exe is", to_string(fileSize.QuadPart), 1);

    Sys_NtClose(NtClose_SyscallId, hFile);

    return g_f_or_t(TRUE);
}

typedef struct _PDB_INFO_RSDS {
    DWORD Signature;
    GUID Guid;
    DWORD Age;
    char PdbName[1];
}PDB_INFO_RSDS, *pPDB_INFO_RSDS;

DWORD RvaToOffset(PIMAGE_NT_HEADERS pNtHeaders, DWORD rva)
{
    PIMAGE_SECTION_HEADER pSectionHeader = IMAGE_FIRST_SECTION(pNtHeaders);
    //traversal section
    for(int i = 0; i < pNtHeaders->FileHeader.NumberOfSections;i++)
    {
        //secure check
        if (rva >= pSectionHeader->VirtualAddress &&
            rva < (pSectionHeader->VirtualAddress + pSectionHeader->Misc.VirtualSize))
        {
            //return rva
            return rva - pSectionHeader->VirtualAddress + pSectionHeader->PointerToRawData;
        }
        pSectionHeader++;
    }
    return 0;
}

BOOLEAN MyFindPdbSignature(
    _In_ LPVOID NtoskrnlFileBuffer,
    _Out_ PCHAR Guid,
    _Out_ PDWORD pAge,
    _Out_ PCHAR PdbName)
{
    BOOLEAN success = FALSE;
    PIMAGE_DOS_HEADER pDosHeader = 
        reinterpret_cast<PIMAGE_DOS_HEADER>(reinterpret_cast<ULONG_PTR>(NtoskrnlFileBuffer));
    if (pDosHeader->e_magic != 0x5A4D)
    {
        PWORD temp = (PWORD)NtoskrnlFileBuffer;
        printf("[-] INVALID MZ DOS HEADER IN Ntoskrnl.exe! first two bytes =%X", *temp);
        return FALSE;
    }

    PIMAGE_NT_HEADERS pNtHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>
        (reinterpret_cast<ULONG_PTR>(NtoskrnlFileBuffer) + pDosHeader->e_lfanew);
    if (pNtHeaders->Signature != IMAGE_NT_SIGNATURE)
    {
        printf("[-] INVALID NT SIGNATURE");
        return FALSE;
    }
    IMAGE_DATA_DIRECTORY dtaDirec =
        pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG];
    if (dtaDirec.VirtualAddress == NULL || dtaDirec.Size == 0)
    {
        printf("[-] INVALID DEBUG DIRECTOYRY");
        return FALSE;
    }

    DWORD DebugOffset = RvaToOffset(pNtHeaders, dtaDirec.VirtualAddress);
    if (DebugOffset == 0)
    {
        printf("[-] INVALID DEBUG DIRECROTY OFFSET");
        return FALSE;
    }

    PIMAGE_DEBUG_DIRECTORY pDebugDirectory =
        reinterpret_cast<PIMAGE_DEBUG_DIRECTORY>((ULONG_PTR)NtoskrnlFileBuffer + DebugOffset);
    int EntriesDebugCount = dtaDirec.Size / sizeof(IMAGE_DEBUG_DIRECTORY);

    for (int i = 0; i < EntriesDebugCount; i++)
    {
        if (pDebugDirectory[i].Type == IMAGE_DEBUG_TYPE_CODEVIEW)
        {
            DWORD DataOffset = pDebugDirectory[i].PointerToRawData;
            if (DataOffset == 0 && pDebugDirectory[i].AddressOfRawData != 0)
            {
                DataOffset = RvaToOffset(pNtHeaders, pDebugDirectory[i].AddressOfRawData);
            }
            if (DataOffset == 0) continue;

            pPDB_INFO_RSDS pRsdsInfo =
                reinterpret_cast<pPDB_INFO_RSDS>((ULONG_PTR)NtoskrnlFileBuffer + DataOffset);

            // "RSDS" Signature Check
            if (pRsdsInfo->Signature == 0x53445352)
            {
                success = TRUE;
                printf("[+] Successfully Get PDB Debug Information!");

                //GUID format 8-4-4-4-12 hex
                char GuidTemp[33] = { 0 };
                sprintf_s(
                    GuidTemp,
                    "%08X%04X%04X%02X%02X%02X%02X%02X%02X%02X%02X",
                    pRsdsInfo->Guid.Data1,
                    pRsdsInfo->Guid.Data2,
                    pRsdsInfo->Guid.Data3,
                    pRsdsInfo->Guid.Data4[0], pRsdsInfo->Guid.Data4[1],
                    pRsdsInfo->Guid.Data4[2], pRsdsInfo->Guid.Data4[3],
                    pRsdsInfo->Guid.Data4[4], pRsdsInfo->Guid.Data4[5],
                    pRsdsInfo->Guid.Data4[6], pRsdsInfo->Guid.Data4[7]
                );
                memcpy(Guid,GuidTemp,32);
                Guid[32] = '\0';
                cout << "\nGUID: ";
                
                for (int i = 0; i < 32; i++)
                {
                    cout << Guid[i];
                    if (i == 7 || i == 11 || i == 15 || i == 19) cout << "-";
                }
                cout << dec << endl;

                //Other
                cout << "Age: " << pRsdsInfo->Age << endl;
                *pAge = pRsdsInfo->Age;
                cout << "PDB: " << pRsdsInfo->PdbName << endl;
                strcpy_s(PdbName, MAX_PATH, pRsdsInfo->PdbName);

                break;
            }
        }
    }
    return success;
}

// due to its a ssl connection, i use wininet
BOOLEAN MyToDownloadPdb(
    _In_ PCHAR _GUID,
    _In_ PDWORD _Age,
    _In_ PCHAR _PdbName,
    _In_ PCWSTR savePath
)
{
    NTSTATUS status;
    BOOLEAN success = TRUE;

    HINTERNET hSession = InternetOpenW(
        L"PdbDownloader-Kitcat/1.0",
        INTERNET_OPEN_TYPE_PRECONFIG,
        NULL, NULL, 0
    );

    if (!hSession)
    {
        wcerr << L"[-] Network Init Failed" << endl;
        return success;
    }
    wstring PdbName(_PdbName, _PdbName + strlen(_PdbName));
    wstring Guid(_GUID, _GUID + strlen(_GUID));
   
    wstring url = L"https://msdl.microsoft.com/download/symbols/" 
        + PdbName 
        + L"/" + Guid + to_wstring(*_Age)
        + L"/" + PdbName;

    wcout << L"[+] Connecting With HTTP Request..." << endl;

    HINTERNET hUrlFile = InternetOpenUrlW(
        hSession,
        url.c_str(),
        NULL, 0,
        INTERNET_FLAG_RELOAD | INTERNET_FLAG_SECURE,
        0
    );
    
    if (!hUrlFile)
    {
        wcerr << L"[-] Cant Connect Server Or SSL HandShake Failed..." << endl;
        InternetCloseHandle(hSession);
        InternetCloseHandle(hUrlFile);
        return success;
    }

    DWORD statusCode = 0;
    DWORD statusCodeLen = sizeof(statusCode);

    HttpQueryInfoW(hUrlFile, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER,
        &statusCode, &statusCodeLen, NULL);

    if (statusCode != 200)
    {
        wcerr << L"[-] Server Connect Failed, Status Code = " << statusCode << endl;
        InternetCloseHandle(hSession);
        InternetCloseHandle(hUrlFile);
        return success;
    }

    wcout << "[+] Connected Server (HTTP 200), Starting Get Pdb Binary Data" << endl;

    HANDLE hFile{};
    OBJECT_ATTRIBUTES obj{};
    UNICODE_STRING UnicodePath{};
    IO_STATUS_BLOCK CreateIoStatus{};
    RtlInitUnicodeString(&UnicodePath, savePath);

    InitializeObjectAttributes(&obj, &UnicodePath, OBJ_CASE_INSENSITIVE, NULL, NULL);
    if (!MyCheckNtStatusFunc(
        "MyToDownloadPdb",
        "NtCreateFile",
        Sys_NtCreateFile(
            NtCreateFile_SyscallId,
            &hFile,
            GENERIC_WRITE | SYNCHRONIZE,
            &obj,
            &CreateIoStatus,
            NULL,
            FILE_ATTRIBUTE_NORMAL,
            FILE_SHARE_READ,
            FILE_OVERWRITE_IF,
            FILE_SYNCHRONOUS_IO_NONALERT,
            NULL,
            0
        )))
    {
        InternetCloseHandle(hSession);
        InternetCloseHandle(hUrlFile);
        return success;
    }

    vector<BYTE> buffer(4096);
    DWORD bytesRead = 0;
    size_t totalBytes = 0;
    IO_STATUS_BLOCK WriteStatus = { 0 };
    LARGE_INTEGER bytesOffset = { 0 };

    cout << "Starting Download... Please Wait..." << endl;
    while (
        InternetReadFile(hUrlFile, buffer.data(),
            static_cast<DWORD>(buffer.size()), &bytesRead)
        && bytesRead > 0)
    {
        status = 
            Sys_NtWriteFile(
                NtWriteFile_SyscallId,
                hFile,
                NULL, NULL, NULL,
                &WriteStatus,
                buffer.data(),
                bytesRead,
                NULL,
                NULL);

        if(!NT_SUCCESS(status))
        {
            InternetCloseHandle(hSession);
            InternetCloseHandle(hUrlFile);
            Sys_NtClose(NtClose_SyscallId, hFile);
            return success;
        }
        totalBytes += bytesRead;
    }
    if (totalBytes == 0)
    {
        cerr << "[-] Receive File Pdb Failed..." << endl;
    }
    else {
        success = TRUE;
        cout << "[+] Recived File Successfully!" << endl;
    }
    InternetCloseHandle(hSession);
    InternetCloseHandle(hUrlFile);
    Sys_NtClose(NtClose_SyscallId, hFile);
    return success;
}

BOOLEAN MySetFileDirectory(
    _Out_ wstring* NtSavePath,
    _Out_ wstring* NormalSavePath
)
{
    PPEB pPeb = reinterpret_cast<PPEB>(GetPEB());
    PRTL_USER_PROCESS_PARAMETERS_PRIVATE params =
        reinterpret_cast<PRTL_USER_PROCESS_PARAMETERS_PRIVATE>(pPeb->ProcessParameters);
    const USHORT dirLength = params->CurrentDirectoryPath.Length / sizeof(wchar_t);

    wchar_t* buffer = params->CurrentDirectoryPath.Buffer;
    wchar_t temp[MAX_PATH] = { 0 };
    memcpy(temp, buffer, MAX_PATH);
    temp[dirLength] = '\0';

    for (int i = dirLength; i > 0; i--)
    {
        if (*(buffer + i) == '\\')
        {
            cout << i << endl;
            temp[i + 1] = '\0';
            break;
        }
    }
    wstring OutputPath(temp);
    OutputPath += L"ntkrnlmap.pdb";
    *NormalSavePath = OutputPath;
    OutputPath.insert(0, L"\\??\\");
    *NtSavePath = OutputPath;
    cout << "[+] Founded File Needed Directory: ";
    wcout << *NtSavePath << endl;
}

BOOLEAN MyFindPdbDataOffsets(
    _In_ PCWSTR pdbPath,
    _In_ PCWSTR DataName
)
{
    BOOLEAN status = FALSE;
    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr))
    {
        MsgConsole("CoInitialize Failed", "FALSE", -1);
        return status;
    }

    //Data Source
    CComPtr<IDiaDataSource> pSource;
    hr = CoCreateInstance(
        CLSID_DiaSource,
        NULL,
        CLSCTX_INPROC_SERVER,
        __uuidof(IDiaDataSource),
        reinterpret_cast<LPVOID*>(&pSource));
    if (FAILED(hr))
    {
        MsgConsole("CoCreateInstance Failed...", "FALSE", -1);
        CoUninitialize();
        return status;
    }
    hr = pSource->loadDataFromPdb(pdbPath);
    if (FAILED(hr))
    {
        MsgConsole("LoadDataFromPdb Failed...", "FALSE", -1);
        //printf("CoCreateInstance Failed... HRESULT: 0x%08X\n", hr);
        CoUninitialize();
        return status;
    }

    //Data Session
    CComPtr<IDiaSession> pSession;
    hr = pSource->openSession(&pSession);
    if (FAILED(hr))
    {
        MsgConsole("openSession Failed...", "FALSE", -1);
        CoUninitialize();
        return status;
    }

    //Data Symbol
    CComPtr<IDiaSymbol> pGlobal;
    hr = pSession->get_globalScope(&pGlobal);
    if (FAILED(hr))
    {
        MsgConsole("get_globalScope Failed...", "FALSE", -1);
        CoUninitialize();
        return status;
    }

    //Enum Data Symbols
    CComPtr<IDiaEnumSymbols> pEnum;
    hr = pGlobal->findChildren(SymTagUDT, DataName, nsNone, &pEnum);
    if (FAILED(hr))
    {
        MsgConsole("findChildren Failed...", "FALSE", -1);
        CoUninitialize();
        return status;
    }

    //Try To Get Count Data Of DataName
    LONG count = 0;
    pEnum->get_Count(&count);
    if (count == 0)
    {
        MsgConsole("get_Count Failed, Maybe your DataName is Wrong...", "FALSE", -1);
        CoUninitialize();
        return status;
    }

    //Try To Get
    CComPtr<IDiaSymbol> pDataName;
    ULONG fetched = 0;
    hr = pEnum->Next(1, &pDataName, &fetched);
    if (FAILED(hr))
    {
        MsgConsole("Found DataName Struct Failed...", "FALSE", -1);
        CoUninitialize();
        return status;
    }
    else {
        wcout << L"Founded " << DataName << L" Struct, Starting Traversaling Member" << endl;
        wcout << wstring(50, '=') << endl;
    }
    
    CComPtr<IDiaEnumSymbols> pMembers;
    hr = pDataName->findChildren(SymTagNull, NULL, nsNone, &pMembers);
    if (SUCCEEDED(hr))
    {
        status = TRUE;
        CComPtr<IDiaSymbol> pMember;
        while (SUCCEEDED(pMembers->Next(1, &pMember, &fetched)) && fetched == 1)
        {
            DWORD symTag = 0;
            pMember->get_symTag(&symTag);

            if (symTag == SymTagData)
            {
                BSTR bstrName = nullptr;
                LONG offset = 0;

                pMember->get_name(&bstrName);
                pMember->get_offset(&offset);

                if (bstrName)
                {
                    wcout << L"Member: " << left << setw(40) << bstrName
                        << L" | Offset: 0x" <<hex << left << setw(10) << offset << L"\n" << dec;
                }
            }
            pMember=nullptr;
        }
    }
    CoUninitialize();
    return status;
}

int main()
{
    SetUnhandledExceptionFilter(GlobalCrashHandler);
    NTSTATUS status;
    wchar_t NtoskrnlFilePath[] = L"C:\\Windows\\System32\\ntoskrnl.exe";
    PBYTE NtdllModule = reinterpret_cast<PBYTE>(MyGetModuleAddr(L"ntdll.dll"));
    if (!NtdllModule) {
        printf("Ntdll Not Found!");
        return -1;
    }
    NtQuerySystemInformation_SyscallId = static_cast<UINT>(MyGetSyscallID(NtdllModule,"NtQuerySystemInformation"));
    NtAllocateVirtualMemory_SyscallId = static_cast<UINT>(MyGetSyscallID(NtdllModule,"NtAllocateVirtualMemory"));
    NtClose_SyscallId = static_cast<UINT>(MyGetSyscallID(NtdllModule,"NtClose"));
    NtQueryInformationProcess_SyscallId = static_cast<UINT>(MyGetSyscallID(NtdllModule,"NtQueryInformationProcess"));
    NtReadVirtualMemory_SyscallId = static_cast<UINT>(MyGetSyscallID(NtdllModule,"NtReadVirtualMemory"));
    NtFreeVirtualMemory_SyscallId = static_cast<UINT>(MyGetSyscallID(NtdllModule,"NtFreeVirtualMemory"));
    NtCreateFile_SyscallId = static_cast<UINT>(MyGetSyscallID(NtdllModule,"NtCreateFile"));
    NtReadFile_SyscallId = static_cast<UINT>(MyGetSyscallID(NtdllModule,"NtReadFile"));
    NtQueryInformationFile_SyscallId = static_cast<UINT>(MyGetSyscallID(NtdllModule,"NtQueryInformationFile"));
    NtWriteFile_SyscallId = static_cast<UINT>(MyGetSyscallID(NtdllModule, "NtWriteFile"));
    if (NtQuerySystemInformation_SyscallId != 0 && NtAllocateVirtualMemory_SyscallId != 0 &&
        NtClose_SyscallId != 0 && NtQueryInformationProcess_SyscallId != 0 &&
        NtReadVirtualMemory_SyscallId != 0 && NtFreeVirtualMemory_SyscallId != 0 &&
        NtCreateFile_SyscallId != 0 && NtReadFile_SyscallId                                                                       != 0 &&
        NtQueryInformationFile_SyscallId != 0 && NtWriteFile_SyscallId != 0)
    {
        printf("[+] Init Success");
    }
    else {
        printf("[-] Init Failed!");
        return -1;
    }

    LPVOID NtoskrnlFileBuffer = nullptr;
    SIZE_T SizeBuffer = 0;
    MyLoadFileToMem(NtoskrnlFilePath, &NtoskrnlFileBuffer, &SizeBuffer);

    CHAR Guid[33] = {0};
    DWORD Age = 0;
    CHAR PdbName[MAX_PATH] = { 0 };
    if (!MyFindPdbSignature(NtoskrnlFileBuffer, Guid, &Age, PdbName))
    {
        return -1;
    }

    wstring NtSavePath{};
    wstring NormalSavePath{};
    MySetFileDirectory(&NtSavePath,&NormalSavePath);
    MyToDownloadPdb(Guid, &Age, PdbName, NtSavePath.c_str());
    MyFindPdbDataOffsets(NormalSavePath.c_str(), L"_EPROCESS");
    ZeroMemory(NtoskrnlFileBuffer, SizeBuffer);

    return 0;
}
