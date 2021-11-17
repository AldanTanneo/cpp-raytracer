@echo off
@echo Building main.exe...
clang++ -o main.exe src/*.cpp -Isrc/include -Wall -Werror -Wfatal-errors
@echo Done!
