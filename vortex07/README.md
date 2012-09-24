overthewire.org vortex level 7
==============================

The vulnerability exposed in [vortex level 7](http://www.overthewire.org/wargames/vortex/vortex7.shtml) is a
basic buffer overflow with two subtleties:

1. The CRC of the buffer must equate to a given value (0xe1ca95ee)
2. The buffer is quite small (58 bytes)


Manipulating the checksum
-------------------------

The [cyclic redundancy check (CRC)](http://en.wikipedia.org/wiki/Cyclic_redundancy_check) computes a check 
value (or checksum), which is used to detect *accidental* changes in data, e.g. when transmitting over 
unreliable communication channels. With this error detecting code, the slightest error (i.e. bit-flip) in 
the input data results in a very different output. As opposed to 
[cryptographic hash functions](http://en.wikipedia.org/wiki/Cryptographic_hash_function) like SHA1 or MD5, 
preimage resistance is not a property of CRC, it is not designed to withstand 
[preimage attacks](http://en.wikipedia.org/wiki/Preimage_attack): given a checksum *C*, it is not hard to 
find an input *m* such that *CRC(m) = C*. As such, one shouldn't rely on it for integrity checks over 
insecure channels since it is very easy to manipulate it as is shown in the solution.

To solve this level, I chose to apply the CRC-reversing algorithm described in 
[Reversing CRC – Theory and Practice (SAR-PR-2006-05)](http://sar.informatik.hu-berlin.de/research/publications/SAR-PR-2006-05/SAR-PR-2006-05_.pdf), 
which by the way also contains a very nice introduction to CRC. The method consists in appending a 32-bit 
pattern to the buffer in order to adjust the CRC-remainder to the desired checksum. The same principle is 
proposed in the suggested lecture, [CRC and how to Reverse it](http://www.woodmann.com/fravia/crctut1.htm). 
But as opposed to the first approach which uses the inverse of the divisor polynomial, the bit pattern is 
derived using a system of equations.


Overflowing the buffer
----------------------

This level contains a classic vulnerability which can easily be exploited to execute arbitrary code: a 
[buffer overflow](http://en.wikipedia.org/wiki/Buffer_overflow). The use of the `strcpy` standard library
function to copy a buffer of data to another completely disregards the destination's capacity. If the
source buffer is larger than the destination, all bytes will be copied, even though the destination's
bound has been exceeded, and in doing so, subsequent structures in memory will be destructed.

Intercepting the instruction pointer
------------------------------------

Depending where the destination buffer is located in the process memory, it may be possible for an
attacker to take influence on the program execution flow. In this case, the destination buffer is
on the stack. By overflowing the buffer, copying bytes past its bound, the stored `eip`-value will
be overwritten. This is a pointer to the next instruction to return to after leaving the current
stack frame, i.e. when returning from the current function call. With a meaningful value, it is possible
to redirect the execution of the program to any executable location in memory.

Making the Shellcode
--------------------



Executing arbitrary code
------------------------

Since the buffer is rather small (58 bytes), it is difficult to dissimulate the
[malicious payload](http://en.wikipedia.org/wiki/Shellcode). Another way to include arbitrary data
into the process memory is to define an evironment variable containing the data. It will be accessible
from the beginning of the stack.




    $ ./v7_wrapper 0 2
    Using address: 0xFFFFD91F
    $ whoami
    vortex8
    $ cat /etc/vortex_pass/vortex8 
    X70A_gcgl

