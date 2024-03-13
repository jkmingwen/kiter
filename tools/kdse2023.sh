#!/bin/bash

# This script runs benchmarks for K2DSE(A) project
set -e

# Default settings
DEFAULT_TIMEOUT=10
EXTRA_TIMEOUT=20
DEFAULT_LOGDIR="./kdse2023_log/"
KITER="./Release/bin/kiter"
DEFAULT_CPU_COUNT=24
DRY_RUN=0

# Define the array of benchmarks
declare -a FAST_BENCHMARKS=(
    "./benchmarks/sample.xml"
    "./benchmarks/sdf3mem/bipartite.xml"
    "./benchmarks/sdf3mem/buffercycle.xml"
    "./benchmarks/sdf3mem/example.xml"
    "./benchmarks/sdf3mem/fig8.xml"
    "./benchmarks/sdf3mem/modem.xml"
    "./benchmarks/sdf3mem/samplerate.xml"
    "./benchmarks/sdf3mem/satellite.xml"
    "./benchmarks/IB5CSDF/BlackScholes.xml"
)

declare -a SLOW_BENCHMARKS=(
    "./benchmarks/IB5CSDF/Echo.xml"
    "./benchmarks/IB5CSDF/PDectect.xml"
    "./benchmarks/sdf3mem/h263decoder.xml"
    "./benchmarks/IB5CSDF/H264.xml"
    "./benchmarks/IB5CSDF/JPEG2000.xml"
)

BENCHMARKS=("${FAST_BENCHMARKS[@]}" "${SLOW_BENCHMARKS[@]}") # Combine arrays

# Define configurations
declare -A CONFIGS

CONFIGS["infos"]="-aPrintInfos"
CONFIGS["throughput"]=" -aKPeriodicThroughput -pDETAILS=1"
CONFIGS["kdse"]=" -athroughputbufferingDSE -prealtime=1 -pmode=KDSE -ptimeout="
CONFIGS["k2dse"]=" -athroughputbufferingDSE -prealtime=1 -pmode=K2DSE -ptimeout="
CONFIGS["k2dsea"]=" -athroughputbufferingDSE -prealtime=1 -pmode=K2DSEA -ptimeout="
CONFIGS["k2dseC"]=" -athroughputbufferingDSE -prealtime=1 -pmode=K2DSE -pusecache=1 -ptimeout="
CONFIGS["k2dseaC"]=" -athroughputbufferingDSE -prealtime=1 -pmode=K2DSEA -pusecache=1 -ptimeout="
CONFIGS["kdseP"]=" -athroughputbufferingDSE -prealtime=1 -pmode=KDSE -pthread= -ptimeout="
CONFIGS["k2dseCP"]=" -athroughputbufferingDSE -prealtime=1 -pmode=K2DSE -pthread= -pusecache=1 -ptimeout="
CONFIGS["k2dseaCP"]=" -athroughputbufferingDSE -prealtime=1 -pmode=K2DSEA -pthread= -pusecache=1 -ptimeout="

# Function to display help
show_help() {
    echo "Usage: $0 [-h] [-t TIMEOUT] [-c CPU] [-l LOGDIR] [-x] [-f] [-d]"
    echo "  -h          Display this help message."
    echo "  -t TIMEOUT  Set a custom timeout for benchmarks."
    echo "  -l LOGDIR   Set a directory for the output logs."
    echo "  -c CPU      Set the thread level for experiments."
    echo "  -d          Run in dry mode (don't run anything)."
    echo "  -f          Run in fast mode (fast benchmarks only)."
    echo "  -x          Run in export mode (if a file already exist, load it)."
}

# Function to check prerequisites
check_prerequisites() {
    if [ ! -x "${KITER}" ]; then
        echo "Error: Kiter is missing: ${KITER} Not found."
        exit 1
    fi
    
    local benchmarks_to_check=("${BENCHMARKS[@]}")
    for f in "${benchmarks_to_check[@]}"; do
        if [ ! -e "$f" ]; then
            echo "Error: $f is missing."
            exit 1
        fi
    done
}

# Apply CPU count to CONFIGS if necessary
update_configs() {
    for key in "${!CONFIGS[@]}"; do
        CONFIGS[$key]="${CONFIGS[$key]/-pthread=/-pthread=$cpu_count}"
        CONFIGS[$key]="${CONFIGS[$key]/-ptimeout=/-ptimeout=$max_time}"
    done
}

# Function to run the benchmarks
run_benchmarks() {
    local logdir=${1}
    local maxtime=${2}
    local exportmode=${3}

    local configargs="-ptimeout=$maxtime"
    
    
 



    
    mkdir -p "${logdir}"
    local command_prefix="timeout --foreground $((max_time + EXTRA_TIMEOUT))s"

    for graphfile in "${BENCHMARKS[@]}"; do
        for i in "${!CONFIGS[@]}"; do
            local config_parameters=${CONFIGS[$i]}
            local graph=$(basename "${graphfile}" .xml)
	    local logfile="${logdir}/${graph}_${i}.txt"
	    if [ "${exportmode}" -eq 1 ]; then
		config_parameters="${config_parameters/-athroughputbufferingDSE/-athroughputbufferingDSE -pexport=\"${logfile}\"}"
	    fi
	    
            echo "${command_prefix} ${KITER} -f ${graphfile} ${config_parameters} > \"${logfile}\""

	    if [ "${dry_mode}" -eq 0 ]; then
		${command_prefix} ${KITER} -f ${graphfile} ${config_parameters} > "${logfile}"
		if cat "${logfile}" | grep -E "[,][-][,][-][,][-][,][-]" -q ; then
		    echo "Interupted!"
		fi 
	    fi
        done
    done
}

# Main function
main() {
    
    local log_dir="${DEFAULT_LOGDIR}"
    local max_time="${DEFAULT_TIMEOUT}"
    local cpu_count="${DEFAULT_CPU_COUNT}"
    local fast_mode=0
    local dry_mode=0
    local export_mode=0

    while getopts "hc:l:t:fxd" opt; do
        case ${opt} in
            h )
                show_help
                exit 0
                ;;
            l )
                log_dir="${OPTARG}"
                ;;
            t )
                max_time="${OPTARG}"
                ;;
	    c )
                cpu_count="${OPTARG}"
                ;;
            x )
                export_mode=1
                ;;
            d )
                dry_mode=1
                ;;
            f )
                fast_mode=1
                ;;
            \? )
                show_help
                exit 1
                ;;
        esac
    done
    
    # Apply dry mode if set
    if [ "${dry_mode}" -eq 1 ]; then
        DRY_MODE=1
    fi
    
    # Apply fast mode if set
    if [ "${fast_mode}" -eq 1 ]; then
        BENCHMARKS=("${FAST_BENCHMARKS[@]}")
    fi

    # Validate and apply CPU count before running benchmarks
    if ! [[ $cpu_count =~ ^[0-9]+$ ]]; then  # ensure it's a number
        echo "Error: CPU count must be a positive integer."
        exit 1
    fi
    
    # Same for max time
    if ! [[ $max_time =~ ^[0-9]+$ ]]; then  # ensure it's a number
        echo "Error: Timeout count must be a positive integer."
        exit 1
    fi
    
    update_configs  # apply the actual CPU count to configurations

    # Check prerequisites
    check_prerequisites

    # Run benchmarks
    run_benchmarks "${log_dir}" "${max_time}" "${export_mode}"
}

# Execute the main function with command line arguments
main "$@"
