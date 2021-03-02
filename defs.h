char begin[] = "\
global _start\n\
section .text\n\
\n\
print:\n\
  push rbp\n\
  mov rbp, rsp\n\
  mov rcx, 10\n\
  sub rsp, 8\n\
  cmp eax,0\n\
  jge notNeg\n\
  mov [rsp], byte 45\n\
  dec rsp\n\
  neg rax\n\
  notNeg:\n\
\n\
  top:\n\
  xor rdx,rdx\n\
  div ecx\n\
  add edx, 48\n\
  mov [rsp], dl\n\
  dec rsp\n\
  cmp dword eax, dword 0\n\
  jne top\n\
\n\
  mov rax, 1\n\
  mov rdi, 1\n\
  lea rsi, [rsp+1]\n\
  mov rdx, rbp\n\
  sub rdx, rsp\n\
  dec rdx\n\
  syscall\n\
  mov rsp, rbp\n\
  pop rbp\n\
  ret\n\
\n\
get:\n\
  push rbp\n\
  mov rbp, rsp\n\
  push 0\n\
  push 0\n\
  xor rax, rax\n\
  mov rdi, 0x0  \n\
  lea rsi, [rsp]  \n\
  mov rdx, 0x8  \n\
  ;mov rax, 0x0  \n\
  syscall       \n\
  xor rax, rax\n\
\n\
  mov [rsp + 8], byte 0xa\n\
  mov rcx, 10\n\
\n\
  top_get:\n\
  cmp [rsp], byte 0xa\n\
  je bottom_get\n\
  sub [rsp], byte 48\n\
  mul rcx\n\
  add al, [rsp]\n\
  inc rsp\n\
  jmp top_get\n\
  bottom_get:\n\
\n\
  mov rsp, rbp\n\
  pop rbp\n\
  ret\n\
  \n\
newline:\n\
  push rbp\n\
  mov rbp, rsp\n\
  push 10\n\
  mov rax, 1\n\
  mov rdi, 1\n\
  mov rsi, rsp\n\
  mov rdx, 1\n\
  syscall\n\
  mov rsp, rbp\n\
  pop rbp\n\
  ret\n\
printc:\n\
  push rbp\n\
  mov rbp, rsp\n\
  push rax\n\
  mov rax, 1\n\
  mov rdi, 1\n\
  mov rsi, rsp\n\
  mov rdx, 1\n\
  syscall\n\
  mov rsp, rbp\n\
  pop rbp\n\
  ret\n\
\n\
\n\
\n\
_start:\n\
  mov rbp, rsp\n\
  mov [rsp], word 0\n";
