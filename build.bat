@echo off
setlocal EnableDelayedExpansion
set FLAGS=-g -gfull -O0 -Isrc -Iinclude -Iimgui/src -Llib -Xlinker /NODEFAULTLIB -includesrc/core/pch.h
set WARNING=-Wno-int-to-void-pointer-cast

set LIBS=-lopengl32.lib -lglfw3.lib -luser32.lib -llibcmt.lib -lucrt.lib -lvcruntime.lib -lKernel32.lib -lGdi32.lib -lShell32.lib
set DEFINES=-DSDL_MAIN_HANDLED
set FILES=

pushd src
for /r %%f in (*.cpp) do (
	set FILES=!FILES! %%f
)
popd

clang -o bin/hellgun.exe %FLAGS% %WARNING% %DEFINES% %FILES% %LIBS%