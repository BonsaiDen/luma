#!/bin/sh

# Utility =====================================================================
# =============================================================================
git_clone() {
    local url=$1
    local target=$2
    git clone $url --depth=1 $target > /dev/null 2>&1
}

tar_fetch() {
    local archive="$2.tar.gz"
    local url="$1$archive"
    local name=$2
    local target=$3
    wget $url > /dev/null 2>&1
    tar xvfz $archive > /dev/null 2>&1
    rm -rf $target
    mv $name $target > /dev/null 2>&1
    rm $archive > /dev/null 2>&1
}


# Audio =======================================================================
# =============================================================================
build_portaudio(){
    rm -rf portaudio > /dev/null 2>&1
    echo " # Audio (portaudio)"
    echo " |- Downloading..."
    svn co https://subversion.assembla.com/svn/portaudio/portaudio/trunk@1788 portaudio > /dev/null 2>&1
    cd portaudio 
    make clean > /dev/null 2>&1

    echo " |- Configuring..."
    ./configure > /dev/null 2>&1
    echo " |- Building..."

    make > /dev/null 2>&1
    cd .. 

    echo " \- Done"
    echo ""
}


# Graphics ====================================================================
# =============================================================================
build_glfw(){
    rm -rf glfw > /dev/null 2>&1
    echo " # Graphics (glfw)"
    echo " |- Downloading..."
    git_clone "https://github.com/glfw/glfw.git" "glfw"
    cd glfw 
    rm -rf build > /dev/null 2>&1

    echo " |- Configuring..."
    mkdir build > /dev/null 2>&1
    cd build 
    cmake .. > /dev/null 2>&1

    echo " |- Building..."
    make > /dev/null 2>&1
    cd .. 
    cd .. 

    echo " \- Done"
    echo ""
}


# Scripting ===================================================================
# =============================================================================
build_lua(){
    rm -rf lua > /dev/null 2>&1
    echo " # Scripting (lua)"
    echo " |- Downloading..."
    tar_fetch "http://www.lua.org/ftp/" "lua-5.2.2" "lua"
    cd lua
    make clean > /dev/null 2>&1

    echo " |- Building..."
    make linux > /dev/null 2>&1
    cd .. 

    echo " \- Done"
    echo ""
}


# Support =====================================================================
# =============================================================================
build_zlib() {
    rm -rf zlib > /dev/null 2>&1
    echo " # Support (zlib)"
    echo " |- Downloading..."
    git_clone "https://github.com/madler/zlib.git" "zlib"
    cd zlib

    echo " |- Configuring..."
    make clean > /dev/null 2>&1
    ./configure > /dev/null 2>&1

    echo " |- Building..."
    make > /dev/null 2>&1
    cd .. 

    echo " \- Done"
    echo ""
}

build_libpng() {

    rm -rf libpng > /dev/null 2>&1

    ZLIBLIB=`pwd`/zlib export ZLIBLIB
    ZLIBINC=`pwd`/zlib export ZLIBINC
    CPPFLAGS="-I$ZLIBINC" export CPPFLAGS
    LDFLAGS="-L$ZLIBLIB" export LDFLAGS
    LD_LIBRARY_PATH="$ZLIBLIB:$LD_LIBRARY_PATH" export LD_LIBRARY_PATH

    echo " # Support (libpng)"
    echo " |- Downloading..."

    # git_clone "git://git.code.sf.net/p/libpng/code" "libpng" # Does not work due to missing pre-built configure
    tar_fetch "http://download.sourceforge.net/libpng/" "libpng-1.6.2" "libpng"
    cd libpng

    echo " |- Configuring..."
    make clean > /dev/null 2>&1
    ./configure > /dev/null 2>&1
    echo " |- Building..."
    make > /dev/null 2>&1
    cd .. 

    echo " \- Done"
    echo ""
}

copy_headers() {
    cp glfw/include/GLFW/glfw3.h ../deps/include
    cp glfw/include/GLFW/glfw3native.h ../deps/include

    cp portaudio/include/portaudio.h ../deps/include

    cp zlib/zlib.h ../deps/include
    cp zlib/zconf.h ../deps/include

    cp libpng/png.h ../deps/include
    cp libpng/pngconf.h ../deps/include
    cp libpng/pnglibconf.h ../deps/include

    cp lua/src/lua.h ../deps/include
    cp lua/src/luaconf.h ../deps/include
    cp lua/src/lualib.h ../deps/include
    cp lua/src/lauxlib.h ../deps/include
}

copy_libs() {
    cp glfw/build/src/libglfw3.a ../deps/lib
    cp portaudio/lib/.libs/libportaudio.a ../deps/lib
    cp zlib/libz.a ../deps/lib
    cp libpng/.libs/libpng16.a ../deps/lib
    cp lua/src/liblua.a ../deps/lib
}

build_clean() {
    rm /deps/include/*.h > /dev/null 2>&1
    rm /deps/lib/*.a > /dev/null 2>&1
}

build_dependencies() {

    echo "=== Building Dependencies ==="
    build_clean
    cd external

    build_lua
    build_glfw
    build_portaudio
    build_zlib
    build_libpng

    setup_project
    cd .. 
    
}

setup_project() {
    echo "=== Setting up Project ==="
    echo " |- Copying headers..."
    copy_headers
    echo " |- Copying libs"
    copy_libs
    echo " \- Done"

    echo ""
    echo "=== Testing ==="

}

#build_dependencies
gcc -o setupTest -Wall -Wextra -Wno-unused  test/*.c deps/lib/*.a -lX11 -lXxf86vm -lGL -lXrandr -lXi -lrt -lasound
./setupTest

