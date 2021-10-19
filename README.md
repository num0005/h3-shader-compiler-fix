# h3-shader-compiler-fix
A small DLL that fixes tool's usage of the Halo 3 shader compiler.

Tool forgot to initialise the compiler before using it, so this DLL initialises the compiler if it hasn't been before any call the requires it.
