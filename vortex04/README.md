Obviously, we need to exploit the `printf(...)` call on line 8 for which we can provide an arbitrary format string. This will allow us to set an arbitrary value anywhere in a writeable memory location. 

The first difficulty is bypassing the check on line 7 which makes passing any arguments conventionally on the command line impossible. I'm using an `execle`-wrapper passing an empty argument vector `argv_` and the actual arguments in the environment vector `envp`. These two vectors are adjacent in memory, so accessing `argv[3]`  actually corresponds to accessing `envp[2]`, this is where to actually put the format string parameter. For more information about `execle`, see the [glibc manual](http://www.gnu.org/software/libc/manual/html_node/Executing-a-File.html#Executing-a-File).

The next difficulty is to locate the format string in memory. Unlike the examples from the reading material, the buffer isn't immediately located on the stack. It's necessary to get hold of it from where the environment variables are located; and that's past the lower stack boundary, towards the higher memory addresses. Here's the memory layout:

    Stack       |                 |
                +-----------------+
                |                 |
     argv  ---> +-----------------+
                | Command line    |
                | arguments       |
     environ -> +-----------------+
                | Environment     |
                | variables:      |
                | - Format string |
                | - NOP-slide     |
                | - Shellcode     |
                +-----------------+
                |                 |
    0xffffffff  +=================+   end of memory address space


I wrote some sample programs that display the memory addresses of environment variables, to get a hint of where the shellcode would be located in the process memory (see `getenv.c` and `listenv.c`). This is roughly around `0xffffdXXX`.

Next step consists in choosing a memory location that will ultimately alter `%eip`'s value. I chose to overwrite the GOT's entry for libc's `exit()`, which is called at the end of the vulnerable program:
     $ readelf -r /vortex/vortex4 
     
     Relocation section '.rel.dyn' at offset 0x28c contains 1 entries:
      Offset     Info    Type            Sym.Value  Sym. Name
     08049ff0  00000106 R_386_GLOB_DAT    00000000   __gmon_start__
     
     Relocation section '.rel.plt' at offset 0x294 contains 4 entries:
      Offset     Info    Type            Sym.Value  Sym. Name
     0804a000  00000107 R_386_JUMP_SLOT   00000000   __gmon_start__
     0804a004  00000207 R_386_JUMP_SLOT   00000000   __libc_start_main
     0804a008  00000307 R_386_JUMP_SLOT   00000000   printf
     0804a00c  00000407 R_386_JUMP_SLOT   00000000   exit
The last entry with address `0x0804a00c` is the interesting one, this will be the target address. Following is the format string used to exploit this level:

     \x0c\xa0\x04\x08\x0d\xa0\x04\x08\x0e\xa0\x04\x08\x0f\xa0\x04\x08A%108\$n%5\$0206d%109\$n%5\$032d%110\$n%111\$n

Note: to avoid such a vulnerability, you should use `printf("%s", argv[3])` instead. Refer to the suggested reading material for details.

