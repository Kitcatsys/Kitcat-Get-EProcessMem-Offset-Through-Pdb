default rel

section .text

global GetPEB
global GetLDR
global Sys_NtQuerySystemInformation
global Sys_NtClose
global Sys_NtQueryInformationProcess
global Sys_NtAllocateVirtualMemory
global Sys_NtReadVirtualMemory
global Sys_NtFreeVirtualMemory
global Sys_NtProtectVirtualMemory
global Sys_NtCreateFile
global Sys_NtReadFile
global Sys_NtQueryInformationFile
global Sys_NtWriteFile

GetPEB:
        mov rax,[gs:0x60]
        ret

GetLDR:
	mov rax,[gs: 0x60]
	mov rax,[rax + 0x18]
	ret

Sys_NtQuerySystemInformation:
    mov eax,ecx
    mov r10,rdx
    mov rdx,r8
    mov r8,r9
    mov r9,[rsp+40]

    syscall
    ret
    
Sys_NtQueryInformationProcess:
    mov eax,ecx
    mov r10,rdx
    mov rdx,r8
    mov r8,r9
    mov r9,[rsp+40]
    
    mov r11,[rsp+48]
    mov [rsp+40],r11
    
    
    syscall
    ret
    
Sys_NtAllocateVirtualMemory:
    mov eax,ecx
    mov r10,rdx
    mov rdx,r8
    mov r8,r9
    mov r9,[rsp+40]
    
    mov r11, [rsp+48]
    mov [rsp+40],r11
    
    mov r11,[rsp+56]
    mov [rsp+48],r11
    
    
    syscall
    ret
    
Sys_NtReadVirtualMemory:

    mov eax,ecx
    mov r10,rdx
    mov rdx,r8
    mov r8,r9
    mov r9,[rsp+40]
    
    mov r11,[rsp+48]
    mov [rsp+40],r11
    
    syscall
    ret
    
Sys_NtFreeVirtualMemory:
    mov eax,ecx
    mov r10,rdx
    mov rdx,r8
    mov r8,r9
    mov r9,[rsp+40]
    
    syscall
    ret
    
Sys_NtClose:
    mov eax,ecx
    mov r10,rdx
    
    syscall
    ret
    
Sys_NtCreateFile:
    mov eax,ecx
    mov r10,rdx
    mov rdx,r8
    mov r8,r9
    mov r9,[rsp+40]
    
    mov r11,[rsp+48]
    mov [rsp+40],r11
    
    mov r11,[rsp+56]
    mov [rsp+48],r11
   
    mov r11,[rsp+64]
    mov [rsp+56],r11
    
    mov r11,[rsp+72]
    mov [rsp+64],r11
    
    mov r11,[rsp+80]
    mov [rsp+72],r11
    
    mov r11,[rsp+88]
    mov [rsp+80],r11
    
    mov r11,[rsp+96]
    mov [rsp+88],r11
    
    syscall
    ret
    
Sys_NtReadFile:
    mov eax,ecx
    mov r10,rdx
    mov rdx,r8
    mov r8,r9
    mov r9,[rsp+40]
    
    mov r11,[rsp+48]
    mov [rsp+40],r11
    
    mov r11,[rsp+56]
    mov [rsp+48],r11
   
    mov r11,[rsp+64]
    mov [rsp+56],r11
    
    mov r11,[rsp+72]
    mov [rsp+64],r11
    
    mov r11,[rsp+80]
    mov [rsp+72],r11
    
    syscall
    ret
    
Sys_NtQueryInformationFile:
    mov eax,ecx
    mov r10,rdx
    mov rdx,r8
    mov r8,r9
    mov r9,[rsp+40]
    
    mov r11,[rsp+48]
    mov [rsp+40],r11
    
    syscall
    ret
    
Sys_NtDeviceIoControlFile:
    mov eax,ecx
    mov r10,rdx
    mov rdx,r8
    mov r8,r9
    mov r9,[rsp+40]
    
    mov r11,[rsp+48]
    mov [rsp+40],r11
    
    mov r11,[rsp+56]
    mov [rsp+48],r11
   
    mov r11,[rsp+64]
    mov [rsp+56],r11
    
    mov r11,[rsp+72]
    mov [rsp+64],r11
    
    mov r11,[rsp+80]
    mov [rsp+72],r11
    
    mov r11,[rsp+88]
    mov [rsp+80],r11
    
    syscall
    ret 
    
Sys_NtWriteFile:
    mov eax,ecx
    mov r10,rdx
    mov rdx,r8
    mov r8,r9
    mov r9,[rsp+40]
    
    mov r11,[rsp+48]
    mov [rsp+40],r11
    
    mov r11,[rsp+56]
    mov [rsp+48],r11
   
    mov r11,[rsp+64]
    mov [rsp+56],r11
    
    mov r11,[rsp+72]
    mov [rsp+64],r11
    
    mov r11,[rsp+80]
    mov [rsp+72],r11
    
    syscall
    ret 
