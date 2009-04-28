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

echo Creating $filename
echo

#floor numbers
number=-9
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

#letter-based floors/symbols
number=1
end=44
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
	if [ $number == 17 ]; then sym="EZ"; fi
	if [ $number == 18 ]; then sym="EX"; fi
	if [ $number == 19 ]; then sym="T"; fi
	if [ $number == 20 ]; then sym="TL"; fi
	if [ $number == 21 ]; then sym="ML"; fi
	if [ $number == 22 ]; then sym="G"; fi
	if [ $number == 23 ]; then sym="LG"; fi
	if [ $number == 24 ]; then sym="UL"; fi
	if [ $number == 25 ]; then sym="U"; fi
	if [ $number == 26 ]; then sym="VP"; fi
	if [ $number == 27 ]; then sym="VL"; fi
	if [ $number == 28 ]; then sym="BL"; fi
	if [ $number == 29 ]; then sym="BR"; fi
	if [ $number == 30 ]; then sym="SB"; fi
	if [ $number == 31 ]; then sym="SG"; fi
	if [ $number == 32 ]; then sym="AL"; fi
	if [ $number == 33 ]; then sym="IL"; fi
	if [ $number == 34 ]; then sym="EL"; fi
	if [ $number == 35 ]; then sym="OD"; fi
	if [ $number == 36 ]; then sym="SD"; fi
	if [ $number == 37 ]; then sym="ST"; fi
	if [ $number == 38 ]; then sym="WP"; fi
	if [ $number == 39 ]; then sym="MZ"; fi
	if [ $number == 40 ]; then sym="CM"; fi
	if [ $number == 41 ]; then sym="MU"; fi
	if [ $number == 42 ]; then sym="PM"; fi
	if [ $number == 43 ]; then sym="A"; fi
	if [ $number == 44 ]; then sym="BR"; fi

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
zip -9 $filename ./*.jpg

#clean up
rm ./*.jpg
echo done - $filename created

