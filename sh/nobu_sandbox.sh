#!/bin/bash

#rsync --progress -ahvn --include="run_2335*" --include="run_2336*" --include="run_2337*" --exclude="*" miho-1:/np1a/cagra/gr/2016campaign/cagra/E450/ /work/np1a/cagra/gr/2016campaign/cagra/E450/

rsync --progress -ahv --include="run_2335*" --include="run_2336*" --include="run_2337*" --exclude="*" miho-1:/np1a/cagra/gr/2016campaign/cagra/E450/ /work/np1a/cagra/gr/2016campaign/cagra/E450/
