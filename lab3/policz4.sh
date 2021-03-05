flag=0
licznik2=1
licznik=0
set -- $(getopt a $*)
while [ "$licznik2" -le "$#" ]
do
	case ${!licznik2} in
		-a) flag=1; shift;;
		*) ;;
	esac
	licznik2=$((licznik2+1))
done

if [ "$#" -gt "$licznik" ]
then
	if [ "$flag" -eq "1" ]
	then
		if [ "$#" -gt 1 ]
		then
			while [ "$licznik" -lt "$#" ]
			do
				licznik=$((licznik+1))
				liczba=`ls -1 -a ${!licznik} | wc -l`
				liczba=`expr $liczba - 2`
				echo $liczba
			done
		else
			liczba=`ls -1 -a | wc -l`
			liczba=`expr $liczba - 2`
			echo $liczba
		fi
	else
		while [ "$licznik" -lt "$#" ] 
                        do
				licznik=$((licznik+1))
                        	liczba=`ls -1 ${!licznik} | wc -l`
                                echo $liczba
                        done
	fi	
else
	ls -1 | wc -l
fi
