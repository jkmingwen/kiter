# Throughput-Buffering Trade-Off Analysis
K-Iter has several design space exploration (DSE) algorithms available to perform throughput-buffering trade-off analysis on SDFGs/CSDFGs. Here, we list the algorithms available, including the parameters that can be passed to them. The algorithms run on SDF3-like XML files as input files.

## Optimal methods
### K-periodic-driven DSE (KDSE)
Perform throughput-buffering DSE using K-periodic scheduling for throughput computation and critical cycles as a search heuristic for buffer size allocations. By default, the ```ThroughputBufferingDSE``` algorithm runs the DSE algorithm using K-periodic scheduling and critical cycles, without logging:

`./Release/bin/kiter -f <input-file> -a ThroughputBufferingDSE`

In order to log the data (search path and Pareto points), specify a log directory using the ```LOGDIR``` parameter:

`./Release/bin/kiter -f <input-file> -a ThroughputBufferingDSE -p LOGDIR=./logs/`

### ASAP scheduling driven DSE (SDF3)
[SDF3](https://www.es.ele.tue.nl/sdf3/) has a throughput-buffering DSE algorithm using ASAP scheduling for throughput computation and storage dependencies as a heuristic for determining buffer size allocations. SDF3 can be installed within K-Iter. We added logging capabilities to SDF3, amongst other changes, that will be listed in the following sections. Note that `patch` is required to run `install_sdf3.sh`, so install that with your package manager of choice before running the script. In order to install and compile SDF3, run the following from the home directory of K-Iter:

1. If the subdirectory doesn't already exist: `mkdir ./tools/sdf3`
2. `./tools/install_sdf3.sh ./tools/sdf3/`

Now that SDF3 has been installed, their throughput-buffering DSE algorithm can be run using the following command:

`SDF3LOGDIR=./logs/ USE_SCC=false COARSE=false ./tools/sdf3/sdf3_custom/sdf3/build/release/Linux/bin/{sdf3analysis-sdf|sdf3analysis-csdf} --graph <input-file> --algo buffersize`

Note that `SDF3LOGDIR` sets the log directory path, so set it accordingly and make sure that the log directory is made before running the DSE. Furthermore, either `sdf3analysis-sdf` or `sdf3analysis-csdf` will have to be used depending on whether the input file is a SDF or CSDF graph.

### Corrected storage dependency detection using strongly connected components
We identified an implementation error in SDF3's simple cycle detection algorithm which, in turn, caused a bug in their storage dependency detection algorithm. We corrected this by using strongly connected components to detect cycles in their dependency graph. This corrected version can be run by setting ```USE_SCC=true```.

`SDF3LOGDIR=./logs/ USE_SCC=true COARSE=false ./tools/sdf3/sdf3_custom/sdf3/build/release/Linux/bin/{sdf3analysis-sdf|sdf3analysis-csdf} --graph <input-file> --algo buffersize`

## Approximate methods
### 1-periodic DSE (1DSE)
Perform throughput-buffering DSE using 1-periodic scheduling and dichotomous search for buffer size allocations:

`./Release/bin/kiter -f <input-file> -a PeriodicDSE -p LOGDIR=./logs/`

### Coarse K-periodic-driven DSE (KDSE-C)
KDSE-C multiplies the step size of the KDSE algorithm by a 2 to increase the coarseness of the DSE:

`./Release/bin/kiter -f <input-file> -a ThroughputBufferingDSE -p LOGDIR=./logs/ -p COARSE=2`

### Coarse K-periodic-driven DSE (SDF3-C)
SDF3-C multiplies the step size of the SDF3 algorithm by a 2 to increase the coarseness of the DSE:

`SDF3LOGDIR=./logs/ USE_SCC=false COARSE=true ./tools/sdf3/sdf3_custom/sdf3/build/release/Linux/bin/{sdf3analysis-sdf|sdf3analysis-csdf} --graph <input-file> --algo buffersize`
