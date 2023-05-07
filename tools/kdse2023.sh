#!/bin/bash
# This script runs the benchmarks used in the paper on K-periodic driven DSE.

MAX_TIME=10s

KITER="./Release/bin/kiter"

SDF3_BENCH_DIR="benchmarks/sdf3mem/"
KITER_BENCH_DIR="benchmarks/IB5CSDF/"


if [ "$#" -ge 1 ]; then
    MAX_TIME=$1
fi

if [ ! -x ${KITER} ]; then
    echo -ne "Error Kiter is missing: ${KITER} Not found.\n"
    exit 1
fi



LOGDIR=./kdse2023_log/

mkdir -p ${LOGDIR}

COMMAND_PREFIX="timeout --foreground  ${MAX_TIME}"


for graph in bipartite samplerate modem satellite fig8 h263decoder; do
    echo "Run ${graph}"
    ${COMMAND_PREFIX} ${KITER} -f ${SDF3_BENCH_DIR}${graph}.xml -a throughputbufferingDSE -prealtime=1 > "${LOGDIR}/${graph}_new_KDSE.txt" 
done

for graph in BlackScholes Echo PDectect H264 JPEG2000; do
    echo "Run ${graph}"
    ${COMMAND_PREFIX} ${KITER} -f ${KITER_BENCH_DIR}${graph}.xml -a throughputbufferingDSE -prealtime=1 > "${LOGDIR}/${graph}_new_KDSE.txt" 
done


