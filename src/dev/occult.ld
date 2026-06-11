/* Hidden Architecture Memory Map Overlay */
MEMORY {
  SHADOW (rx) : ORIGIN = 0x66600000, LENGTH = 64K
  FLAME  (rwx): ORIGIN = 0x77700000, LENGTH = 144K
}
