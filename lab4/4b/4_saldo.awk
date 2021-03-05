#!/usr/bin/awk -f

BEGIN{FS=",";}

NR==2 { 
	balance=$6;
}


NR>2 {	
	balance=balance+$4;
	if(balance ""!= $6)
	{	
		print "Saldo po operacji nr " NR " : " $6 " z opisem: \"" $5 "\", nie zgadza sie z saldem obliczonym na podstawie wczesniejszych operacji: " balance;
	}
}
