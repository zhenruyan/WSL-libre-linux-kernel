.. SPDX-License-Identifier: GPL-2.0

=============
Old Microcode
=============

The kernel keeps a table of released microcode. Systems that had
microcode older than this at boot will say "Vulnerable".  This means
that the system was vulnerable to some known CPU issue. It could be
security or functional, the kernel does not know or care.

/*(DEBLOBBED)*/
