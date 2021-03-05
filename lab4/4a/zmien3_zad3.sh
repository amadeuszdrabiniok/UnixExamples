if [ "$#" -eq 0 ]
then
	find . -name "*" -type f | while read filename
	do
		newfilename=`basename "$filename"`
		oldfilename=$newfilename
		result=`printf "$newfilename" | tr -c '[:alnum:]' '[_*]' | tr [a-z] [A-Z]`
		echo $result $oldfilename
	done
else
	find $1 -name "*" -type f | while read filename
        do
		newfilename=`basename "$filename"`
		oldfilename=$newfilename
		result=`printf "$newfilename" | tr -c '[:alnum:]' '[_*]' | tr [a-z] [A-Z]`
		echo $result $oldfilename
	done
fi
	
