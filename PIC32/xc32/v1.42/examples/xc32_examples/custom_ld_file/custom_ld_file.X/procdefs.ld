/*************************************************************************
 * Processor-specific object file.  Contains SFR definitions.
 *************************************************************************/
INPUT("processor.o")

/*************************************************************************
 * Processor-specific peripheral libraries are optional
 *************************************************************************/
OPTIONAL("libmchp_peripheral.a")
OPTIONAL("libmchp_peripheral_32MX360F512L.a")

/*************************************************************************
 * For interrupt vector handling
 *************************************************************************/
PROVIDE(_vector_spacing = 0x00000001);
_ebase_address  = 0x9FC01000;

/*************************************************************************
 * Memory Address Equates
 *************************************************************************/
_RESET_ADDR              = 0xBFC00000;
_BEV_EXCPT_ADDR          = 0xBFC00380;
_DBG_EXCPT_ADDR          = 0xBFC00480;
_DBG_CODE_ADDR           = 0xBFC02000;
_DBG_CODE_SIZE           = 0xFF0     ;
_GEN_EXCPT_ADDR          = _ebase_address + 0x180;

/*************************************************************************
 * Memory Regions
 *
 * Memory regions without attributes cannot be used for orphaned sections.
 * Only sections specifically assigned to these regions can be allocated
 * into these regions.
 *************************************************************************/
MEMORY
{
  kseg0_program_mem    (rx)  : ORIGIN = 0x9D000000, LENGTH = 0x7D000	/* Reduce this section by my_reserved_flash size */
  my_reserved_flash	         : ORIGIN = 0x9D07D000, LENGTH = 0x3000	    /* Name your memory with desired size */
  kseg0_boot_mem             : ORIGIN = 0x9FC00490, LENGTH = 0x970
  exception_mem              : ORIGIN = 0x9FC01000, LENGTH = 0x1000
  kseg1_boot_mem             : ORIGIN = 0xBFC00000, LENGTH = 0x490
  debug_exec_mem             : ORIGIN = 0xBFC02000, LENGTH = 0xFF0
  config3                    : ORIGIN = 0xBFC02FF0, LENGTH = 0x4
  config2                    : ORIGIN = 0xBFC02FF4, LENGTH = 0x4
  config1                    : ORIGIN = 0xBFC02FF8, LENGTH = 0x4
  config0                    : ORIGIN = 0xBFC02FFC, LENGTH = 0x4
  kseg1_data_mem       (w!x) : ORIGIN = 0xA0000000, LENGTH = 0x8000
  sfrs                       : ORIGIN = 0xBF800000, LENGTH = 0x100000
}
SECTIONS
{
  .config_BFC02FF0 : {
    *(.config_BFC02FF0)
  } > config3
  .config_BFC02FF4 : {
    *(.config_BFC02FF4)
  } > config2
  .config_BFC02FF8 : {
    *(.config_BFC02FF8)
  } > config1
  .config_BFC02FFC : {
    *(.config_BFC02FFC)
  } > config0

  /* Define what sections should be placed in my_reserved_flash memory */
  .my_space : {
  my_reserved_flash_addr = .;						/* This variable can be used in 'C' or assembly using extern */
   *(.my_space)
  } > my_reserved_flash
}
