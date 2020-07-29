#rsync --progress -av --bwlimit=0  -e ssh cagragr@ftp-1:/np1a/cagra/gr/2016campaign/cagra/E455/run_10*   /run/media/kobayash/CAGRAGR3/cagra/E455
#rsync --progress -av --bwlimit=0  -e ssh cagragr@ftp-1:/np1a/cagra/gr/2016campaign/cagra/E455/run_20*   /run/media/kobayash/CAGRAGR4/cagra/E455
#rsync --progress -av --bwlimit=0  -e ssh cagragr@ftp-1:/np1a/cagra/gr/2016campaign/cagra/E455/run_11*   /run/media/kobayash/CAGRAGR4/cagra/E455
#rsync --progress -av --bwlimit=0  -e ssh cagragr@ftp-1:/np1a/cagra/gr/2016campaign/cagra/E455/run_40*   /run/media/kobayash/CAGRAGR4/cagra/E455
rsync --progress -av --bwlimit=0 --exclude='run_10*' --exclude='Merged' -e ssh cagragr@ftp-1:/np1a/cagra/gr/2016campaign/cagra/E455/ /run/media/kobayash/CAGRAGR4/cagra/E455
