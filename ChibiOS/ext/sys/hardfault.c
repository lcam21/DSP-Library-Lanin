void HardFault_Handler(void) __attribute__( ( naked, noreturn ) );

void HardFault_Handler(void) {
  __asm__ volatile(
      " tst lr, #4   \n"
      " ite eq       \n"
      " mrseq r0, psp\n"
      " mrsne r0, msp\n"
      " mov   r0, sp \n"
  );
  while(1)
    __asm__ volatile("bkpt");
}
