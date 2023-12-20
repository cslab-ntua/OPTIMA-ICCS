# export BUILD=Hardware
export BUILD=Emulation-SW
export XCL_EMULATION_MODE=sw_emu

export PROJECT=optima
export CU=8

# export M_sizes='128' # For Emulation-SW
export M_sizes='1024 2048 4096 8192' # For Hardware

export OMP_NUM_THREADS="14"
export DEV_ID="0000:0a:00.1"
for M in $M_sizes; do
    export K=${M}
    export N=${M}
    export alpha="1.2"
    export beta="0.5"
    ./${PROJECT}/${BUILD}/${PROJECT}_${CU} ./${PROJECT}/${BUILD}/binary_container_1_CU${CU}.xclbin ${M} ${K} ${N} ${alpha} ${beta}
done
