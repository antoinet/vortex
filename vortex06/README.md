vortex level 6 consists in reverse engineering the binary executable. objdump provides
a very good decompiler function:

    $ objdump -d vortex6.bin

    08048424 <restart>:
     8048424:       55                      push   %ebp
     8048425:       89 e5                   mov    %esp,%ebp
     8048427:       83 ec 18                sub    $0x18,%esp
     804842a:       c7 44 24 08 00 00 00    movl   $0x0,0x8(%esp)
     8048431:       00
     8048432:       8b 45 08                mov    0x8(%ebp),%eax           -- load argv into %eax
     8048435:       89 44 24 04             mov    %eax,0x4(%esp)           -- push argv on the stack
     8048439:       8b 45 08                mov    0x8(%ebp),%eax           -- load argv into %eax (again)
     804843c:       89 04 24                mov    %eax,(%esp)              -- push argv on the stack
     804843f:       e8 00 ff ff ff          call   8048344 <execlp@plt>     -- execlp(argv, argv)
     8048444:       c9                      leave
     8048445:       c3                      ret

    08048446 <main>:
     8048446:       55                      push   %ebp
     8048447:       89 e5                   mov    %esp,%ebp
     8048449:       83 e4 f0                and    $0xfffffff0,%esp         -- align %esp to 4-byte boundary
     804844c:       83 ec 10                sub    $0x10,%esp               -- alloc 16 bytes in the current frame
     804844f:       8b 45 10                mov    0x10(%ebp),%eax          -- load envp into %eax
     8048452:       8b 00                   mov    (%eax),%eax
     8048454:       85 c0                   test   %eax,%eax                -- if (envp[0] == 0)
     8048456:       74 0d                   je     8048465 <main+0x1f>      --   jmp <0x08048465>
     8048458:       8b 45 0c                mov    0xc(%ebp),%eax           -- load argv into %eax
     804845b:       8b 00                   mov    (%eax),%eax
     804845d:       89 04 24                mov    %eax,(%esp)              -- push argv on the stack (pass as argument
     8048460:       e8 bf ff ff ff          call   8048424 <restart>        -- to restart), call restart
     8048465:       8b 45 10                mov    0x10(%ebp),%eax
     8048468:       83 c0 0c                add    $0xc,%eax
     804846b:       8b 00                   mov    (%eax),%eax
     804846d:       89 04 24                mov    %eax,(%esp)
     8048470:       e8 df fe ff ff          call   8048354 <printf@plt>
     8048475:       c7 04 24 25 73 00 00    movl   $0x7325,(%esp)
     804847c:       e8 b3 fe ff ff          call   8048334 <_exit@plt>


Again, I use a execve-wrapper to control which arguments to pass to the process (see `v6_wrapper.c`):

    vortex6@melissa:/tmp$ ./v6_wrapper
    $ cat /etc/vortex_pass/vortex7
    Y52jxHtt/
