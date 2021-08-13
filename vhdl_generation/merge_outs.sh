#!/bin/bash
KITER_DIR="/home/jaime/Documents/research/dataflow/kiter-jaime/" #"/path/to/kiter"
KITER="${KITER_DIR}/Release/bin/kiter"
GRAPH_DIR="/home/jaime/Documents/research/hdl/faust_demo_programs/freeverb/"#"/path/to/generated/vhdl_code"
GRAPH_NAME=#GRAPH_NAME
RES_NAME=#MERGED_NAME

cp ${GRAPH_DIR}${GRAPH_NAME} ${GRAPH_DIR}${RES_NAME}
for component in #ACTOR_NAMES; do
    echo "Merging ${component}"
    ${KITER} -f ${GRAPH_DIR}${RES_NAME} -pname=${component} -a SingleOutput -a PrintXML -v10 > tmp.xml;
    mv tmp.xml ${GRAPH_DIR}${RES_NAME}
done
