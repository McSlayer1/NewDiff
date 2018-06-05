# NewDiff
To make the diff command better! I wanted to improve the diff command when comparing files in a *NIX system, 
and also improve my C skills at the same time.

This program operates very similiary to the original diff command in Linux, UNIX, etc.
First compile the program with gcc:
gcc main.c -o \<runable file name\>

Then run the file with two files:
./\<runable file name\> \<old text file\> \<new text file\>

The program will begin to compare both files. The output will then display the file names with their last modified date.
The the first set of data to be shown will be the edits/changes made between the two files, like line A in the old file
was modified to line B in the new file. The the next set of data will be the items removed from the old file. Finally,
the last set of data will be the additions added to the new file.

The output should appear as follows:

Last modified time for <old text file>: Day Month Date HH:MM:SS YYYY

Last modified time for <new text file>: Day Month Date HH:MM:SS YYYY

Changed from \<old text file\> ---> \<new text file\>

\--------------------------

... ---> ...

... ---> ...

... ---> ...

...

Removed from \<old text file\>

\--------------------------

...

...

...

...

Added from \<new text file\>

\--------------------------

...

...

...

...
</br>
**Before running the program, make a directory called 'diff_logs' or the program will seg fault!**