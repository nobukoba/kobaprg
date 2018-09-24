for str in ./data/run*
do
    str2=${str#./data/run}
    if [ ${str2%.dat} -ne 3012 ] ; then
	echo ./kobaprg/offline/offline $str root_files/run${str2%.dat}.root
	#./kobaprg/offline/offline $str run${str2%.dat}.root
    fi
done
