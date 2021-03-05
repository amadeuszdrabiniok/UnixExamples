echo "Podaj URL:"
read URLarg
echo "Czy ustawic flage q? (y/n)"
read qarg
echo "Czy ustawic flage r? (y/n)"
read rarg
echo "Podaj argument l, lub zostaw puste"
read larg
echo "Podaj lokalizacje w ktorej ma zostac zapisana strona, lub zostaw puste"
read Parg

case "$Parg" in
        "") Parg=. ;;
esac


case "$qarg" in
	"y") qarg=1 ;;
	"n") qarg=0 ;;
esac

case "$rarg" in
        "y") rarg=1 ;;
        "n") rarg=0 ;;
esac


if [ "$rarg" -eq 1 ]
then
	if [ "$qarg" -eq 1 ]
	then
		wget -q -r -l $larg -P $Parg $URLarg
	else
		wget -r -l $larg -P $Parg $URLarg
	fi
else
	if [ "$qarg" -eq 1 ]
        then
                wget -q -l $larg -P $Parg $URLarg
	else
                wget -l $larg -P $Parg $URLarg
        fi
fi
