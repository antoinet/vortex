This level is quite straightforward, it's a good way to get confortable with sockets programming in C. Check out [Beej's Guide to C Programming](http://beej.us/guide/bgc/) as well as [Beej's Guide to Network Programming](http://beej.us/guide/bgnet/).

Here's the output you get running the code in vortex.c:

    $ vortex/vortex0 > gcc vortex0.c -o vortex0
    $ vortex/vortex0 > ./vortex0 
    Connecting to vortex.labs.overthewire.org (141.138.199.118)
    Received 16 bytes.
    network: 0x51e9f086 (1374285958)	host: 0x86f0e951 (2263935313)
    network: 0x500d387c (1343043708)	host: 0x7c380d50 (2084048208)
    network: 0x2a98301a (714616858)	host: 0x1a30982a (439392298)
    network: 0x24d401c5 (617873861)	host: 0xc501d424 (3305231396)
    Sum: 0xf1635ae1 (4049820385).
    Response: Username: vortex1 Password: Gq#qu3bF3

