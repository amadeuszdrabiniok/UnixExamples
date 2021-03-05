#!/usr/bin/awk -f

NR>1 {
	sum[$3]=sum[$3] + $4;
	name[$3]=$1 " " $2;	
}

END {
	for (x in sum)
	{
		print "|Imie i Nazwisko: "name[x]"|--|nr:  "x"|--|Suma punktow: "sum[x]"|";	
	}
}
