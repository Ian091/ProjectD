Group project by Tyler McKenzie and Ian Chartier

1) For step 1 (readFile) I followed the steps provided by the document.
I first load the directory sector and match all filenames found with the provided name.
Any match increases a confirm variable by 1 but the moment it is wrong it goes back down to 0 and moves on.
Once the file is found the file is read and put into a buffer array to be displayed back to the user.
This step required many loops and if else statements in order to work WITH shell.
my older code worked fine outside of shell but broke once it was used in shell.
The current code is now fully functional within shell.

Step 2 was simple as it was only having the file read and a PutInMemory called and looped during this.
After the loop was complete and all the code was read and put into segment (originally declared 0x2000), it is put into the provided call launchProgram(segment);.

Step 3 was a simple interrupt in order to function but was then changed to terminate shell for step 4.

Step 4 was very easy to implement as it was a change in terminate(); and two syscalls to show "SHELL>" during inputs.
Also provided was "Bad Command!" if the input is not type or exec.

Step 5 was a simple loop as it checked if the array line[512]'s 0-3 instances spelled out "type" and the 4th was blank.
after this it took everything 5 and beyond and put into a name variable so "type messag" you end up with only messag.
with messag taken out and put through readFile you can use type to read out files within shell.
I also as asked added "File not found" if you did type and a nonexistant file.

Step 6 was very easy as it took the same parameter to take out type but switched to "exec".
so once something such as "exec tstpr1" is provided it takes the tstpr1 and calls the executeprogram function with tstpr1.

2) to verify it run ./compileOS.sh. mine is different as i took code from earlier versions.
I did this as originally once compiled enough the diskc would corrupt/run out of space.
This way it overwrites the diskc and allowed me to make edits and overwrite diskc without risk of running out of space.
kernel is also made with the same process as tstpr1, tstpr2, and shell so loadFile can actually load it.

commands to verify:

./compileOS.sh

java -jar simulator.jar (select my diskc.img)

SHELL> hi (returns bad command!)

SHELL> type (returns type must have a file name)

SHELL> exec (returns exec needs a program name)

SHELL> type messag (returns message.txt)

SHELL> exec tstpr1 (tstpr1 works and displays but tstpr2 wont?)

Everything should be verified and working. readFile is under readSector in my kernel.c
