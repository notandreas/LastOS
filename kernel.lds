ENTRY(_start)
SECTIONS
{
	. = 0x00015000;
	.init : { *(.init) }
	.text : { *(.text) }
}

