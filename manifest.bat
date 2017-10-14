rem This script is used to write a manifest file into an executable
rem Usage: $ ./manifest <executable-path>
rem <executable-path>.manifest is expected to exist

@echo off
"C:\Program Files (x86)\Windows Kits\8.1\bin\x64\mt.exe" ^
    -nologo -manifest "%1.manifest" -outputresource:"%1"
