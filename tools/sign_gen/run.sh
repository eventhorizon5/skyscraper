#!/bin/bash

if [[ $1 == 1 ]]; then
	font="Nimbus Sans L"
	fontsize=64
	fontsize2=38
	filename="signs-sans.zip"
fi
if [[ $1 == 2 ]]; then
	font="Nimbus Sans L Bold"
	fontsize=60
	fontsize2=36
	filename="signs-sans_bold.zip"
fi
if [[ $1 == 3 ]]; then
	font="Nimbus Sans L Condensed"
	fontsize=64
	fontsize2=46
	filename="signs-sans_cond.zip"
fi
if [[ $1 == 4 ]]; then
	font="Nimbus Sans L Bold Condensed"
	fontsize=64
	fontsize2=44
	filename="signs-sans_cond_bold.zip"
fi

#floor numbers
number=1
maxnumber=300

while (( number <= maxnumber ))
do
	echo $number
	nice -n 12 gimp -c -d -i -b "(script-fu-makesign \"$number.jpg\" \"$number\" $fontsize \"$font\" '(0 0 0) '(255 255 255) 128 128)" -b '(gimp-quit 0)'
	number=$((++number))
done

#LL numbers
number=1
maxnumber=9

while (( number <= maxnumber ))
do
	echo LL$number
	nice -n 12 gimp -c -d -i -b "(script-fu-makesign \"LL$number.jpg\" \"LL$number\" $fontsize \"$font\" '(0 0 0) '(255 255 255) 128 128)" -b '(gimp-quit 0)'
	number=$((++number))
done

#P numbers
number=1
maxnumber=9

while (( number <= maxnumber ))
do
	echo P$number
        nice -n 12 gimp -c -d -i -b "(script-fu-makesign \"P$number.jpg\" \"P$number\" $fontsize \"$font\" '(0 0 0) '(255 255 255) 128 128)" -b '(gimp-quit 0)'
        number=$((++number))
done

#B numbers
number=1
maxnumber=9

while (( number <= maxnumber ))
do
	echo B$number
        nice -n 12 gimp -c -d -i -b "(script-fu-makesign \"B$number.jpg\" \"B$number\" $fontsize \"$font\" '(0 0 0) '(255 255 255) 128 128)" -b '(gimp-quit 0)'
        number=$((++number))
done

#letter-based floors
number=1
end=16
while (( number <= end ))
do
	if [ $number == 1 ]; then sym="L"; fi
	if [ $number == 2 ]; then sym="M"; fi
	if [ $number == 3 ]; then sym="R"; fi
	if [ $number == 4 ]; then sym="C"; fi
	if [ $number == 5 ]; then sym="H"; fi
	if [ $number == 6 ]; then sym="X"; fi
	if [ $number == 7 ]; then sym="PR"; fi
	if [ $number == 8 ]; then sym="PH"; fi
	if [ $number == 9 ]; then sym="LL"; fi
	if [ $number == 10 ]; then sym="GL"; fi
	if [ $number == 11 ]; then sym="CL"; fi
	if [ $number == 12 ]; then sym="S"; fi
	if [ $number == 13 ]; then sym="P"; fi
	if [ $number == 14 ]; then sym="B"; fi
	if [ $number == 15 ]; then sym="PL"; fi
	if [ $number == 16 ]; then sym="CC"; fi
	echo $sym
	nice -n 12 gimp -c -d -i -b "(script-fu-makesign \"$sym.jpg\" \"$sym\" $fontsize \"$font\" '(0 0 0) '(255 255 255) 128 128)" -b '(gimp-quit 0)'
	number=$((++number))
done

#elevator operational buttons
number=1
end=5
while (( number <= end ))
do
        if [ $number == 1 ]; then sym="Alarm"; file="Alarm"; fi
        if [ $number == 2 ]; then sym="Close"; file="Close"; fi
        if [ $number == 3 ]; then sym="Open"; file="Open"; fi
        if [ $number == 4 ]; then sym="Stop"; file="Stop"; fi
        if [ $number == 5 ]; then sym="  Call\nCancel"; file="CallCancel"; fontsize2=$((fontsize2 - 6)); fi
        echo $file
        nice -n 12 gimp -c -d -i -b "(script-fu-makesign \"$file.jpg\" \"$sym\" $fontsize2 \"$font\" '(0 0 0) '(255 255 255) 128 128)" -b '(gimp-quit 0)'
        number=$((++number))
done

#create zip archive
echo Creating zip file...
zip -9 $filename *.jpg

#clean up
rm *.jpg
echo done - $filename created

