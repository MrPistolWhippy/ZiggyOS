void scan_pci_bus() {
	unsigned int addr = 0x0;
	volatile char* p = (volatile char*)addr;
	for(int b=0; b<8; b++) {
		*p = '0' + b; print(" Slot ");
		for(int s=0; s<32; s++) {
			*p = (s / 10) ? '0' + (s / 10) : '0'; *p = '0' + (s % 10);
		}
	}
}
