Linux
1) install runtime wxgtk2.8
2) make
3) in case of errors uncomment line starting by "WX_LIBS" (see the makefile)
sudo cp libgarglk.so /usr/lib/

install:
libSDL_sound-1.0 (sudo pacman -S sdl_sound)


Windows



NOTE:
for windows you can use relative path in the res file
for linux you should use absolute path in the res file
due to BLC which is different for platform