@echo off
msbuild -v:m -nologo -maxCpuCount hellgun.vcxproj -p:Configuration=debug -t:Build