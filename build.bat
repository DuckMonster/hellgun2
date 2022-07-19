@echo off
msbuild -v:m -nologo -maxCpuCount hellgun.vcxproj -p:Configuration=debug -p:Platform=x64 -t:Build