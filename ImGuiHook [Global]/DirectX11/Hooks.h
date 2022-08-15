#pragma once
bool hooked = false;
DWORD64 HookAddr;
DWORD64 ViewAddr;
DWORD64 jmpback;

__declspec(naked) void GetView() {
	__asm{
		mov[ViewAddr], rax
		movss[rbp + 0x00], xmm0
		movaps xmm4, [rax]
		movaps xmm5, [rax + 0x10]
		movaps xmm6, [rax + 0x20]
		jmp[jmpback]
	}
}