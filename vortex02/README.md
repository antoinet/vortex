Both the password file and the vortex2 binary belong to user vortex3. The binary also has the SUID-bit set for execution, this means the process will be run as user vortex3, which is the only user which has read/write permissions on the password file.

    $ ls -al /etc/vortex_pass/vortex3
    -r-------- 1 vortex3 vortex3 10 2011-11-14 18:15 /etc/vortex_pass/vortex3
    $ ls -la /vortex/vortex2
    -r-sr-x--- 1 vortex3 vortex2 7134 2011-11-13 23:07 /vortex/vortex2


The call to the vortex2 binary creates a tar-archive in /tmp. I first thought $$ would be evaluated to the PID of the running shell, in order to create one file per bash-user, but actually the file name is treated literally.

One can specify up to 3 additional arguments to tar, one is enough to specify which file to include in the archive.

