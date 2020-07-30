#!/bin/bash
#SBATCH --partition=parallel
#SBATCH --mem=10GB
#SBATCH --cpus-per-task=4



module load gcc/9.3.0
 
dphi=$1
D0=$2
dimY=$3

mkdir /scratch/dtsiakoulias/grid_auto6/xml_ck_out_${dphi}/xml_ck_out_${dphi}/xml_ck_out_${dphi}_${D0}
###
mkdir /scratch/dtsiakoulias/grid_auto6/xml_ck_out_${dphi}_${D0}_${dimY}/build
cd /scratch/dtsiakoulias/grid_auto6/xml_ck_out_${dphi}_${D0}_${dimY}/build
/home/dtsiakoulias/qboot/cmake-3.18.0-rc3/bin/cmake .. -DQBoot_ROOT=/home/dtsiakoulias/qboot/executable -DCMAKE_BUILD_TYPE=Debug
make
cd /scratch/dtsiakoulias/grid_auto6/xml_ck_out_${dphi}_${D0}_${dimY}/build/bin
./sample-debug ${dphi} ${D0} ${dimY}
####



#module unload gcc/9.3.0
#module load gcc/7.3.0


#THE BELOW CODE CHECK THAT THE SDPB INPUTS EXIST BEFORE RUNNING SDPB, IF THEY DO NOT EXIST IT RUNS sbatchgridScratch again


FILE=/scratch/dtsiakoulias/grid_auto6/xml_ck_out_${dphi}_${D0}_${dimY}/build/bin/Cubic_phi-S_mixed_Y_grid-dext${dphi}-dS${D0}-Dmin${dimY}
if [ -d "$FILE" ]; then
 if [ -z "$(ls -A /scratch/dtsiakoulias/grid_auto6/xml_ck_out_${dphi}_${D0}_${dimY}/build/bin/Cubic_phi-S_mixed_Y_grid-dext${dphi}-dS${D0}-Dmin${dimY})" ]; then
    sbatch -e /home/dtsiakoulias/workdir/${dphi}_${D0}_${dimY}.err -o /home/dtsiakoulias/workdir/out${dphi}_${D0}_${dimY}.out /scratch/dtsiakoulias/grid_auto6/xml_ck_out_${dphi}_${D0}_${dimY}/sbatchgrid.sh ${dphi} ${D0} ${dimY}
 else
    sbatch  --nodes=1 --ntasks-per-node=20 -e /scratch/dtsiakoulias/grid_auto6/xml_ck_out_${dphi}/${dphi}_${D0}_${dimY}.err -o /scratch/dtsiakoulias/grid_auto6/xml_ck_out_${dphi}/out${dphi}_${D0}_${dimY}.out  $HOME/workdir/SdpbRunnerScratch ${dphi} ${D0} ${dimY}
 fi
fi



if [ ! -d "$FILE" ]; then
    sbatch -e /home/dtsiakoulias/workdir/${dphi}_${D0}_${dimY}.err -o /home/dtsiakoulias/workdir/out${dphi}_${D0}_${dimY}.out /scratch/dtsiakoulias/grid_auto6/xml_ck_out_${dphi}_${D0}_${dimY}/sbatchgrid.sh ${dphi} ${D0} ${dimY}

fi

