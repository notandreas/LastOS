ENTRY(_start)
SECTIONS
{
	. = 0x00388000;
	.init : { *(.init) }
	.text : { *(.text) }
}

