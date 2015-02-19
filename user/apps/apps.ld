PHDRS
{
    code PT_LOAD;
    data PT_LOAD;
}

SECTIONS
{
	. = 0x4000;

	.text :
	{
		*(.text)
	} : code

	.rodata :
	{
		*(.rodata)
	} : data

	.data :
	{
		*(.data) *(.bss) *(COMMON)
	} : data
}
