@echo off
IF "%1"=="--release" (
    @echo Building main.exe [release mode]
    clang++ -o main.exe src/*.cpp -Isrc/include -Wall -Werror -Wfatal-errors -O3
) ELSE (
    @echo Building main.exe [debug mode]
    clang++ -o main.exe src/*.cpp -Isrc/include -Wall -Werror -Wfatal-errors -O0 %*
)
@echo Done!
