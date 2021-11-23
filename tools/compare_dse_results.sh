#!/bin/bash

#set -x

# Script to compare the DSE results from different DSE algorithms (Critical cycle vs Storage Dependency driven DSEs)
# Call script and list benchmark names to compare DSE results (pareto point logs):
# e.g. ./compare_dse_results.sh bipartite samplerate
# NOTE currently hardcoded to compare kiter and sdf3 pareto logs but can be easily be adapted to different logs

KITER_ROOT_DIR="./"
LOG_DIR=${KITER_ROOT_DIR}/logs/ # TODO don't hardcode DSE logs subdirectory ('logs')
RESULT_LOG=${LOG_DIR}/pp_logs/

# make directories to store logs for verification
# mkdir -p ${LOG_DIR}

echo "Arguments: $*"
BENCHMARK_ARRAY=( "$@" )

# compare logs
TOTAL_TESTS=0
PASSED=0

# check for differences in logs for each benchmark specified
for BENCHMARK in "${BENCHMARK_ARRAY[@]}"; do
    GRAPH=${BENCHMARK##*/}
    GRAPH_NAME=${GRAPH%.xml}
    SDF3_RES="${RESULT_LOG}/${GRAPH_NAME}_pp_sdf3.csv"
    KITER_RES="${RESULT_LOG}/${GRAPH_NAME}_pp_kiter.csv"

    if [ -f "${SDF3_RES}" ] && [ -f "${KITER_RES}" ]; then
        TOTAL_TESTS=$((TOTAL_TESTS + 1))
        printf "Checking %s" "${GRAPH_NAME}..."
        if diff -q <(sort "$KITER_RES") <(sort "$SDF3_RES") > /dev/null; then # sort results to avoid mismatches due to ordering
            PASSED=$((PASSED + 1))
            echo "results match!"
        else
            echo "failed! Run 'diff -y <(sort $KITER_RES) <(sort $SDF3_RES)' to see differences"
            # diff -y <(sort $KITER_RES) <(sort $SDF3_RES)
        fi
    else
	echo "***** One of them failed."
    fi
done

if [ $PASSED -eq $TOTAL_TESTS ]; then
    echo "All tests passed; DSEs produce identical results!"
else
    echo "$PASSED/$TOTAL_TESTS matching DSE results"
fi
