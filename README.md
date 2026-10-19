LVGL v9.5 (lv_port_linux-v9.5)  

=================================================================  
Compiler: gcc-linaro-12.1.1-2022.08-x86_64_aarch64-linux-gnu  

build:  
unzip file thirdpart.tar.xz  
make  

cmake compilation error.  
gitclone https://github.com/lvgl/lv_port_linux  
I only modified the cross-compilation. cmake gives an error when compiling.  
but make (makefile) works fine. I don't know why.  

This is a reference example, and a lot of problems haven't been solved.  

Directory:  
lv_port_linux-release-v9.5/  
├── build/  
│ ├── bin/  
│ └── obj/  
├── docker/  
├── example/  
├── lvgl/  
├── scripts/  
├── src/  
│ ├── lib/  
│ ├── ui/  
│ │ ├── sqlite3/      #sqlite3 amalgamation source code  
│ │ ├── ffplay/       #limited ability, copied the ffplay.c code for use.  
│ │ └── deskgui_res/  #resource folder, copy it to the same directory as the executable.  
│ └── main.c  
└── thirdpart/  
 ├── alsa-1.2.15/  
 │ ├── include/  
 │ ├── lib/  
 │ └── share/  
 ├── ffmpeg-9.0.1/  
 │ ├── include/  
 │ ├── lib/  
 │ └── share/  
 ├── sdl-2.32.8/  
 │ ├── bin/  
 │ ├── include/  
 │ ├── lib/  
 │ └── share/  
 ├── zlib/  
 │ ├── include/  
 │ ├── lib/  
 │ └── share/  
 └── sqlite3/        #remove, use sqlite3 amalgamation version.  
 
alsa-1.2.15 cross-compiled
copy the share (alsa.conf) directory to the same directory path on the target board.
sdl cross-compiling requires.
the final lvgl cross-compilation is also needed. 
otherwise you'll get errors linking the SDL staitc library.  

sdl-2.32.8 cross-compiled
make sure alsa is used for the audio decoding library. 
the video library requires too many dependencies, 
so we dropped it and just use LVGL to display the decoded images.

  

