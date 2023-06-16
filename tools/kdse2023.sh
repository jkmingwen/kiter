#!/bin/bash
# This script run benchmarks for K2DSE(A) project

DEFAULT_MAX_TIME=60s
DEFAULT_LOGDIR=./kdse2023_log/

BENCHMARKS=(
    "./benchmarks/sample.xml"
    "./benchmarks/sdf3mem/bipartite.xml"
#   "./benchmarks/sdf3mem/buffercycle.xml"
    "./benchmarks/sdf3mem/example.xml"
    "./benchmarks/sdf3mem/fig8.xml"
    "./benchmarks/sdf3mem/h263decoder.xml"
    "./benchmarks/sdf3mem/modem.xml"
    "./benchmarks/sdf3mem/samplerate.xml"
    "./benchmarks/sdf3mem/satellite.xml"
    "./benchmarks/IB5CSDF/BlackScholes.xml"
    "./benchmarks/IB5CSDF/Echo.xml"
    "./benchmarks/IB5CSDF/H264.xml"
    "./benchmarks/IB5CSDF/JPEG2000.xml"
    "./benchmarks/IB5CSDF/PDectect.xml"
)

CONFIGS=(
    "-aPrintInfos"
    "-aKPeriodicThroughputwithDSE"
    "-aDeepKPeriodicThroughputwithDSE"
    "-aDeepKPeriodicThroughputwithDSE -papprox=1"
    "-athroughputbufferingDSE -prealtime=1 -pmode=KDSE"
    "-athroughputbufferingDSE -prealtime=1 -pmode=K2DSE"
#    "-athroughputbufferingDSE -prealtime=1 -pmode=K2DSEA"
    "-athroughputbufferingDSE -prealtime=1 -pmode=KDSE -pthread=4"
    "-athroughputbufferingDSE -prealtime=1 -pmode=K2DSE -pthread=4"
#    "-athroughputbufferingDSE -prealtime=1 -pmode=K2DSEA -pthread=4"
)

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

MAX_TIME=${DEFAULT_MAX_TIME}
if [ "$#" -ge 1 ]; then
    MAX_TIME=$1
fi

LOGDIR=${DEFAULT_LOGDIR}
mkdir -p ${LOGDIR}



COMMAND_PREFIX="timeout --foreground  ${MAX_TIME}"

for i in ${!CONFIGS[@]}; do
    config_parameters=${CONFIGS[$i]}
    for graphfile in ${BENCHMARKS[@]}; do
	graph=`basename ${graphfile} .xml`
	echo "Run ${graphfile} $i (${config_parameters})"
	${COMMAND_PREFIX} ${KITER} -f ${graphfile} ${config_parameters} > "${LOGDIR}/${graph}_$i.txt"
    done
done

