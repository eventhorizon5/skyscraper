#!/bin/bash
font="Nimbus Sans L"
fontsize=38
#font="Nimbus Sans L Bold"
#fontsize=36
#font="Nimbus Sans L Condensed"
#fontsize=46
#font="Nimbus Sans L Bold Condensed"
#fontsize=44

number=1
end=5
while (( number <= end ))
do
	if [ $number == 1 ]; then sym="Alarm"; file="Alarm"; fi
	if [ $number == 2 ]; then sym="Close"; file="Close"; fi
	if [ $number == 3 ]; then sym="Open"; file="Open"; fi
	if [ $number == 4 ]; then sym="Stop"; file="Stop"; fi
	if [ $number == 5 ]; then sym="  Call\nCancel"; file="CallCancel"; fontsize=$((fontsize - 6)); fi
	echo $file
	nice -n 12 gimp -c -d -i -b "(script-fu-makesign \"$file.jpg\" \"$sym\" $fontsize \"$font\" '(0 0 0) '(255 255 255) 128 128)" -b '(gimp-quit 0)'
	number=$((++number))
done

