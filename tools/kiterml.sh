#!/bin/bash
# This script runs the benchmarks used in the paper on K-periodic driven DSE.

MAX_TIME=20m

KITER="./Release/bin/kiter"
KITER_PARAMS1="-aAddFeedbackBuffers -aLivenessDSE -prealtime=1"
KITER_PARAMS2="-aAddFeedbackBuffers -aLivenessDSE -prealtime=1 -puse_dichotomy=1"
KITER_PARAMS3="-aAddFeedbackBuffers -aLivenessDSE -prealtime=1 -puse_dichotomy=1 -puse_last=1"

SDF3_BENCH_DIR="benchmarks/sdf3mem/"
KITER_BENCH_DIR="benchmarks/IB5CSDF/"


if [ "$#" -ge 1 ]; then
    MAX_TIME=$1
fi

if [ ! -x ${KITER} ]; then
    echo -ne "Error Kiter is missing: ${KITER} Not found.\n"
    exit 1
fi



LOGDIR=./kiterml_log/

mkdir -p ${LOGDIR}

COMMAND_PREFIX="timeout --foreground  ${MAX_TIME}"


for graph in bipartite samplerate modem satellite fig8 h263decoder; do
    echo "Run ${graph} with ${MAX_TIME}"
    ${COMMAND_PREFIX} ${KITER} -f ${SDF3_BENCH_DIR}${graph}.xml ${KITER_PARAMS1} > "${LOGDIR}/${graph}_1.txt" 
    ${COMMAND_PREFIX} ${KITER} -f ${SDF3_BENCH_DIR}${graph}.xml ${KITER_PARAMS2} > "${LOGDIR}/${graph}_2.txt" 
    ${COMMAND_PREFIX} ${KITER} -f ${SDF3_BENCH_DIR}${graph}.xml ${KITER_PARAMS3} > "${LOGDIR}/${graph}_3.txt" 
done

for graph in BlackScholes Echo PDectect H264 JPEG2000; do
    echo "Run ${graph}"
    ${COMMAND_PREFIX} ${KITER} -f ${KITER_BENCH_DIR}${graph}.xml  ${KITER_PARAMS1} > "${LOGDIR}/${graph}_1.txt" 
    ${COMMAND_PREFIX} ${KITER} -f ${KITER_BENCH_DIR}${graph}.xml  ${KITER_PARAMS2} > "${LOGDIR}/${graph}_2.txt" 
    ${COMMAND_PREFIX} ${KITER} -f ${KITER_BENCH_DIR}${graph}.xml  ${KITER_PARAMS3} > "${LOGDIR}/${graph}_3.txt" 
done


