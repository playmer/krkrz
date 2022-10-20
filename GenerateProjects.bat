cmake -A Win32 -DVCPKG_TARGET_TRIPLET=x86-windows-static -DVCPKG_CRT_LINKAGE=static -B buildx86
cmake -A x64 -DVCPKG_TARGET_TRIPLET=x64-windows-static -DVCPKG_CRT_LINKAGE=static -B buildx64