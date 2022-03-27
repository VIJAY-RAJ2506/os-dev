ENTRY(_start)
OUTPUT_FORMAT(binary)
SECTIONS
{
	. = 1M;
	
	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.text)
	}

	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)
	}

	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
	}

	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(.bss)
	}

	.asm BLOCK(4K) : ALIGN(4K)
	{
		*(.asm)
	}


}
