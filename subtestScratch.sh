#!/bin/bash
#SBATCH --partition=parallel
#SBATCH --mem=10GB
#SBATCH --cpus-per-task=4

#full node = 96GB
#for 3-5 I used 60 GB and 5 cpus

#module load python/3.6.4
#module load cmake/3.10.2
#module load mpi/ofed/openmpi-1.8.8-gcc5
#module load python/2.7.9
#module load gcc/7.3.0


module load gcc/9.3.0


dphi=$1
D0=$2
dphi_list=($dphi)
D0_list=($D0)
dimY=$3
dim_Y_list=($dimY)


DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)



#dphi_list=(0.516 0.517 0.518 0.519 0.52 0.521 0.522 0.523 0.524)

#D0_list=(1.63 1.62 1.61 1.6 1.59 1.58 1.57 1.56 1.55 1.54 1.53 1.52 1.51 1.5 1.49 1.48 1.47 1.46 1.45 1.44 1.43 1.42 1.41 1.4 1.39)
#D0_list=(1.39 1.4 1.41 1.42 1.43)
#D0_list=(1.3 1.31 1.32 1.33 1.34 1.35)
#D0_list=(1.46 1.47 1.48)
#D0_list=(1.57)
#do 1.52 to 1.53 next
#dim_Y_list=(0.99 0.992 0.994 0.996 0.998 1.0 1.002 1.004 1.006 1.008 1.01)

echo 0 > Counter


mkdir SLURM_Grid_OUTS
mkdir out_dphi

for dphi in "${dphi_list[@]}"; do
   mkdir /scratch/dtsiakoulias/grid_auto6/
   mkdir /scratch/dtsiakoulias/grid_auto6/xml_ck_out_${dphi}
#   cd /scratch/dtsiakoulias/grid_auto/xml_ck_out_${dphi}

     
   for D0 in "${D0_list[@]}"; do
   for dimY in "${dim_Y_list[@]}"; do

   mkdir /scratch/dtsiakoulias/grid_auto6/xml_ck_out_${dphi}_${D0}_${dimY}   

   cp main.cpp /scratch/dtsiakoulias/grid_auto6/xml_ck_out_${dphi}_${D0}_${dimY}/main.cpp
   cp CMakeLists.txt /scratch/dtsiakoulias/grid_auto6/xml_ck_out_${dphi}_${D0}_${dimY}/CMakeLists.txt

   cp sbatchgridScratch.sh /scratch/dtsiakoulias/grid_auto6/xml_ck_out_${dphi}_${D0}_${dimY}/sbatchgrid.sh
###
#   mkdir /scratch/dtsiakoulias/grid_auto/xml_ck_out_${dphi}_${D0}_${dimY}/build
#   cd /scratch/dtsiakoulias/grid_auto/xml_ck_out_${dphi}_${D0}_${dimY}/build
#   /home/dtsiakoulias/qboot/cmake/cmakedir/bin/cmake .. -DQBoot_ROOT=/home/dtsiakoulias/qboot/executable -DCMAKE_BUILD_TYPE=Debug
#   make
#   cd /scratch/dtsiakoulias/grid_auto/xml_ck_out_${dphi}_${D0}_${dimY}/build/bin
#   ./sample-debug ${dphi} ${D0} ${dimY}
###
   mkdir /scratch/dtsiakoulias/grid_auto6/xml_ck_out_${dphi}/xml_ck_out_${dphi}_${D0}
   mkdir /scratch/dtsiakoulias/grid_auto6/xml_ck_out_${dphi}/xml_ck_out_${dphi}
   mkdir /scratch/dtsiakoulias/grid_auto6/xml_ck_out_${dphi}/xml_ck_out_${dphi}/xml_ck_out_${dphi}_${D0}
   sbatch -e ${DIR}/SLURM_Grid_OUTS/${dphi}_${D0}_${dimY}.err -o ${DIR}/SLURM_Grid_OUTS/out${dphi}_${D0}_${dimY}.out /scratch/dtsiakoulias/grid_auto6/xml_ck_out_${dphi}_${D0}_${dimY}/sbatchgrid.sh ${dphi} ${D0} ${dimY}
   done
   done
   cd ${DIR}

done

