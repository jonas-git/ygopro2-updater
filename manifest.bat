@echo off
"C:\Program Files (x86)\Windows Kits\8.1\bin\x64\mt.exe" ^
    -nologo -manifest "%1.manifest" -outputresource:"%1"
