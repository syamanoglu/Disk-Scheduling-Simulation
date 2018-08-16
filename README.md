# Disk-Scheduling-Simulation
Disk Scheduling algorithms are simulated and corresponding statistics are printed


**Objectives:** Practice developing a simulator and doing simulation experiments;
practice disk scheduling.


**Following algorithms are simulated:**

* FCFS serves the requests in first-come first-served manner (according to their order in scheduling
queue).

* SSTF serves the shortest distance request first (the closest request to the current head
position)

* LOOK algorithm scans the disk from one end to the other while serving requests; we do not
go till the end of the disk, but to the last request in that direction. Then, direction is reversed and
requests are continued to be served in the new direction.

* CLOOK is a circular version of LOOK and
serves the requests only in one direction; for example from left to right. When we come to end (to
the right-most), we reverse the direction and go the beginning (to the left-most request) without
serving (but still moving the disk and spending time). Then we change the direction again towards
right and serve the requests again while going from left to right.


An example output (not related to the values above) is shown below (just to
give an example for the format):

    FCFS : 2360 30 60
    SSTF : 2146 20 52
    LOOK : 1690 32 50
    CLOOK: 1912 35 40
    
The output says, for example, that for FCFS scheduling, the total time to serve all the
requests is 2360 time units, the average wait time is 30 and the standard deviation
of the wait time is 60. Note that the values in the example above are just given
randomly (may not be used to compare the algorithms, i.e., may not make sense
when compared).
    
    
The program executable will be called ds and an example invocation of it can
be as below:

    ./ds <N> <infile.txt>
* \<N> is the number of cylinders on the disk (1..N). 
* <infile.txt> is the input file.
