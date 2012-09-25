overthewire.org vortex level 7
==============================

The vulnerability exposed in [vortex level 7][vortex7] is a
basic buffer overflow with two subtleties:

1. The CRC of the buffer must equate to a given value (0xe1ca95ee)
2. The buffer is rather small (58 bytes)


Manipulating the checksum
-------------------------

The [cyclic redundancy check (CRC)][wiki-crc] computes a check  value (or checksum), which is used to detect 
*accidental* changes in data, e.g. when transmitting over unreliable communication channels. With this
error detecting code, the slightest change (i.e. bit-flip) in the input data results in a very different
output pattern. As opposed to [cryptographic hash functions][wiki-crypto-hash] like SHA1 or MD5, 
preimage resistance is not a property of CRC, it is not designed to withstand 
[preimage attacks][wiki-preimage-attacks]: given a checksum *C*, it is not hard to 
find an input *m* such that *CRC(m) = C*. As such, one shouldn't rely on it for integrity checks over 
insecure channels since it is very easy to manipulate it as is shown in the solution.

To solve this level, I chose to apply the CRC-reversing algorithm described in 
[Reversing CRC – Theory and Practice][SAR-PR-2006-05], 
which by the way also contains a very nice introduction to CRC. The method consists in appending a 32-bit 
pattern to the buffer in order to adjust the CRC-remainder to the desired checksum. The same principle is 
proposed in the suggested lecture, [CRC and how to Reverse it][reverse-crc]. 
But as opposed to the first approach which uses the inverse of the divisor polynomial, the bit pattern is 
derived using a system of equations.


Overflowing the buffer
----------------------

This level contains a classic vulnerability which can easily be exploited to execute arbitrary code: a 
[buffer overflow][wiki-buffer-overflow]. The use of the `strcpy` standard library
function to copy a buffer of data to another completely disregards the destination's capacity. If the
source buffer is larger than the destination, all bytes will be copied, even though the destination's
bound has been exceeded, and in doing so, subsequent structures in memory will be destructed.

Intercepting the instruction pointer
------------------------------------

Depending where the destination buffer is located in the process memory, it may be possible for an
attacker to take influence on the program execution flow. In this case, the destination buffer is
on the stack. By overflowing the buffer, copying bytes past its bound, the stored `eip` value will
be overwritten. This is a pointer to the next instruction to return to after leaving the current
stack frame, i.e. when returning from the current function call. With a meaningful value, it is possible
to redirect the execution of the program to any executable location in memory.

Making the Shellcode
--------------------

The payload we want to execute consists in a small fragment of x86 machine instructions, which perform
2-3 syscalls that allow us to run a shell:
* `geteuid()`/`setreuid()` are used to set the effective user-id. The exploited binary runs with 
   the suid-bit, which means the process is executed in the name of the file owner (the user that
   has read-privileges for the next level's password file).
* `execve()` is called to run */bin/bash*.

The original x86/asm code can be found [here](/antoinet/vortex/blob/master/shellcode/shellcode.s). Check
out the [Makefile](/antoinet/vortex/blob/master/shellcode/Makefile) to see how it is compiled and the raw
instruction data is extracted. It is then necessary to encode the data to avoid specific patterns such
as `\0` bytes. I used [metasploit][metasploit]'s `msfencode` tool for this.


Executing arbitrary code
------------------------

Since the buffer is rather small (58 bytes), it is difficult to dissimulate the
[malicious payload][wiki-shellcode]. An alternative way to include arbitrary data
into the process memory is to define an evironment variable containing the data. It will be accessible
from the beginning of the stack. The buffer must then overflow the saved `eip` value to point
at the corresponding region in memory. Unfortunately, this address cannot be precisely deduced.
Therefore, a common strategy consists in prepending a large number of `nop` instructions before
the shellcode. This extends the *landing platform* of the target address thus increasing the
probability of hitting the shellcode.


The source code
---------------

The finalized exploit is available [here](/antoinet/vortex/blob/master/vortex07/v7_wrapper.c).
It is a C wrapper that prepares a shellcode and the buffer contents and calls the binary
to exploit. It expects two arguments:

1. An offset for the target address, relative to the environment pointer taken from the current
process (the wrapper).
2. An alignment index (0-4) used to align the target address in the buffer.

Following arguments worked for me:

    $ ./v7_wrapper 0 2
    Using address: 0xFFFFD91F
    $ whoami
    vortex8

The password for the next level is retrieved as follows:

    $ cat /etc/vortex_pass/vortex8 
    X70A_gcgl


[vortex7]: http://www.overthewire.org/wargames/vortex/vortex7.shtml
[wiki-crc]: http://en.wikipedia.org/wiki/Cyclic_redundancy_check
[wiki-crypto-hash]: http://en.wikipedia.org/wiki/Cryptographic_hash_function
[wiki-preimage-attacks]: http://en.wikipedia.org/wiki/Preimage_attack
[SAR-PR-2006-05]: http://sar.informatik.hu-berlin.de/research/publications/SAR-PR-2006-05/SAR-PR-2006-05_.pdf
[reverse-crc]: http://www.woodmann.com/fravia/crctut1.htm
[wiki-buffer-overflow]: http://en.wikipedia.org/wiki/Buffer_overflow
[metasploit]: http://framework.metasploit.com/
[wiki-shellcode]: http://en.wikipedia.org/wiki/Shellcode