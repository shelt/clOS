ENTRY(kernel_mode)
SECTIONS
{
	. = 1M;
	kernel_start = .;

	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.mboot_header)
		*(.text)
	}

	/* Read-only data. */
	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)
	}

	/* Read-write data (initialized) */
	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
	}

	/* bootstrap stack (uninitialized) */
	.bss BLOCK(4K) : ALIGN(4K)
	{
		bss_start = .;
		*(COMMON)
		*(.bss)
		bss_end = .;
	}

	kernel_end = ABSOLUTE(.);
}
