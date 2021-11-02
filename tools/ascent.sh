#!/bin/sh

###
###  This script generates pictures for ASCENT paper.
###  Usage is : sh tools/ascent.sh with Release/bin/kiter available.

ROOT=$(pwd)

OUTDIR=${ROOT}/ascent_pictures/
KITER=${ROOT}/Release/bin/kiter
MOTIV=${ROOT}/benchmarks/NoCMotivation/sdf_mapping.xml

echo "Current directory is ${ROOT}"

if [ ! -x ${KITER} ] ; then
    echo "Kiter executable ${KITER} not found."
    exit 1
fi

if [ ! -e ${MOTIV} ] ; then
    echo "Motivation example ${MOTIV} not found."
    exit 1
fi

if [ ! -d ${OUTDIR} ] ; then
    echo "Output directory ${OUTDIR} not found, we create it."
    mkdir -p ${OUTDIR}/
fi


cd ${OUTDIR}/ || exit # Avoid side-effect inside the main repository.

echo "[Kiter] Produce ${OUTDIR}/mapping.dot"
${KITER} -f ${MOTIV}  -a BufferlessNoCMapAndRoute -v10 -pcolor_routes=1 -a PrintMapping  > ${OUTDIR}/mapping.dot 2> /dev/null

echo "[Kiter] More files: ${KITER} -f ${MOTIV}  -a BufferlessNoCMapAndRoute -v10 -a BufferlessNoCScheduling"
${KITER} -f ${MOTIV}  -a BufferlessNoCMapAndRoute -v10 -a BufferlessNoCScheduling  2> /dev/null

echo "[CP] rename some files"
cp bufferless_noc_schedule_example_*_ModelNoCConflictFreeCommunication_noc.dot modelnoc.dot
cp bufferless_noc_schedule_example_*_end_noc.dot final.dot


echo "[dot] generate pdf files"
dot ./mapping.dot -Kneato -Tpdf -o ./mapping.pdf -Gmargin=0 
dot modelnoc.dot -Kneato -Tpdf -o ./modelnoc.pdf -Gmargin=0 
dot final.dot -Kneato -Tpdf -o ./final.pdf -Gmargin=0 
