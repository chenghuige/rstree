#! /bin/bash
############################################
#
# Author:
# Create time: 2015 8ÔÂ 07 09Ê±43·Ö48Ãë
# E-Mail: jiaxiaojian@baidu.com
# version 1.1
#
############################################
export MAC=64
export PATH=/opt/compiler/gcc-4.8.2/bin:$PATH
comake2 -UB -F 
if [ "$?" -ne 0 ]
then
  echo "comake2 failed!"
  cd ../../../../../public/ub
  comake2 -UB  -F
  if [ "$?" -ne 0 ]
  then
      echo "comake2 ub alonely failed!"
  fi

  make clean && make 
  if [ "$?" -ne 0 ]
  then
      echo "make ub alonely failed!"
  fi
  cd -
fi
comake2 -UB

if [ ! -d "../../../ksarch/store/proxy-lib/redis/output/lib/libredisclient.a" ]
then
    cd ../../../ksarch/store/proxy-lib/redis/
    make clean 
    make    
    cd -    
fi

if [ ! -d "../gezi/output/" ]
then
    cd ../gezi/
    make clean
    make 
    cd -
fi

comake2
if [ "$?" -ne 0 ]
then
    echo "comake2 2 failed!"
fi
make clean && make -j 8
if [ "$?" -ne 0 ]
then
    echo "make rstree  failed!"
fi


#find ./ -type d -name .svn |xargs -i rm -rf {}
