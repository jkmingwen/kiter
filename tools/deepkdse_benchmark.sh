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



LOGDIR=./deep_log/

mkdir -p ${LOGDIR}

COMMAND_PREFIX="timeout --foreground  ${MAX_TIME}"


for graph in bipartite samplerate modem satellite fig8 h263decoder; do
    echo "Run ${graph}"
    ${COMMAND_PREFIX} ${KITER} -f ${SDF3_BENCH_DIR}${graph}.xml -a KPeriodicThroughputwithDSE -poutput="${LOGDIR}/${graph}_KDSE_pareto.txt"  > "${LOGDIR}/${graph}_KDSE.txt" & # 2> /dev/null
    ${COMMAND_PREFIX} ${KITER} -f ${SDF3_BENCH_DIR}${graph}.xml -a DeepKPeriodicThroughputwithDSE -poutput="${LOGDIR}/${graph}_DKDSE_pareto.txt"  > "${LOGDIR}/${graph}_DKDSE.txt" &  # 2> /dev/null
    APPROX_DSE=1 ${COMMAND_PREFIX} ${KITER} -f ${SDF3_BENCH_DIR}${graph}.xml -a DeepKPeriodicThroughputwithDSE -poutput="${LOGDIR}/${graph}_ADKDSE_pareto.txt"  > "${LOGDIR}/${graph}_ADKDSE.txt" &  # 2> /dev/null
    ${COMMAND_PREFIX} ${KITER} -f ${SDF3_BENCH_DIR}${graph}.xml -a SPeriodicDSE -poutput="${LOGDIR}/${graph}_PDSE_pareto.txt"  > "${LOGDIR}/${graph}_PDSE.txt" &  # 2> /dev/null
done

for graph in BlackScholes Echo PDectect H264 JPEG2000; do
    echo "Run ${graph}"
    ${COMMAND_PREFIX} ${KITER} -f ${KITER_BENCH_DIR}${graph}.xml -a KPeriodicThroughputwithDSE -poutput="${LOGDIR}/${graph}_KDSE_pareto.txt"  > "${LOGDIR}/${graph}_KDSE.txt" & # 2> /dev/null
    ${COMMAND_PREFIX} ${KITER} -f ${KITER_BENCH_DIR}${graph}.xml -a DeepKPeriodicThroughputwithDSE -poutput="${LOGDIR}/${graph}_DKDSE_pareto.txt"  > "${LOGDIR}/${graph}_DKDSE.txt" &  # 2> /dev/null
    APPROX_DSE=1 ${COMMAND_PREFIX} ${KITER} -f ${KITER_BENCH_DIR}${graph}.xml -a DeepKPeriodicThroughputwithDSE -poutput="${LOGDIR}/${graph}_ADKDSE_pareto.txt"  > "${LOGDIR}/${graph}_ADKDSE.txt" &  # 2> /dev/null
    ${COMMAND_PREFIX} ${KITER} -f ${KITER_BENCH_DIR}${graph}.xml -a SPeriodicDSE -poutput="${LOGDIR}/${graph}_PDSE_pareto.txt"  > "${LOGDIR}/${graph}_PDSE.txt" &  # 2> /dev/null
done


