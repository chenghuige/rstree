perl -ane '{print "$F[0]\t1\n";}' black.txt >  black.dm.txt 
perl -ane '{print "$F[0]\t1\n";}' white.fordm.txt >  white.dm.txt 
saveDmDict black.dm.txt black.dm
saveDmDict white.dm.txt white.dm
