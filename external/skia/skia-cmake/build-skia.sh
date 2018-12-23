#!/bin/sh

BINARY_DIR=$PWD
SRC_DIR=$PWD

VIRTUAL_ENV_EXE=virtualenv2
PYTHON_EXE=python2

while [ "$1" != "" ]; do
    PARAM=`echo $1 | awk -F= '{print $1}'`
    VALUE=`echo $1 | awk -F= '{print $2}'`
    case $PARAM in
        --src-dir)
            SRC_DIR=$VALUE
            ;;
        --binary-dir)
            BINARY_DIR=$VALUE
            ;;
        --virtual-env)
            VIRTUAL_ENV_EXE=$VALUE
            ;;
        --python)
            PYTHON_EXE=$VALUE
            ;;
        *)
            echo "ERROR: unknown parameter \"$PARAM\""
            usage
            exit 1
            ;;
    esac
    shift
done

DEPOT_TOOLS_SRC_DIR=$SRC_DIR/depot_tools
SKIA_SRC_DIR=$SRC_DIR/skia

echo ""
echo "---------------------------"
echo "BINARY_DIR          = $BINARY_DIR"
echo "DEPOT_TOOLS_SRC_DIR = $DEPOT_TOOLS_SRC_DIR"
echo "SKIA_SRC_DIR        = $SKIA_SRC_DIR"
echo "VIRTUAL_ENV_EXE     = $VIRTUAL_ENV_EXE"
echo "PYTHON_EXE          = $PYTHON_EXE"
echo "---------------------------"
echo ""

echo Entering python virtualenv: $BINARY_DIR/python2-virtual-env
$VIRTUAL_ENV_EXE --python=$PYTHON_EXE $BINARY_DIR/python2-virtual-env

# enter python2 environment
source $BINARY_DIR/python2-virtual-env/bin/activate

# sync skia dependencies
PATH="${DEPOT_TOOLS_SRC_DIR}/:$PATH"
cd ${SKIA_SRC_DIR}
${PYTHON_EXE} tools/git-sync-deps

# configure
cd $SKIA_SRC_DIR
$SKIA_SRC_DIR/bin/gn gen out/Shared --args='is_official_build=true is_component_build=true skia_use_sfntly=false extra_cflags_cc=["-Wno-error"]'
# skia_use_icu=false 
# build
cd $SKIA_SRC_DIR/out/Shared
ninja

if [ ! -d "$BINARY_DIR/lib" ]; then
    mkdir $BINARY_DIR/lib/
fi
cp $SKIA_SRC_DIR/out/Shared/libskia.so $BINARY_DIR/lib/

#leave python2 environment
deactivate

