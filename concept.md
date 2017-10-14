*There will be a permanent file storing the date and time of the last update.*

**1)** Look for that file and continue accordingly:
The file does not exists i.e. there was no update yet: proceed with **2)**
The file exists i.e. updates were done already before: proceed with **3)**

**2)** Use the Git Trees API to retrieve all file names and download them
https://api.github.com/repos/shadowfox87/YGOSeries10CardPics/git/trees/master
**2.1)** Create the mentioned file and store the date and time

**3)** Retrieve the date/time from the file and browse in the Git Commits list
https://api.github.com/repos/shadowfox87/YGOSeries10CardPics/commits
(The parameter 'page' (?page=[1,2,3,...]) is used to do this. The pages are ordered chronologically)
**3.1)** Check each commit's date and compare it to the date in the file. Proceed until you find a date before the date of the last update (i.e. it was already part of the last update). All previous commits that were after the stored date are new commits that weren't pulled yet
**3.2)** Retrieve the Git Trees URL for all missing commits (path: 'commit/tree/url') and download the files listed in it