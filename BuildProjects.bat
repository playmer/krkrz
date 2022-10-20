call GenerateProjects.bat

cmake --build buildx86 --config Release
cmake --build buildx64 --config Release
robocopy buildx64/Release output tvpwin64.exe
robocopy buildx86/Release output tvpwin32.exe