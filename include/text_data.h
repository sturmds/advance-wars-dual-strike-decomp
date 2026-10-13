#ifndef TEXT_DATA_H
#define TEXT_DATA_H

/* Data that the original keeps in .text, between functions (the module
 * parameters after crt0, tables in ITCM). Put its definition between
 *
 *   #pragma section TEXT_DATA begin
 *   #pragma section TEXT_DATA end
 *
 * mwcc then emits it in a section named .text, which the linker script places
 * with the object's code. The pragma moves initialized data only: a const
 * object stays in .rodata whatever the section says, so these definitions
 * are not const. */
#pragma define_section TEXT_DATA ".text" abs32 RWX

#endif
