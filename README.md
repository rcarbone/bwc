Hash Table Performance Tests by Jeff Preshing
=============================================
  http://preshing.com/20110603/hash-table-performance-tests

Word Counting Benchmark porting to Linux R. Carbone Jul 2016
============================================================

  o Added top-level Makefile

      make help for help

  o Added top-level README.md

  o rm -rf python-2.7.1

  o rm -rf judy-1.0.5

  o rm -rf sparsehash-1.10

  o rm -rf gcc-4.6.0

  o installed python

    sudo apt-get install python-dev

  o installed judy

    sudo apt-get install libjudy-dev

  o installed Google sparsehash

    sudo apt-get install libsparsehash-dev

  o added in gcc-latest source files with gcc hashtab implementation


 Benckmark results reported in the web page

 ==========================================

 StringHashTable      171 ms

 GCC Hash Table       239 ms

 Chained Hash Table   247 ms

 OpenHash Table       279 ms

 Google Dense Hash    309 ms

 JudyL array          342 ms

 stdext::hash_map     357 ms

 JudySL Array         359 ms

 Google Sparse Hash   501 ms

 Python Dictionary    661 ms

 std::map             956 ms


 Benckmark results on my system

 ==============================

rocco> make benchmark

Sorted Average of 5 runs:

Linux nuc 3.2.0-4-amd64 #1 SMP Debian 3.2.81-1 x86_64 GNU/Linux

name                 avg(ms)      stddev(ms)

==================== =======    =============

ChainedHashTable     106.980    1838793580.33

Google-dense-hash    115.183    1709242558.01

OpenHashTable        118.607    7923195852.90

StringHashTable      125.918    5578868875.23

Google-sparse-hash   171.690     545720933.92

JudyL-hash-table     206.481    2006373458.69

JudySL-Array         215.245    2234865362.72

GCC-hash-table       215.395    6769747387.21

Python-dictionary    389.968   12591410102.29

std::map-hash-table  446.073    6808571383.90

std::map             608.187    9840020753.08

ChainedHashTable      96.854     638447503.30

StringHashTable      102.112    1697072539.97

Google-dense-hash    109.193     682166969.62

OpenHashTable        110.431     442027297.48

Google-sparse-hash   162.567     708914801.97

GCC-hash-table       185.272   12696055070.03

JudyL-hash-table     201.441    6700670768.71

JudySL-Array         210.483    2774119939.13

Python-dictionary    368.460   20340048397.58

std::map-hash-table  411.945    4647452852.76

std::map             593.534    3101650202.85

# bwc
