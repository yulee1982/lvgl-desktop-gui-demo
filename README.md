LVGL v9.5 (lv_port_linux-v9.5)

=================================================================
Compiler: gcc-linaro-12.1.1-2022.08-x86_64_aarch64-linux-gnu
build: make

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
│ │ └── deskgui_res/  #resource folder, copy it to the same directory as the final executable.
│ └── main.c
└── thirdpart/
  ├── ffmpeg-9.0.1/
  │ ├── include/
  │ ├── lib/          #lib.tar.xz: unzip the cross-compiled static library.
  │ └── share/
  └── sqlite3/        #remove, use sqlite3 amalgamation version.

  

