archstring="_arch"

if [ ! -d "$HOME/.arch/" ]
then
	    mkdir $HOME/.arch/
fi

ls | sed -n /"$1"$/p | sed s/"$1"$// | while read file
do
	newfile="$file$archstring$1"
	cp -p "$file$1" $HOME/.arch/"$newfile"
done
