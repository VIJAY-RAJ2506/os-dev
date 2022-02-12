ENTRY(_start)
OUTPUT_FORMAT(binary)
SECTIONS
{
	. = 1M;
	
	.text : ALIGN(4096)
	{
		*kernel.asm.o(.text)
		. = ALIGN(512);
		*(.text)
	}

	.rodata : ALIGN(4096)
	{
		*(.rodata)
	}

	.data : ALIGN(4096)
	{
		*(.data)
	}

	.bss : ALIGN(4096)
	{
		*(.bss)
	}

	.asm : ALIGN(4096)
	{
		*(.asm)
	}


}
