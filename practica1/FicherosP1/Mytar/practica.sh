#!/bin/bash

if [ ! -x "mytar" ]
then 
	echo "Mytar is not executable or doesn't exist"
	exit 1
fi

if [ -d "tmp" ]
then
	rm -rf tmp
fi

mkdir tmp

cd tmp

echo "Hello world!" > file1.txt
head /etc/passwd > file2.txt
head -c 1024 /dev/urandom > file3.dat

../mytar -cf filetar.mtar file1.txt file2.txt file3.dat

mkdir out

cp filetar.mtar out

cd out
../mytar -xf filetar.mytar

if diff ../tmp/file1.txt file1.txt > /dev/null 
then 
	true
else
	cd ../..
	echo "file1.txt is different"
	exit 1
fi

if diff ../tmp/file2.txt file2.txt > /dev/null
then 
	true
else
	cd ../..
	echo "file2.txt is different"
	exit 1
fi

if diff ../tmp/file3.dat file3.dat > /dev/null 
then 
	true
else
	cd ../..
	echo "file3.dat is different"
	exit 1
fi

cd ../..
echo Correct
exit 0