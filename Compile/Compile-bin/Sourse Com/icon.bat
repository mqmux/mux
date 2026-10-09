cd /d %~dp1
echo A ICON %2  >prog.rc
windres -i prog.rc --input-format=rc -o prog.res -O coff
g++ -c -o prog.o %1
g++ prog.o prog.res -o "%~n1"&& (  echos 0x0a ====±‡“Î≥…π¶==== &pause ) || (  echos 0x0c ====±‡“Î ß∞‹==== &pause )
del prog.rc prog.res prog.o