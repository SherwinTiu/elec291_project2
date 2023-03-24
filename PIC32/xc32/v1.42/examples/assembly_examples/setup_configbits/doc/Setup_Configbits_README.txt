This examples shows how to put configuration bits in assembly only project when using the
MPLAB IDE menu option or the "#pragma" options are not available.

The config.s file under "source" shows how to setup the configuration bits.

To verify, dump the ELF output of the project and use the following command to view the
configuration word locations which are properly set:
	> xc32-objdump.exe -Dr setup_configbits.elf | less
and look for "config" pattern.
