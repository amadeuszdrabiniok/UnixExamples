#!/usr/bin/awk -f

NR>1 {
	if (name[$3]=="" || name[$3] == $1 $2)
	{
        	name[$3] = $1 $2;
	}
	else
	{
		print "Blad w linijce: " NR;		
	}
}

