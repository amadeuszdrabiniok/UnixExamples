archstring="_arch"

if [ ! -d "$HOME/.arch/" ]
then
        mkdir $HOME/.arch/
fi

ls *$1 | while read file
do
	    newfilename=`basename "$file" $1`
        newfile="$newfilename$archstring$1"
        cp -p "$newfilename$1" $HOME/.arch/"$newfile"
done
