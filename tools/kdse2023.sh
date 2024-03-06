#!/bin/bash
# This script run benchmarks for K2DSE(A) project

KITER_MAX_TIME=3600
TIMEOUT_MAX_TIME=3800
DEFAULT_LOGDIR=./kdse2023_log/

BENCHMARKS=(
    "./benchmarks/sample.xml"
    "./benchmarks/sdf3mem/bipartite.xml"
    "./benchmarks/sdf3mem/buffercycle.xml"
    "./benchmarks/sdf3mem/example.xml"
    "./benchmarks/sdf3mem/fig8.xml"
    "./benchmarks/sdf3mem/modem.xml"
    "./benchmarks/sdf3mem/samplerate.xml"
    "./benchmarks/sdf3mem/satellite.xml"
    "./benchmarks/IB5CSDF/BlackScholes.xml"
    "./benchmarks/IB5CSDF/Echo.xml"
    "./benchmarks/IB5CSDF/PDectect.xml"
    "./benchmarks/sdf3mem/h263decoder.xml"
    "./benchmarks/IB5CSDF/H264.xml"
    "./benchmarks/IB5CSDF/JPEG2000.xml"
)

declare -A CONFIGS  # Declare associative array
CONFIGS["infos"]="-aPrintInfos"
declare -A CONFIGS  # Declare associative array

# Populate the array
#CONFIGS["infos"]="-aPrintInfos"
#CONFIGS["throughput"]=" -aKPeriodicThroughput -pDETAILS=1"
#CONFIGS["kdse"]=" -athroughputbufferingDSE -prealtime=1 -pmode=KDSE"
#CONFIGS["k2dse"]=" -athroughputbufferingDSE -prealtime=1 -pmode=K2DSE"
#CONFIGS["k2dsea"]=" -athroughputbufferingDSE -prealtime=1 -pmode=K2DSEA"
#CONFIGS["k2dsec"]=" -athroughputbufferingDSE -prealtime=1 -pmode=K2DSE -pusecache=1"
#CONFIGS["k2dseac"]=" -athroughputbufferingDSE -prealtime=1 -pmode=K2DSEA -pusecache=1"
CONFIGS["k2dsec24"]=" -athroughputbufferingDSE -prealtime=1 -pmode=K2DSE -pthread=24 -pusecache=1 -ptimeout=$KITER_MAX_TIME"
CONFIGS["k2dseac24"]=" -athroughputbufferingDSE -prealtime=1 -pmode=K2DSEA -pthread=24 -pusecache=1 -ptimeout=$KITER_MAX_TIME"



KITER="./Release/bin/kiter"

if [ ! -x ${KITER} ]; then
    echo -ne "Error Kiter is missing: ${KITER} Not found.\n"
    exit 1
fi


for f in ${BENCHMARKS[@]}; do
    if [ ! -e ${f} ]; then
	echo -ne "Error ${f} is missing.\n"
	exit 1
    fi
done

MAX_TIME=${TIMEOUT_MAX_TIME}
if [ "$#" -ge 1 ]; then
    MAX_TIME=$1
fi

LOGDIR=${DEFAULT_LOGDIR}
mkdir -p ${LOGDIR}



COMMAND_PREFIX="timeout --foreground  ${TIMEOUT_MAX_TIME}"

for graphfile in ${BENCHMARKS[@]}; do
    for i in "${!CONFIGS[@]}"; do
	config_parameters=${CONFIGS[$i]}
	graph=`basename ${graphfile} .xml`
	echo "${COMMAND_PREFIX} ${KITER} -f ${graphfile} ${config_parameters} > \"${LOGDIR}/${graph}_$i.txt\""
	${COMMAND_PREFIX} ${KITER} -f ${graphfile} ${config_parameters} > "${LOGDIR}/${graph}_$i.txt"
    done
done

