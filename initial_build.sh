#!/bin/bash
set -e
cd external/jpeg-8d

echo "**********************************"
echo "   Configuring the jpeg library   "
echo "**********************************"

./configure

echo "*******************************"
echo "   Building the jpeg library   "
echo "*******************************"

make

cd ..
cd mxml

echo "**********************************"
echo "   Configuring the mxml library   "
echo "**********************************"

case `uname` in
    CYGWIN*)
        ./configure CFLAGS="-DWIN32" ;;
    *)
        ./configure;;
esac

echo "*******************************"
echo "   Building the mxml library   "
echo "*******************************"

make

cd ../..

echo "*******************************"
echo "   Building image_meta_tools   "
echo "*******************************"

make

echo "**************"
echo "     Done     "
echo "**************"