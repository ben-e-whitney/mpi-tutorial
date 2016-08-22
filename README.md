Introduction
============
Today we will be learning to use MPI (Message Passing Interface). There are lots of MPI guides available online, so this repository only contains some exercises to help introduce you to the basics. Feel free to work together, and ask if you have any questions!

Exercises
=========

hello
-----
We begin with a simple 'hello world' program. Edit the file `src/hello/main.c`, and then compile it with the command `make hello`. When you run it (`mpirun -n $N bin/hello/main`), each process should print out its rank. You can look at `src/hello/solution.c` if you need help.

Helpful links: [`MPI_Init`][MPI_Init], [`MPI_Finalize`][MPI_Finalize], [`MPI_Comm_rank`][MPI_Comm_rank], [`MPI_Comm_size`][MPI_Comm_size], [`MPI_COMM_WORLD`][MPI_COMM_WORLD]

[MPI_Init]: 
[MPI_Finalize]: 
[MPI_Comm_size]: 
[MPI_Comm_rank]: 
[MPI_COMM_WORLD]: 

telephone
---------
Telephone is a game for children (or processors). All of the children sit in a circle. The game starts when one child whispers a message to the child sitting to their right. That child then whispers the message to the child to their right, and so on until the message reaches the child who first whispered it. It's kind of hard to understand whispers, so the message the first child hears might not be the same as the one they whispered!

Edit the file `src/telephone/main.c` so that each processor passes the message to the processor with rank one greater. (Who should the processor with rank `size - 1` communicate with?) The message is held in the variable `whispers`, which is an array of length `argc - 1` of arrays of length `MAX_WORD_LENGTH` of `char`s. Compile with `make telephone` and run with `mpirun -n $N bin/telephone/main $message`. You can look at `src/telephone/solution.c` if you need help.

Helpful links: [`MPI_Send`][MPI_Send], [`MPI_Recv`][MPI_Recv], [`MPI_Status`][MPI_Status]
[MPI_Send]: 
[MPI_Recv]: 
[MPI_Status]: 

homework
--------
You are a student in an arithmetic class, and this week your teacher is teaching you multiplication. Your homework is to fill out a times table. Unfortunately, you and the other students are *very lazy*, and you've decided to cheat. Do this in three ways:
	* Have each student fill out one row (or column) of the times table and send that row (column) to every other student. By the time you're done, everyone should have a full times table.
	* Have each student fill out one row of the times table. Instead of having the students individually send rows to each other, use `MPI_Gather` and `MPI_Bcast` to share the products.
	* Have each student fill out one row of the times table, and then use `MPI_Allgather` to exchange all of the information all at once!

Edit the file `src/homework/main.c` so that every processor ends up with the full times table, which is held in the variable `times_table`, an array of length `size` of arrays of length `size` of `char`s. Compile with `make homework` and run with `mpirun -n $N bin/homework/main`. You can look at `src/homework/solution.c` if you need help.

Helpful links: [`MPI_Gather`][MPI_Gather], [`MPI_Bcast`][MPI_Bcast], [`MPI_Allgather`][MPI_Allgather]
[MPI_Gather]: 
[MPI_Bcast]: 
[MPI_Allgather]: 

Acknowledgments
===============
`data/telephone/arpabet_common.txt` is a subset of the [CMU Pronouncing Dictionary version 0.7b][cmudict]. Its entries are restricted to the 2^14 most common English words according to [Peter Norvig][norvig]. Norvig's list is derived from [a corpus][corpus] distributed by the [Linguistic Data Consortium][LDC] and available under the [Web 1T 5-gram Version 1 Agreement][agreement]. Note that only one pronunciation per word has been retained. The commands used to generate this file can be viewed by executing
    make --always-make --dry-run regenerate-dictionary

[cmudict]: http://www.speech.cs.cmu.edu/cgi-bin/cmudict
[norvig]: http://norvig.com/ngrams/count_1w.txt
[corpus]: https://catalog.ldc.upenn.edu/LDC2006T13
[LDC]: https://www.ldc.upenn.edu/
[agreement]: https://catalog.ldc.upenn.edu/license/web-1t-5-gram-version-1.pdf

`data/prom/countries.csv` is taken from [the Google Developers Dataset Public Language site][google], where it is made available under the [Creative Commons Attribution 3.0 License][cc].

[google]: https://developers.google.com/public-data/docs/canonical/countries_csv
[cc]: https://creativecommons.org/licenses/by/3.0/
