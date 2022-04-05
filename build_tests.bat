@echo off
pushd tests

for /r %%f in (*.cpp) do (
	echo %%f
	set OUTPUT=%%f
	set OUTPUT=!%%f:.cpp=.exe!
	clang -o !OUTPUT! %%f
)

popd