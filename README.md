# Shell_in_C
Personal Shell built using C

# Description
### Warning: You need to click enter and then type any command you want for it to work.   
<br> <br>

## 1) Display
- displays system + username 
- display changes according to which directory you are in
- directory of code acts as the main dir

## 2) Input Requirements
- ; and & can be used to seperate commands if needed

## 3) warp
- works exactly like cd
- “.”, “..”, “~”, and “-” flags work

## 4) peek
- gives file information
- -l, -a, -la, -al, -a -l, -l -a flags are supported
  
## 5) pastevents
- works exactly like history
- past 20 history is stored in a .txt file also

## 6) proclore
- gives information about any particular process
- PID, Process Status, Process Group ID, Virtual Memory, Executable Path is shown

## 7)  I/O Redirection
- ">" and ">>" followed by the name of a .txt (example.txt) is supported
- This works with echo, peek, proclore, etc

## 8) Signals
- Ctrl-C, Ctrl-Z, Ctrl-D implemented

## 9)Neonate
- prints the Process-ID of the most recently created process on the system

## 10) Networking
- iMan command implemented


# Assumptions
To run file use, gcc main.c and ./a.out  <br> <br>
make file is giving errors for now
