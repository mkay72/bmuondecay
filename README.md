# bmuondecay
## Overview
`bmuondecay` is a small program written in C which is purposed for calculation of the total transition rate of the bound-muon to free electron decay process within the relativistic framework.

## Dependencies
The program mainly depends on the following external libraries:
* All linear algebra operations are performed employing BLAS library. It is assumed that the user have their own BLAS installed.
* The radial muon and electron wave functions are calculated with the package RADIAL [F. Salvat and J. M. Fernández-Varea, Radial: A Fortran Subroutine Package for the Solution of the Radial Schrödinger and Dirac Wave Equations, Computer Physics Communications 240, 165 (2019).]
* The spherical Bessel functions are computed using [D. E. Amos, Algorithm 644: A Portable Package for Bessel Functions of a Complex Argument and Nonnegative Order, ACM Trans. Math. Softw. 12, 265 (1986).]

## Installation and run
The program is supported on Linux. To compile a program, call `make libs all`. The examples of input and ouptut files are located in `example` directory. To run a program, use `./bin/bmudecay ./example/example_z83.inp`. For documentation, advice manual in `doc/bmuondecay.pdf`.
