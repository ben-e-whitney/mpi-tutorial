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
