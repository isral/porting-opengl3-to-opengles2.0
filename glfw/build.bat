@echo off

clang-cl main.c  -ferror-limit=1 -FC -GR- -EHa- /EHsc -nologo -Zi /MDd C:/code/libs/lib/x64/glfw3.lib C:/code/libs/lib/x64/glew32.lib opengl32.lib shell32.lib User32.lib Gdi32.lib /link /SUBSYSTEM:CONSOLE
