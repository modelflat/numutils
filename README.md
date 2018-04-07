# Num[erical]Utils 
[![Build Status](https://travis-ci.org/modelflat/numutils.svg?branch=master)](https://travis-ci.org/modelflat/numutils)
[![Project Status: WIP – Initial development is in progress, but there has not yet been a stable, usable release suitable for the public.](http://www.repostatus.org/badges/latest/wip.svg)](http://www.repostatus.org/#wip)
[![Coverage Status](https://coveralls.io/repos/github/modelflat/numutils/badge.svg?branch=master)](https://coveralls.io/github/modelflat/numutils?branch=master) _<sup>(coverage is broken for now, probably because we are header only, searching for solution)</sup>_

Yet another home-grown numerical utils for C++

This small library provides you with convenient functional interfaces to write numerical-related code (integrals, derivatives and so).
It primarily consists of heavily templated code and is (currently) not so customizable in run-time.

Check examples/ directory for usage examples.

## Build

This is header-only library, so feel free to ```git submodule add ...``` or copy-paste ```include/*``` and ```matplotlibcpp/matplotlibcpp.h```
files into your project (nasty!)

Make sure you have Python 3 + NumPy installed if you want plots. [GTest](https://github.com/google/googletest) is required for tests.

To build tests and examples, run

```mkdir build && cd build && cmake -G "<your preferred generator name>" .. && make```

This should (theoretically) work nearly anywhere. 

