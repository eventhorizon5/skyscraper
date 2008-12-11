#!/bin/bash
font="Nimbus Sans L"
number=1
maxnumber=150

while (( number <= maxnumber ))
do
	echo $number
	nice -n 12 gimp -c -d -i -b "(script-fu-makesign \"$number.jpg\" \"$number\" 64 \"$font\" '(0 0 0) '(255 255 255) 128 128)" -b '(gimp-quit 0)'
	number=$((++number))
done

number=1
maxnumber=9

while (( number <= maxnumber ))
do
	echo LL$number
	nice -n 12 gimp -c -d -i -b "(script-fu-makesign \"LL$number.jpg\" \"LL$number\" 64 \"$font\" '(0 0 0) '(255 255 255) 128 128)" -b '(gimp-quit 0)'
	number=$((++number))
done

number=1
maxnumber=9

while (( number <= maxnumber ))
do
	echo P$number
        nice -n 12 gimp -c -d -i -b "(script-fu-makesign \"P$number.jpg\" \"P$number\" 64 \"$font\" '(0 0 0) '(255 255 255) 128 128)" -b '(gimp-quit 0)'
        number=$((++number))
done

number=1
maxnumber=9

while (( number <= maxnumber ))
do
	echo B$number
        nice -n 12 gimp -c -d -i -b "(script-fu-makesign \"B$number.jpg\" \"B$number\" 64 \"$font\" '(0 0 0) '(255 255 255) 128 128)" -b '(gimp-quit 0)'
        number=$((++number))
done

number=1
end=14
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
	echo $sym
	nice -n 12 gimp -c -d -i -b "(script-fu-makesign \"$sym.jpg\" \"$sym\" 64 \"$font\" '(0 0 0) '(255 255 255) 128 128)" -b '(gimp-quit 0)'
done

