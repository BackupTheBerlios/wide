Linux
1) install runtime wxgtk2.8
2) make
3) in case of errors uncomment line starting by "WX_LIBS" (see the makefile)


Windows
1) install and compile wxwidgets (using static)
2) compile wide with these parameters:
compiler:
UNICODE=1
-fno-exceptions -fno-pcc-struct-return -fstrict-aliasing -Wall -D__WXMSW__ -D__GNUWIN32__ -D__WIN95__

linker:
-mwindows -lkernel32 -luser32 -lgdi32 -lcomdlg32 -lwinspool -lwinmm -lshell32 -lcomctl32 -lole32 -loleaut32 -luuid -lrpcrt4 -ladvapi32 -lwsock32 -lodbc32 -lopengl32 C:\repository\wide\wide\wide.res
set strip ON

3) to add icon use:
windres wide.rc -O coff -o wide.res
g++ -o wide ...... wide.res


NOTE:
for windows you can use relative path in the res file
for linux you should use absolute path in the res file
due to BLC which is different for platform