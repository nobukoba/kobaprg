for str in ../data/run*
do
    str2=${str#../data/run}
    if [ ${str2%.dat} -ne 3012 ] ; then
	echo ../kobamac/offline/offline $str run${str2%.dat}.root
	../kobamac/offline/offline $str run${str2%.dat}.root
    fi
done
