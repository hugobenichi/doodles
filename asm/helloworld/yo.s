# compile with $ as yo.s -o yo.o
# link with $ ld -lc -macosx_version_min 10.8.5 yo.o -o yo
.data
_hello:
  .asciz "hello world %i\n"

.text
.globl _main
_main:                      # only rbx, rsp, rbp, r12 - r15 are preserved across 
  subq $8, %rsp             # function calls

  movq $40, %rdi
  call _putchar
  movq $94, %rdi
  call _putchar
  movq $95, %rdi
  call _putchar
  movq $94, %rdi
  call _putchar
  movq $41, %rdi
  call _putchar
  movq $10, %rdi
  call _putchar

  movb $1, %al              # %al is 1B wide, so mov becomes movb
  leaq _hello(%rip), %rdi
  movq $42, %rsi            # arguments are read from rdi, rsi, rdx, rcx, r8, r9
  call _printf

  movq $0, %rdi
  call _exit
