# ygopro2-updater

*There will be a permanent file storing the date and time of the last update.*  

>**`1.`** *Look for a new release of the updater and download it.*  

>**`2.`** *Look for that file and continue accordingly.*  
If the file does not exist (there was no update yet) - go to **`A`**  
If the file exists (updates were done already before) - go to **`B`**  

>**`A|1.`** *Use the Git Trees API to retrieve all file names and download them.*  
https://api.github.com/repos/shadowfox87/YGOSeries10CardPics/git/trees/master  
**`A|2.`** *Create the mentioned file and store the date and time.*  

>**`B|1.`** *Retrieve the date/time from the file and browse in the Git Commits list.*  
https://api.github.com/repos/shadowfox87/YGOSeries10CardPics/commits  
Use the `page` parameter (`?page=[1,2,3,...]`) for this. They are ordered chronologically.  
**`B|2.`** *Check each commit's date and compare it to the date in the file. Proceed until you find a date before the date of the last update (i.e. it was already part of the last update). All previous commits that were after the stored date are new commits that weren't pulled yet.*  
**`B|3.`** *Retrieve the Git Trees URL for all missing commits (JSON path: `commit/tree/url`) and download the files listed in it.*  
