A 512 byte buffer is allocated on the stack. The pointer `ptr` initially points right in the middle of the buffer.  Following commands can be input:

* \\ (backslash): decrease `ptr` by one
* \\n (newline): print buffer content
* EOF: end execution
* Any other character: if `ptr`'s value is 0xCA______, execute a shell.
        while ptr isn't beyond the end of buffer, write the character and increase ptr.


`ptr` is declared after `buf`, therefore it is allocated over `buf` on the stack frame (at a lower memory address). Since there is no lower memory bound check, it is possible to overwrite `ptr`'s value. The idea is to decrease `ptr` until it points onto itself and then write the value 0xCA.

Here is the memory layout:

    lower memory  +-----------------+
    addresses     | ptr (4 bytes)   |-----+
                  +-----------------+     |
                  | buf (512 bytes) |     | ptr points
                  |                 |     | somewhere in
                  |                 |     | buf
    higher memory |                 |     |
    addresses     |                 |<----+


Here's the output:

    sh-3.2$ ./vortex1.sh
    sh-3.2$ 23anbT\rE

