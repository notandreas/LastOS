ENTRY(_start)
SECTIONS
{
	. = 0x00160000;
	.init : { *(.init) }
	.text : { *(.text) }
}

