##  Assembly Files

On PlatformIO, assembling `CortexContextSwitch.s` gives error `arm-none-eabi-as: unrecognized option '-x'`. We use the `CortexContextSwitch.s.o` compiled from Codal.

On Codal, `CortexContextSwitch.s` is always assembled and linked.  `CortexContextSwitch.s.o` is never used.
