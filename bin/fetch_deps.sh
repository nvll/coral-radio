#!/bin/sh

OS=`uname`
if [ "$OS" = "Darwin" ]; then
    OS=mac
fi

function extract () {
    FILE=$1
    OUT_DIR=$2
    EXT=$(echo $FILE | awk -F . '{print $NF}')

    mkdir -p $OUT_DIR
    echo "Extracting $FILE to $OUT_DIR"
    # Generally assume our archives are .zip .tgz .tar.gz .tar.bz2 .tar.xz
    if [ "$EXT" = "zip" ]; then
        unzip -qqo third_party/archives/$FILE -d $OUT_DIR && touch third_party/archives/.$FILE.extracted
    else
        tar xf third_party/archives/$FILE -C $OUT_DIR && touch third_party/archives/.$FILE.extracted
    fi
}

function check_fetch_extract () {
    URL=$1
    OUT_DIR=third_party/$2
    FILE=$(echo $URL | awk -F / '{print $NF}')

    mkdir -p $OUT_DIR
    if [ ! -f third_party/archives/.$FILE.downloaded ]; then
        echo "Fetching $URL"
        curl -s -L -o third_party/archives/$FILE $URL && touch third_party/archives/.$FILE.downloaded

        # If we downloaded a new file for this archive then clear the old directory out
        if [ "$?" = "0" ]; then
            rm -f third_party/archives/.$FILE.extracted
        fi
    fi


    if [ ! -f third_party/archives/.$FILE.extracted ]; then
        extract $FILE $OUT_DIR
    fi
}

while getopts c arg
do
    case $arg in
        c ) CLEAN=1 ;;
        * ) echo "unrecognized option '$arg'" ; exit 1 ;;
    esac
done

if [ "$CLEAN" = "1" ]; then
    rm -rf third_party
    exit 0
fi

mkdir -p third_party/archives

NRF51_URL="https://developer.nordicsemi.com/nRF5_SDK/nRF51_SDK_v10.x.x/nRF51_SDK_10.0.0_dc26b5e.zip"
check_fetch_extract $NRF51_URL nrf51_sdk

GCC_URL="https://launchpad.net/gcc-arm-embedded/5.0/5-2015-q4-major/+download/gcc-arm-none-eabi-5_2-2015q4-20151219-$OS.tar.bz2"
# GCC always contains a container directory, so omit it here.
check_fetch_extract $GCC_URL
