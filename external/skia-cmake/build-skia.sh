#!/bin/sh

BINARY_DIR=$PWD

DEPOT_TOOLS_REMOTE="https://chromium.googlesource.com/chromium/tools/depot_tools.git"
DEPOT_TOOLS_SRC_DIR=$BINARY_DIR/depot_tools

SKIA_REMOTE="https://github.com/google/skia.git"
SKIA_SRC_DIR=$BINARY_DIR/skia

VIRTUAL_ENV_EXE=virtualenv2
PYTHON_EXE=python2

while [ "$1" != "" ]; do
    PARAM=`echo $1 | awk -F= '{print $1}'`
    VALUE=`echo $1 | awk -F= '{print $2}'`
    case $PARAM in
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

echo ""
echo "---------------------------"
echo "BINARY_DIR          = $BINARY_DIR"
echo "DEPOT_TOOLS_SRC_DIR = $DEPOT_TOOLS_SRC_DIR"
echo "SKIA_SRC_DIR        = $SKIA_SRC_DIR"
echo "VIRTUAL_ENV_EXE     = $VIRTUAL_ENV_EXE"
echo "PYTHON_EXE          = $PYTHON_EXE"
echo "---------------------------"
echo ""

if [ ! -d "$DEPOT_TOOLS_SRC_DIR" ]; then
  echo "Cloning depot tools into: '$DEPOT_TOOLS_SRC_DIR'"
  git clone $DEPOT_TOOLS_REMOTE $DEPOT_TOOLS_SRC_DIR
else
  echo "Skipping clone of depot tools: Directory '$DEPOT_TOOLS_SRC_DIR' already exists"
fi

if [ ! -d "$SKIA_SRC_DIR" ]; then
  echo "Cloning depot tools into: '$SKIA_SRC_DIR'"
  git clone $SKIA_REMOTE $SKIA_SRC_DIR
else
  echo "Skipping clone of skia: Directory '$SKIA_SRC_DIR' already exists"
fi

echo Entering python virtualenv: $BINARY_DIR/python2-virtual-env
$VIRTUAL_ENV_EXE --python=$PYTHON_EXE python2-virtual-env

# enter python2 environment
source $BINARY_DIR/python2-virtual-env/bin/activate

# sync skia dependencies
PATH="${DEPOT_TOOLS_SRC_DIR}/:$PATH"
cd ${SKIA_SRC_DIR}
${PYTHON_EXE} tools/git-sync-deps

# configure
cd $SKIA_SRC_DIR
$SKIA_SRC_DIR/bin/gn gen out/Shared --args='is_official_build=true is_component_build=true skia_use_icu=false skia_use_sfntly=false extra_cflags_cc=["-Wno-error"]'

# build
cd $SKIA_SRC_DIR/out/Shared
ninja

cp $SKIA_SRC_DIR/out/Shared/libskia.so $BINARY_DIR

#leave python2 environment
deactivate

