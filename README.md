In this project we wrote a few functions to do the following:
writeSector,Directory,deleteFile,writeFile,copyFile,and createFile

writeSector: 
Making writeSector was basaiclaly the same thing as readSector from last project

Directory: 
The point of directory was to make a function that printed out all of the files onto the screen
We basically just looped through the directory entries + 
the buffer holding the names of the files. It skips through the '\o'
in the directory and once it reaches a name, it will print out those characters.

deleteFile:
Thie objective of this function is to delete a file given a filename parameter. First, we load the directory and map,
loop through the directory for the filename and check to see if they match,
set the first byte in the dir to 0 then setting the all the sectors in the map to 0.
Finally, write those sectors changes back to the dir and map to complete the changes.

writeFile:
This function objective is to write a block of text to a file. We did this by loading the dir and map,
looping through the directory for a place marked \0 to place a new file,
then mark a spot on the map with a 0xff which tells it start writing here onward
then putting all of the text written into said map sectors
Finally, we writeSectors the changes back to the dir and map to complete the changes.

copyFile:
For this one we went into the shell to make a copy command. We did this by using the readFile and WriteFile commands through the syscalls implemented.
firstly, we loop through the buffer to get the filename we want copied and another loop to get the filename we are copying too.
Then, we copy/read the memory into the filename, then write it into the newfile via the memeory.

createFile:
For this we created a create command in the shell to make a file. We did this by getting the filename on the line, 
printing out the name of the file to the user
then we prompt the user for the text to be put into the file which will readString(syscall 1)to the filename. 
In this we have an exit code of '\0'to stop writing to it
This will go into the memory of the file. 
 
How to verify:

./compileOS.sh
java -jar simulator.jar
select the diskc.img and boot it to disk c
run it

Different commands:

dir
(will list the directories)

create testfl
(type "hello" when promtpted, then \0 to exit writing to the file)

dir
(see that testfl showed up in the directory now to show that its creation worked)

type testfl
(will display the text you put into the file (i.e. "hello"))

copy testfl testf2
(copie contents in testfl into testf2)

dir
(displays testfl2 existing in the directory)

type testf2
(show the contents of testfl2, showing that copying works (i.e should say "hello"))

del testf2
(will delete the testf2 file)

dir
(will show that testf2 will not be in the directory any longer)

That will showcase everything!
