inline int read_vector_wide_lu(v_dt* in, hls::stream<v_dt>& inStream,const int N, const int start) {
	unsigned int vSize = ((N - 1) / VDATA_SIZE) + 1;

mem_rd_1:
    for (int i = 0; i < vSize; i++) {
	#pragma HLS pipeline II=1
	#pragma HLS loop_tripcount min=1 max=512

        inStream << in[i+start];
    }

    return 0;
}

inline int read_vector_wide_lu_df(v_dt* in, hls::stream<v_dt>& inStream,const int N, const int start) {
	unsigned int vSize = ((N - 1) / VDATA_SIZE) + 1;

mem_rd_1_df:
    for (int i = 0; i < vSize; i++) {
	#pragma HLS pipeline II=1
	#pragma HLS loop_tripcount min=1 max=512

        inStream << in[i+start];
    }

    return 0;
}

inline void read_vector_wide_in_float_bram2d_lu(v_dt* in, float bram[LU_BRAM_DEPTH][VDATA_SIZE],const int N, const int start) {
	unsigned int vSize = ((N - 1) / VDATA_SIZE) + 1;

mem_rd_2:
    for (int i = 0; i < vSize; i++) {
	#pragma HLS pipeline II=1
	#pragma HLS loop_tripcount min=1 max=512

        for (int j=0;j<VDATA_SIZE;j++) {
			#pragma HLS unroll
        	bram[i][j] = in[i+start].data[j];
        }
    }
}

inline void read_vector_wide_in_float_bram2d_lu_12(v_dt* in, v_dt* bram,const int N, const int start) {
	unsigned int vSize = ((N - 1) / VDATA_SIZE) + 1;
	v_dt tmpData;
	#pragma HLS array_partition variable=tmpData type=complete dim=1

mem_rd_2b:
    for (int i = 0; i < vSize; i++) {
	#pragma HLS pipeline II=1
	#pragma HLS loop_tripcount min=1 max=512

    	tmpData = in[i+start];

        for (int j=0;j<VDATA_SIZE;j++) {
			#pragma HLS unroll
        	bram[i].data[j] = tmpData.data[j];
        }
    }
}

inline void store_stream_wide_in_float_bram2d_lu(hls::stream<v_dt>& inStream, float bram[LU_BRAM_DEPTH][VDATA_SIZE],const int N) {
	unsigned int vSize = ((N - 1) / VDATA_SIZE) + 1;

mem_rd_3:
    for (int i = 0; i < vSize; i++) {
	#pragma HLS pipeline II=1
	#pragma HLS loop_tripcount min=1 max=512

    	v_dt tmpData = inStream.read();

        for (int j=0;j<VDATA_SIZE;j++) {
			#pragma HLS unroll
        	bram[i][j] = tmpData.data[j];
        }
    }
}

inline void broadcast_vector_wide_in_4float_bram2d_lu(v_dt* in, float bram0[LU_BRAM_DEPTH][VDATA_SIZE], float bram1[LU_BRAM_DEPTH][VDATA_SIZE], float bram2[LU_BRAM_DEPTH][VDATA_SIZE], float bram3[LU_BRAM_DEPTH][VDATA_SIZE],const int N, const int start) {
	unsigned int vSize = ((N - 1) / VDATA_SIZE) + 1;

mem_rd_4:

    for (int i = 0; i < vSize; i++) {
	#pragma HLS pipeline II=1
	#pragma HLS loop_tripcount min=1 max=512

        for (int j=0;j<VDATA_SIZE;j++) {
			#pragma HLS unroll
        	bram0[i][j] = in[i+start].data[j];
        	bram1[i][j] = in[i+start].data[j];
        	bram2[i][j] = in[i+start].data[j];
        	bram3[i][j] = in[i+start].data[j];
        }
    }
}

inline void broadcast_vector_wide_in_2float_bram2d_lu(v_dt* in, float bram0[LU_BRAM_DEPTH][VDATA_SIZE], float bram1[LU_BRAM_DEPTH][VDATA_SIZE],const int N, const int start) {
	unsigned int vSize = ((N - 1) / VDATA_SIZE) + 1;

mem_rd_5:

    for (int i = 0; i < vSize; i++) {
	#pragma HLS pipeline II=1
	#pragma HLS loop_tripcount min=1 max=512

        for (int j=0;j<VDATA_SIZE;j++) {
			#pragma HLS unroll
        	bram0[i][j] = in[i+start].data[j];
        	bram1[i][j] = in[i+start].data[j];
        }
    }
}

inline void write_vector_wide_from_float_bram2d_lu(v_dt* out, float bram[LU_BRAM_DEPTH][VDATA_SIZE],const int N, const int start) {
	unsigned int vSize = ((N - 1) / VDATA_SIZE) + 1;

mem_wr_1:
    for (int i = 0; i < vSize; i++) {
	#pragma HLS pipeline II=1
	#pragma HLS loop_tripcount min=1 max=512

        for (int j=0;j<VDATA_SIZE;j++) {
			#pragma HLS unroll
        	out[i+start].data[j] = bram[i][j];
        }
    }
}

inline void write_vector_wide_from_float_bram1d_lu(v_dt* out, float bram[LU_LROWS_BRAM],const int N, const int start) {
	unsigned int vSize = ((N - 1) / VDATA_SIZE) + 1;

mem_wr_2:
    for (int i = 0; i < vSize; i++) {
	#pragma HLS pipeline II=1
	#pragma HLS loop_tripcount min=1 max=512

        for (int j=0;j<VDATA_SIZE;j++) {
			#pragma HLS unroll
        	out[i+start].data[j] = bram[i*VDATA_SIZE+j];
        }
    }
}

inline int write_vector_wide_to_output_and_bram2d_lu(v_dt* out, hls::stream<v_dt>& outStream,float bram[LU_BRAM_DEPTH][VDATA_SIZE],const int N,const int start) {
	unsigned int vSize = ((N - 1) / VDATA_SIZE) + 1;
	v_dt tmpData;

mem_wr_3:
    for (int i = 0; i < vSize; i++) {
		#pragma HLS pipeline II=1

    	tmpData = outStream.read();

        out[i+start] = tmpData;
        for (int j=0;j<VDATA_SIZE;j++) {
			#pragma HLS unroll
			bram[i][j] = tmpData.data[j];
		}
    }

	return 0;
}

inline int write_vector_wide_to_output_and_bram2d_lu_12(v_dt* out, hls::stream<v_dt>& outStream,v_dt* bram,const int N,const int start) {
	unsigned int vSize = ((N - 1) / VDATA_SIZE) + 1;
	v_dt tmpData;
	#pragma HLS array_partition variable=tmpData type=complete dim=1

mem_wr_3b:
    for (int i = 0; i < vSize; i++) {
		#pragma HLS pipeline II=1

    	tmpData = outStream.read();

        for (int j=0;j<VDATA_SIZE;j++) {
			#pragma HLS unroll
			bram[i].data[j] = tmpData.data[j];
		}

        out[i+start] = tmpData;
    }

	return 0;
}

inline int write_vector_wide_lu(v_dt* out, hls::stream<v_dt>& outStream,const int N,const int start) {
	unsigned int vSize = ((N - 1) / VDATA_SIZE) + 1;
	v_dt tmpData;

mem_wr_4:
    for (int i = 0; i < vSize; i++) {
	#pragma HLS pipeline II=1

    	tmpData = outStream.read();
        out[i+start] = tmpData;
    }

	return 0;
}

inline int write_vector_wide_lu_df(v_dt* out, hls::stream<v_dt>& outStream,const int N,const int start) {
	unsigned int vSize = ((N - 1) / VDATA_SIZE) + 1;
	v_dt tmpData;

mem_wr_4_df:
    for (int i = 0; i < vSize; i++) {
	#pragma HLS pipeline II=1

    	tmpData = outStream.read();

        out[i+start] = tmpData;
    }

	return 0;
}

inline int write_wide_float_stream_lu(v_dt* out, hls::stream<float>& outStream,const int N,const int start) {
	unsigned int vSize = ((N - 1) / VDATA_SIZE) + 1;

	v_dt tmpData;
	#pragma HLS array_partition variable=tmpData type=complete dim=1

mem_wr_5:
    for (int i = 0; i < vSize; i++) {
    	for (int j=0;j<VDATA_SIZE;j++) {
		#pragma HLS pipeline II=1
		//#pragma HLS latency max=3
    		if (i*VDATA_SIZE+j < N) {
    			tmpData.data[j] = outStream.read();
    		}
    		else {
    			tmpData.data[j] = 0.0f;
    		}
    	}

    	out[i+start] = tmpData;
    }

    return 0;
}

inline int write_vector_wide_out_and_to_buffer_lu(v_dt* out, v_dt* offChipBuf, hls::stream<v_dt>& outStream,const int N,const int outStart,const int bufStart) {
	unsigned int vSize = ((N - 1) / VDATA_SIZE) + 1;
	static v_dt tmpData;

mem_wr_7:
    for (int i = 0; i < vSize; i++) {
	#pragma HLS pipeline II=1

    	tmpData = outStream.read();

    	offChipBuf[i+bufStart] = tmpData;
        out[i+outStart] = tmpData;
    }

	return 0;
}

inline int write_vector_wide_out_and_to_stream_lu(v_dt* out, hls::stream<v_dt>& bufStream, hls::stream<v_dt>& outStream,const int N,const int outStart) {
	unsigned int vSize = ((N - 1) / VDATA_SIZE) + 1;
	static v_dt tmpData;

mem_wr_8:
    for (int i = 0; i < vSize; i++) {
	#pragma HLS pipeline II=1

    	tmpData = outStream.read();

    	bufStream.write(tmpData);
        out[i+outStart] = tmpData;
    }

	return 0;
}



inline int lu_write_last_snapshot_line(v_dt* out, hls::stream<v_dt>& outStream,const int N,const int start, bool writeLine) {
	unsigned int vSize = ((N - 1) / VDATA_SIZE) + 1;
	v_dt tmpData;

	if (!writeLine) {
		return -1;
	}

mem_wr_last_snapshot_line:
    for (int i = 0; i < vSize; i++) {
	#pragma HLS pipeline II=1

    	tmpData = outStream.read();
        out[i+start] = tmpData;
    }

	return 0;
}

inline void lu_short_auxiliary_process_array_snapshot_row(
		int c, int iterVdtsPerRow,
		bool appendLastResults,
		v_dt* mybramRd,
		hls::stream<v_dt>& URowsStreamRd, hls::stream<v_dt>& URowsStreamWr,
		hls::stream<float>& LRowsStream
	) {

	static v_dt URows;
	#pragma HLS array_partition variable=URows type=complete dim=1

	static v_dt temp;
	#pragma HLS array_partition variable=temp type=complete dim=1

	static v_dt prevBuffer;
	#pragma HLS array_partition variable=prevBuffer type=complete dim=1

	int tmpRowsStreamSize = 0;
	static float xCurRow = 0.0f;

	for (int j=0;j<VDATA_SIZE;j++) {
		#pragma HLS unroll
		temp.data[j] = 0.0f;
		URows.data[j] = 0.0f;
		prevBuffer.data[j] = 0.0f;
	}

#ifndef __SYNTHESIS__
	printf("<---------- lu_short_auxiliary_process_array_snapshot_row START ----------> \n");
	printf("(c=%d): URowsStreamRd size = %d\n",c,(int)URowsStreamRd.size());
#endif

	URows = URowsStreamRd.read();

	xCurRow = URows.data[0] / mybramRd[0].data[0];
#ifndef __SYNTHESIS__
	printf("2. xCurRow set, URows.data[0]=%08f, mybram[0][0]=%08f, xCurRow=%08f\n",URows.data[0],mybramRd[0].data[0],xCurRow);
#endif

#ifndef __SYNTHESIS__
		printf("(c=%d): row_for_loop..\n",c);
#endif
	row_for_loop: for (int bIdx=0;bIdx<iterVdtsPerRow;bIdx++) {
		#pragma HLS pipeline II=1

		if (bIdx == iterVdtsPerRow-1) {  //row's last vdt

			row_last_multi_vdt_process_for_loop: for (int j=0;j<VDATA_SIZE;j++) {
				#pragma HLS unroll
				temp.data[j] = URows.data[j] - xCurRow * mybramRd[bIdx].data[j];
			}
			prevBuffer.data[VDATA_SIZE-1] = temp.data[0];  //prevBuffer is now ready to be written in the URowsHBMBuffer
			if (bIdx == 0) {
				for (int j=1;j<VDATA_SIZE;j++) {
					#pragma HLS unroll
					prevBuffer.data[j-1] = temp.data[j];
				}
			}

			URowsStreamWr.write(prevBuffer);
		}
		else {  //processing row's next vdt
			row_next_vdt_process_for_loop: for (int j=0;j<VDATA_SIZE;j++) {
				#pragma HLS unroll
				temp.data[j] = URows.data[j] - xCurRow * mybramRd[bIdx].data[j];
			}

			if (bIdx > 0) {
				prevBuffer.data[VDATA_SIZE-1] = temp.data[0];  //prevBuffer is now ready to be written in the URowsHBMBuffer

				URowsStreamWr.write(prevBuffer);
			}

			//store to prevBuffer the new results from temp
			for (int j=1;j<VDATA_SIZE;j++) {
				#pragma HLS unroll
				prevBuffer.data[j-1] = temp.data[j];
			}
			prevBuffer.data[VDATA_SIZE-1] = 0.0f;

			URows = URowsStreamRd.read();
		}

	}  //outer for-loop


	//case where the next set of rows will require the same number of vdt, so we need to write temp as well.
	if (appendLastResults) {

		for (int j=1;j<VDATA_SIZE;j++) {
			#pragma HLS unroll
			prevBuffer.data[j-1] = temp.data[j];
		}
		prevBuffer.data[VDATA_SIZE-1] = 0.0f;

		URowsStreamWr.write(prevBuffer);
	}

	LRowsStream.write(xCurRow);

#ifndef __SYNTHESIS__
	printf("(c=%d): URowsStreamRd size = %d\n",c,(int)URowsStreamRd.size());
	printf("(c=%d): URowsStreamWr size = %d\n",c,(int)URowsStreamWr.size());
	printf("(c=%d): LRowsStream size = %d\n",c,(int)LRowsStream.size());
	printf("<---------- lu_short_auxiliary_process_array_snapshot_row END ----------> \n");
#endif

}


inline void lu_process_array_snapshot_row(
		int c, int iterVdtsPerRow,
		bool kernelNotLastRound, bool updateURowsStreamWr, bool appendLastResults,
		v_dt* mybramRd,
		hls::stream<v_dt>& URowsStreamRd, hls::stream<v_dt>& URowsStreamWr, hls::stream<v_dt>& URowsOutStream,
		hls::stream<float>& LRowsStream
	) {

	static v_dt URows;
	#pragma HLS array_partition variable=URows type=complete dim=1

	static v_dt temp;
	#pragma HLS array_partition variable=temp type=complete dim=1

	static v_dt prevBuffer;
	#pragma HLS array_partition variable=prevBuffer type=complete dim=1

	int tmpRowsStreamSize = 0;
	static float xCurRow = 0.0f;

	for (int j=0;j<VDATA_SIZE;j++) {
		#pragma HLS unroll
		temp.data[j] = 0.0f;
		URows.data[j] = 0.0f;
		prevBuffer.data[j] = 0.0f;
	}

#ifndef __SYNTHESIS__
	printf("<---------- lu_process_array_snapshot_row START ----------> \n");
	printf("(c=%d): URowsStreamRd size = %d\n",c,(int)URowsStreamRd.size());
#endif

	URows = URowsStreamRd.read();

	xCurRow = URows.data[0] / mybramRd[0].data[0];

#ifndef __SYNTHESIS__
	printf("xCurRow set, URows.data[0]=%08f, mybram[0][0]=%08f, xCurRow=%08f\n",URows.data[0],mybramRd[0].data[0],xCurRow);
#endif

	//lu_process_array_vdts returns how many elements were processed
#ifndef __SYNTHESIS__
	printf("(c=%d): row_for_loop..\n",c);
#endif

	row_for_loop: for (int bIdx=0;bIdx<iterVdtsPerRow;bIdx++) {
		#pragma HLS pipeline II=1

		if (bIdx == iterVdtsPerRow-1) {  //row's last vdt

			row_last_vdt_process_for_loop: for (int j=0;j<VDATA_SIZE;j++) {
				#pragma HLS unroll
				temp.data[j] = URows.data[j] - xCurRow * mybramRd[bIdx].data[j];
			}
			prevBuffer.data[VDATA_SIZE-1] = temp.data[0];  //prevBuffer is now ready to be written in the URowsHBMBuffer
			if (bIdx == 0) {

				for (int j=1;j<VDATA_SIZE;j++) {
					#pragma HLS unroll
					prevBuffer.data[j-1] = temp.data[j];
				}
			}

			URowsOutStream.write(prevBuffer);  //write the last vdt also to the output stream

			if ((kernelNotLastRound) && (updateURowsStreamWr)) {
				//don't write the last vdt of the last row to the internal URowsHBMBuffer to avoid leaving leftover data
				URowsStreamWr.write(prevBuffer);
			}
		}
		else {  //processing row's next vdt
			row_next_vdt_process_for_loop: for (int j=0;j<VDATA_SIZE;j++) {
				#pragma HLS unroll
				temp.data[j] = URows.data[j] - xCurRow * mybramRd[bIdx].data[j];
			}

			if (bIdx > 0) {
				prevBuffer.data[VDATA_SIZE-1] = temp.data[0];  //prevBuffer is now ready to be written in the URowsHBMBuffer

				if ((kernelNotLastRound) && (updateURowsStreamWr)) {
					URowsStreamWr.write(prevBuffer);
				}

				URowsOutStream.write(prevBuffer);  //write the vdt also to the output stream
			}

			//store to prevBuffer the new results from temp
			for (int j=1;j<VDATA_SIZE;j++) {
				#pragma HLS unroll
				prevBuffer.data[j-1] = temp.data[j];
			}
			prevBuffer.data[VDATA_SIZE-1] = 0.0f;

			URows = URowsStreamRd.read();
		}
	}  //outer for-loop

	//case where the next set of rows will require the same number of vdt, so we need to write temp as well.
	if (appendLastResults) {

		for (int j=1;j<VDATA_SIZE;j++) {
			#pragma HLS unroll
			prevBuffer.data[j-1] = temp.data[j];
		}
		prevBuffer.data[VDATA_SIZE-1] = 0.0f;

		URowsOutStream.write(prevBuffer);  //write the last vdt also to the output stream

		if ((kernelNotLastRound) && (updateURowsStreamWr)) {
			//don't write the last vdt of the last row to the internal URowsStreamWr to avoid leaving leftover data
			URowsStreamWr.write(prevBuffer);
		}
	}

	LRowsStream.write(xCurRow);

#ifndef __SYNTHESIS__
	printf("(c=%d): URowsStreamRd size = %d\n",c,(int)URowsStreamRd.size());
	printf("(c=%d): URowsStreamWr size = %d\n",c,(int)URowsStreamWr.size());
	printf("(c=%d): LRowsStream size = %d\n",c,(int)LRowsStream.size());
	printf("(c=%d): URowsOutStream size = %d\n",c,(int)URowsOutStream.size());
	printf("<---------- lu_process_array_snapshot_row END ----------> \n");
#endif
}

inline void lu_update_L(int c, v_dt* SLRows0Out, hls::stream<float>& LRowsStream, int* SLRows0OutStartIdx, int totalProcessedRows) {

	int tmpLRowsLastElements = 0, tmpPaddedTotalProcessedRows = 0, tmpTotalProcessedRows = 0;

	//round totalProcessedRows to next multiple of VDATA_SIZE
	tmpLRowsLastElements = totalProcessedRows % VDATA_SIZE;
	if (tmpLRowsLastElements == 0) {
		tmpPaddedTotalProcessedRows = totalProcessedRows;
	}
	else {
		tmpPaddedTotalProcessedRows = totalProcessedRows + VDATA_SIZE - tmpLRowsLastElements;
	}

#ifndef __SYNTHESIS__
	printf("lu_update_L: (c=%d): must write %d elements, padded to %d elements..\n",c,totalProcessedRows,tmpPaddedTotalProcessedRows);
	printf("lu_update_L: (c=%d): write_wide_float_stream_lu %d elements to SLRows0Out[%d]..\n",c,tmpPaddedTotalProcessedRows,*SLRows0OutStartIdx/VDATA_SIZE);
#endif

	write_wide_float_stream_lu(SLRows0Out,LRowsStream,totalProcessedRows,*SLRows0OutStartIdx/VDATA_SIZE);

	*SLRows0OutStartIdx+=tmpPaddedTotalProcessedRows;
}

inline void lu_update_row_boundaries(
		int remRows, int *lastVdtElements, int* iterPaddedRowWidth, int *iterRowsElementsPadded, int *iterVdtsPerRow, int* HBMBufferElemNumToWrite,
		bool* appendLastResults
	) {

	//round iterPaddedRowWidth to next multiple of VDATA_SIZE
	*lastVdtElements = remRows % VDATA_SIZE;
	if (*lastVdtElements == 0) {
		*iterPaddedRowWidth = remRows;
		*lastVdtElements = VDATA_SIZE; //overwrite lastVdtElements = VDATA_SIZE since the last vdt has VDATA_SIZE elements (and not 0)
	}
	else {
		*iterPaddedRowWidth = remRows + VDATA_SIZE - *lastVdtElements;
	}

	*iterRowsElementsPadded = remRows * (*iterPaddedRowWidth);  //padded rows have remRows*iterPaddedRowWidth elements in total (including pivot row to be cached in bram)
	*iterVdtsPerRow = *iterPaddedRowWidth / VDATA_SIZE;  //each padded row has iterPaddedRowWidth / VDATA_SIZE vdts!

	//do not append the last set of results if the next set of rows will have 1 less vdt or each row fits within a single vdt
	*appendLastResults = (*lastVdtElements-1 > 0) && (*iterVdtsPerRow > 1);

	if (*iterVdtsPerRow == 1) {  //case where the output row fits within a single vdt
		*HBMBufferElemNumToWrite = *iterPaddedRowWidth;
	}
	else {
		if (*lastVdtElements > 1) {  //case where the output row will comprise the same vdts compared to the input row
			*HBMBufferElemNumToWrite = *iterPaddedRowWidth;
		}
		else {  //case where the output row will comprise 1 less vdt compared to the input row
			*HBMBufferElemNumToWrite = *iterPaddedRowWidth - VDATA_SIZE;
		}
	}
#ifndef __SYNTHESIS__
	printf("lu_update_row_boundaries: (remRows=%d) appendLastResults=%d, lastVdtElements=%d, iterPaddedRowWidth=%d, iterRowsElementsPadded=%d, iterVdtsPerRow=%d, HBMBufferElemNumToWrite=%d\n",remRows,*appendLastResults,*lastVdtElements,*iterPaddedRowWidth,*iterRowsElementsPadded,*iterVdtsPerRow,*HBMBufferElemNumToWrite);
#endif
}


inline void dfLine3(
		int *inputRowIdx, int c, int *SURows0OutStartIdx,
		int iterPaddedRowWidth, int iterVdtsPerRow,
		int iterVdtsPerRow_2,
		int iterVdtsPerRow_3,
		int iterVdtsPerRow_4, int HBMBufferElemNumToWrite_4,
		v_dt *SURowsHBMBufferRd, v_dt *SURowsHBMBufferWr, v_dt *mybram, v_dt *SURows0Out, v_dt *mybram_2, v_dt *mybram_3, v_dt *mybram_4,
		bool kernelNotLastRound,
		bool appendLastResults, bool appendLastResults_2, bool appendLastResults_3, bool appendLastResults_4,
		hls::stream<float>& LRowsStreamA, hls::stream<float>& LRowsStreamB, hls::stream<float>& LRowsStreamC, hls::stream<float>& LRowsStreamD
	) {
	#pragma HLS dataflow

	static hls::stream<v_dt> URowsStreamRd("URowsStreamRd");
	#pragma HLS STREAM variable = URowsStreamRd depth = 64

	static hls::stream<v_dt> URowsStreamTmp("URowsStreamTmp");
	#pragma HLS STREAM variable = URowsStreamTmp depth = 64

	static hls::stream<v_dt> URowsStreamTmp2("URowsStreamTmp2");
	#pragma HLS STREAM variable = URowsStreamTmp2 depth = 64

	static hls::stream<v_dt> URowsStreamTmp3("URowsStreamTmp3");
	#pragma HLS STREAM variable = URowsStreamTmp3 depth = 64

	static hls::stream<v_dt> URowsStreamWr("URowsStreamWr");
	#pragma HLS STREAM variable = URowsStreamWr depth = 64

	static hls::stream<v_dt> URowsOutStream("URowsOutStream");
	#pragma HLS STREAM variable = URowsOutStream depth = 64

	int readVectorStartDf = (*inputRowIdx)*iterVdtsPerRow;
	int writeVectorStartDf = (*SURows0OutStartIdx)/VDATA_SIZE;
	int secondProcC = c+1, thirdProcC = c+2, fourthProcC = c+3;

#ifndef __SYNTHESIS__
	printf("dfLine3: 14. (c=%d): READ BUFFER (%d elements) SURowsHBMBufferRd[%d] ==> URowsStreamRd..\n",c,iterPaddedRowWidth,(*inputRowIdx));
#endif
	read_vector_wide_lu(SURowsHBMBufferRd,URowsStreamRd,iterPaddedRowWidth,readVectorStartDf);

#ifndef __SYNTHESIS__
	printf("dfLine3: 15. (c=%d): lu_short_auxiliary_process_array_snapshot_row URowsStreamRd => URowsStreamTmp, LRowsStreamA..\n",c);
#endif
	//1st process of next row (sends results from URowsStreamRd -> URowsStreamTmp, LRowsStreamA!)
	lu_short_auxiliary_process_array_snapshot_row(c,iterVdtsPerRow,appendLastResults,mybram,URowsStreamRd,URowsStreamTmp,LRowsStreamA);

#ifndef __SYNTHESIS__
	printf("dfLine3: 16. (c=%d): lu_short_auxiliary_process_array_snapshot_row URowsStreamTmp => URowsStreamTmp2, LRowsStreamB..\n",c);
#endif
	//2nd process of next row (sends results from URowsStreamTmp -> URowsStreamTmp2, LRowsStreamB!)
	lu_short_auxiliary_process_array_snapshot_row(secondProcC,iterVdtsPerRow_2,appendLastResults_2,mybram_2,URowsStreamTmp,URowsStreamTmp2,LRowsStreamB);

#ifndef __SYNTHESIS__
	printf("dfLine3: 17. (c=%d): lu_short_auxiliary_process_array_snapshot_row URowsStreamTmp2 => URowsStreamTmp3, LRowsStreamC..\n",c);
#endif
	//3rd process of next row (sends results from URowsStreamTmp2 -> URowsStreamTmp3, LRowsStreamC!)
	lu_short_auxiliary_process_array_snapshot_row(thirdProcC,iterVdtsPerRow_3,appendLastResults_3,mybram_3,URowsStreamTmp2,URowsStreamTmp3,LRowsStreamC);

#ifndef __SYNTHESIS__
	printf("dfLine3: 18. (c=%d): lu_process_array_snapshot_row URowsStreamTmp3 => URowsOutStream, URowsStreamWr, LRowsStreamD..\n",c);
#endif
	//4th process of next row (sends results from URowsStreamTmp3 -> URowsOutStream, LRowsStreamD, URowsStreamWr!)
	lu_process_array_snapshot_row(fourthProcC,iterVdtsPerRow_4,kernelNotLastRound,true,appendLastResults_4,mybram_4,URowsStreamTmp3,URowsStreamWr,URowsOutStream,LRowsStreamD);

#ifndef __SYNTHESIS__
	printf("dfLine3: 19. (c=%d): WRITE OUT appendLastResults = %d, (%d elements) URowsOutStream ==> SURows0Out[%d]..\n",c,appendLastResults_4,HBMBufferElemNumToWrite_4,(*SURows0OutStartIdx)/(iterVdtsPerRow_4*VDATA_SIZE));
#endif
	write_vector_wide_lu(SURows0Out,URowsOutStream,HBMBufferElemNumToWrite_4,writeVectorStartDf);

	//write new row back also SURowsHBMBufferWr
#ifndef __SYNTHESIS__
	printf("dfLine3: 20. (c=%d): WRITE BUFFER (%d elements) URowsStreamWr ==> SURowsHBMBufferWr[0]..\n",c,HBMBufferElemNumToWrite_4);
#endif
	//write_vector_wide_lu(SURowsHBMBufferWr,URowsStreamWr,HBMBufferElemNumToWrite_4,0);
	lu_write_last_snapshot_line(SURowsHBMBufferWr,URowsStreamWr,HBMBufferElemNumToWrite_4,0,kernelNotLastRound);

}


inline void dfLine2(
		int *inputRowIdx, int c, int *SURows0OutStartIdx,
		int iterPaddedRowWidth, int iterVdtsPerRow,
		int iterVdtsPerRow_2,
		int iterVdtsPerRow_3, int HBMBufferElemNumToWrite_3,
		v_dt *SURowsHBMBufferRd, v_dt *mybram, v_dt *SURows0Out, v_dt *mybram_2, v_dt *mybram_3, v_dt *mybram_4,
		bool kernelNotLastRound,
		bool appendLastResults, bool appendLastResults_2, bool appendLastResults_3,
		hls::stream<float>& LRowsStreamA, hls::stream<float>& LRowsStreamB, hls::stream<float>& LRowsStreamC
	) {

	#pragma HLS dataflow

	static hls::stream<v_dt> URowsStreamRd("URowsStreamRd");
	#pragma HLS STREAM variable = URowsStreamRd depth = 64

	static hls::stream<v_dt> URowsStreamTmp("URowsStreamTmp");
	#pragma HLS STREAM variable = URowsStreamTmp depth = 64

	static hls::stream<v_dt> URowsStreamTmp2("URowsStreamTmp2");
	#pragma HLS STREAM variable = URowsStreamTmp2 depth = 64

	static hls::stream<v_dt> URowsStreamWr("URowsStreamWr");
	#pragma HLS STREAM variable = URowsStreamWr depth = 1

	static hls::stream<v_dt> URowsOutStream("URowsOutStream");
	#pragma HLS STREAM variable = URowsOutStream depth = 64

	int readVectorStartDf = (*inputRowIdx)*iterVdtsPerRow;
	int writeVectorStartDf = (*SURows0OutStartIdx)/VDATA_SIZE;
	int secondProcC = c+1, thirdProcC = c+2;

#ifndef __SYNTHESIS__
	printf("dfLine2 9. (c=%d): READ BUFFER (%d elements) SURowsHBMBufferRd[%d] ==> URowsStreamRd..\n",c,iterPaddedRowWidth,(*inputRowIdx));
#endif
	read_vector_wide_lu(SURowsHBMBufferRd,URowsStreamRd,iterPaddedRowWidth,readVectorStartDf);

#ifndef __SYNTHESIS__
	printf("dfLine2 10. (c=%d): lu_short_auxiliary_process_array_snapshot_row URowsStreamRd => URowsStreamTmp, LRowsStreamA..\n",c);
#endif
	//1st process of next row (sends results from URowsStreamRd -> URowsStreamTmp, LRowsStreamA!)
	lu_short_auxiliary_process_array_snapshot_row(c,iterVdtsPerRow,appendLastResults,mybram,URowsStreamRd,URowsStreamTmp,LRowsStreamA);

#ifndef __SYNTHESIS__
	printf("dfLine2 11. (c=%d): lu_short_auxiliary_process_array_snapshot_row URowsStreamTmp => URowsStreamTmp2, LRowsStreamB..\n",c);
#endif
	//2nd process of next row (sends results from URowsStreamTmp -> URowsStreamTmp2, LRowsStreamB!)
	lu_short_auxiliary_process_array_snapshot_row(secondProcC,iterVdtsPerRow_2,appendLastResults_2,mybram_2,URowsStreamTmp,URowsStreamTmp2,LRowsStreamB);

#ifndef __SYNTHESIS__
	printf("dfLine2 12. (c=%d): lu_process_array_snapshot_row URowsStreamTmp2 => URowsOutStream, URowsStreamWr, LRowsStreamC..\n",c);
#endif
	//3rd process of next row (sends results from URowsStreamTmp2 -> URowsOutStream, LRowsStreamC, does not update URowsStreamWr!)
	lu_process_array_snapshot_row(thirdProcC,iterVdtsPerRow_3,kernelNotLastRound,false,appendLastResults_3,mybram_3,URowsStreamTmp2,URowsStreamWr,URowsOutStream,LRowsStreamC);
	//write new row back to SURows0Out and also cache it to the 4th bram as 4th pivot row
#ifndef __SYNTHESIS__
	printf("dfLine2 13. (c=%d): WRITE OUT AND CACHE appendLastResults = %d, (%d elements) URowsOutStream ==> SURows0Out[%d], cache 4..\n",c,appendLastResults_3,HBMBufferElemNumToWrite_3,(*SURows0OutStartIdx)/(iterVdtsPerRow_3*VDATA_SIZE));
#endif
	write_vector_wide_to_output_and_bram2d_lu_12(SURows0Out,URowsOutStream,mybram_4,HBMBufferElemNumToWrite_3,writeVectorStartDf);
}

inline void dfLine1(
		int *inputRowIdx, int c, int *SURows0OutStartIdx,
		int iterPaddedRowWidth, int iterVdtsPerRow,
		int iterVdtsPerRow_2, int HBMBufferElemNumToWrite_2,
		v_dt *SURowsHBMBufferRd, v_dt *mybram, v_dt *SURows0Out, v_dt *mybram_2, v_dt *mybram_3,
		bool kernelNotLastRound,
		bool appendLastResults, bool appendLastResults_2,
		hls::stream<float>& LRowsStreamA, hls::stream<float>& LRowsStreamB
	) {
	#pragma HLS dataflow

	static hls::stream<v_dt> URowsStreamRd("URowsStreamRd");
	#pragma HLS STREAM variable = URowsStreamRd depth = 64

	static hls::stream<v_dt> URowsStreamTmp("URowsStreamTmp");
	#pragma HLS STREAM variable = URowsStreamTmp depth = 64

	static hls::stream<v_dt> URowsStreamWr("URowsStreamWr");
	#pragma HLS STREAM variable = URowsStreamWr depth = 1

	static hls::stream<v_dt> URowsOutStream("URowsOutStream");
	#pragma HLS STREAM variable = URowsOutStream depth = 64

	int readVectorStartDf = (*inputRowIdx)*iterVdtsPerRow;
	int writeVectorStartDf = (*SURows0OutStartIdx)/VDATA_SIZE;
	int secondProcC = c+1;

#ifndef __SYNTHESIS__
		printf("dfLine1 5. (c=%d): READ BUFFER (%d elements) SURowsHBMBufferRd[%d] ==> URowsStreamRd.\n",c,iterPaddedRowWidth,(*inputRowIdx));
#endif
		read_vector_wide_lu(SURowsHBMBufferRd,URowsStreamRd,iterPaddedRowWidth,readVectorStartDf);

#ifndef __SYNTHESIS__
		printf("dfLine1 6. (c=%d): lu_short_auxiliary_process_array_snapshot_row URowsStreamRd => URowsStreamTmp, LRowsStreamA..\n",c);
#endif
		//1st process of next row (sends results from URowsStreamRd -> URowsStreamTmp, LRowsStreamA!)
		lu_short_auxiliary_process_array_snapshot_row(c,iterVdtsPerRow,appendLastResults,mybram,URowsStreamRd,URowsStreamTmp,LRowsStreamA);

#ifndef __SYNTHESIS__
		printf("dfLine1 7. (c=%d): lu_process_array_snapshot_row URowsStreamTmp => URowsOutStream, URowsStream2, LRowsStreamB..\n",c);
#endif
		//2nd process of next row (sends results from URowsStreamTmp -> URowsOutStream, LRowsStreamB, does not update URowsStreamWr!)
		lu_process_array_snapshot_row(secondProcC,iterVdtsPerRow_2,kernelNotLastRound,false,appendLastResults_2,mybram_2,URowsStreamTmp,URowsStreamWr,URowsOutStream,LRowsStreamB);

		//write new row back to SURows0Out and also cache it to the 3rd bram as 3rd pivot row
#ifndef __SYNTHESIS__
		printf("dfLine1 8. (c=%d): WRITE OUT AND CACHE appendLastResults = %d, (%d elements) URowsOutStream ==> SURows0Out[%d], cache 3..\n",c,appendLastResults_2,HBMBufferElemNumToWrite_2,(*SURows0OutStartIdx)/(iterVdtsPerRow_2*VDATA_SIZE));
#endif
		write_vector_wide_to_output_and_bram2d_lu_12(SURows0Out,URowsOutStream,mybram_3,HBMBufferElemNumToWrite_2,writeVectorStartDf);
}

inline void dfLine0(
		int *inputRowIdx, int c, int *SURows0OutStartIdx,
		int iterPaddedRowWidth, int iterVdtsPerRow, int HBMBufferElemNumToWrite,
		v_dt *SURowsHBMBufferRd, v_dt *mybram, v_dt *SURows0Out, v_dt *mybram_2,
		bool kernelNotLastRound,
		bool appendLastResults,
		hls::stream<float>& LRowsStreamA
	){
	#pragma HLS dataflow

	static hls::stream<v_dt> URowsStreamRd("URowsStreamRd");
	#pragma HLS STREAM variable = URowsStreamRd depth = 64

	static hls::stream<v_dt> URowsStreamWr("URowsStreamWr");
	#pragma HLS STREAM variable = URowsStreamWr depth = 1

	static hls::stream<v_dt> URowsOutStream("URowsOutStream");
	#pragma HLS STREAM variable = URowsOutStream depth = 64

	int readVectorStartDf = (*inputRowIdx)*iterVdtsPerRow;
	int writeVectorStartDf = (*SURows0OutStartIdx)/VDATA_SIZE;

	//read 1st row from SURowsHBMBuffer to URowsStream
#ifndef __SYNTHESIS__
	printf("dfLine0: 2. (c=%d): READ BUFFER (%d elements) SURowsHBMBufferRd[%d] ==> URowsStreamRd..\n",c,iterPaddedRowWidth,(*inputRowIdx));
#endif
	read_vector_wide_lu(SURowsHBMBufferRd,URowsStreamRd,iterPaddedRowWidth,readVectorStartDf);

	//process next row (sends results from URowsStreamRd -> URowsOutStream, LRowsStreamA, does not update URowsStreamWr!)
#ifndef __SYNTHESIS__
	printf("dfLine0: 3. (c=%d): lu_process_array_snapshot_row, URowsStreamRd => URowsOutStream, LRowsStreamA..\n",c);
#endif
	lu_process_array_snapshot_row(c,iterVdtsPerRow,kernelNotLastRound,false,appendLastResults,mybram,URowsStreamRd,URowsStreamWr,URowsOutStream,LRowsStreamA);

	//write new row back to SURows0Out and also cache it to the 2nd bram as 2nd pivot row
#ifndef __SYNTHESIS__
	printf("dfLine0: 4. (c=%d): WRITE OUT AND CACHE appendLastResults = %d, (%d elements) URowsOutStream ==> SURows0Out[%d], cache 2..\n",c,appendLastResults,HBMBufferElemNumToWrite,(*SURows0OutStartIdx)/(iterVdtsPerRow*VDATA_SIZE));
#endif
	write_vector_wide_to_output_and_bram2d_lu_12(SURows0Out,URowsOutStream,mybram_2,HBMBufferElemNumToWrite,writeVectorStartDf);
}

inline void dfUpdateRowBoundaries(
		int remRows, int remRows_2, int remRows_3, int remRows_4,
		int *lastVdtElements, int *lastVdtElements_2, int *lastVdtElements_3, int *lastVdtElements_4,
		int *iterPaddedRowWidth, int *iterPaddedRowWidth_2, int *iterPaddedRowWidth_3, int *iterPaddedRowWidth_4,
		int *iterRowsElementsPadded, int *iterRowsElementsPadded_2, int *iterRowsElementsPadded_3, int *iterRowsElementsPadded_4,
		int *iterVdtsPerRow, int *iterVdtsPerRow_2, int *iterVdtsPerRow_3, int *iterVdtsPerRow_4,
		int *HBMBufferElemNumToWrite, int *HBMBufferElemNumToWrite_2, int *HBMBufferElemNumToWrite_3, int *HBMBufferElemNumToWrite_4,
		bool *appendLastResults, bool *appendLastResults_2, bool *appendLastResults_3, bool *appendLastResults_4
	) {
	#pragma HLS dataflow

	lu_update_row_boundaries(remRows,lastVdtElements,iterPaddedRowWidth,iterRowsElementsPadded,iterVdtsPerRow,HBMBufferElemNumToWrite,appendLastResults);
	lu_update_row_boundaries(remRows_2,lastVdtElements_2,iterPaddedRowWidth_2,iterRowsElementsPadded_2,iterVdtsPerRow_2,HBMBufferElemNumToWrite_2,appendLastResults_2);
	lu_update_row_boundaries(remRows_3,lastVdtElements_3,iterPaddedRowWidth_3,iterRowsElementsPadded_3,iterVdtsPerRow_3,HBMBufferElemNumToWrite_3,appendLastResults_3);
	lu_update_row_boundaries(remRows_4,lastVdtElements_4,iterPaddedRowWidth_4,iterRowsElementsPadded_4,iterVdtsPerRow_4,HBMBufferElemNumToWrite_4,appendLastResults_4);

}


inline void dfLoop(
		v_dt* SURowsHBMBufferRd, v_dt* SURowsHBMBufferWr,
		int i, int inputRowIdx, int c, int iterVdtsPerRow, int iterPaddedRowWidth,
		int iterVdtsPerRow_2, int iterPaddedRowWidth_2,
		int iterVdtsPerRow_3, int iterPaddedRowWidth_3,
		int iterVdtsPerRow_4, int HBMBufferElemNumToWrite_4, int iterPaddedRowWidth_4,
		bool appendLastResults, bool appendLastResults_2, bool appendLastResults_3, bool appendLastResults_4,
		hls::stream<float>& LRowsStreamA, hls::stream<float>& LRowsStreamB, hls::stream<float>& LRowsStreamC, hls::stream<float>& LRowsStreamD,
		v_dt* mybram,v_dt* mybram_2,v_dt* mybram_3,v_dt* mybram_4
	) {
	#pragma HLS dataflow

	static hls::stream<v_dt> URowsStreamRdDf("URowsStreamRdDf");
	#pragma HLS STREAM variable = URowsStreamRdDf depth = 128
	static hls::stream<v_dt> URowsStreamTmpDf("URowsStreamTmpDf");
	#pragma HLS STREAM variable = URowsStreamTmpDf depth = 128
	static hls::stream<v_dt> URowsStreamTmpDf2("URowsStreamTmpDf2");
	#pragma HLS STREAM variable = URowsStreamTmpDf2 depth = 128
	static hls::stream<v_dt> URowsStreamTmpDf3("URowsStreamTmpDf3");
	#pragma HLS STREAM variable = URowsStreamTmpDf3 depth = 128
	static hls::stream<v_dt> URowsStreamWrDf("URowsStreamWrDf");
	#pragma HLS STREAM variable = URowsStreamWrDf depth = 128

	int iDf = inputRowIdx, cDf = c, iterVdtsPerRowDf = iterVdtsPerRow;
	int iterPaddedRowWidthDf = iterPaddedRowWidth;
	bool appendLastResultsDf = appendLastResults;
	int iDf_2 = inputRowIdx, cDf_2 = c+1, iterVdtsPerRowDf_2 = iterVdtsPerRow_2;
	int iDf_3 = inputRowIdx, cDf_3 = c+2, iterVdtsPerRowDf_3 = iterVdtsPerRow_3;
	int iDf_4 = inputRowIdx, cDf_4 = c+3, iterVdtsPerRowDf_4 = iterVdtsPerRow_4;
	int iterPaddedRowWidthDf_2 = iterPaddedRowWidth_2;
	int iterPaddedRowWidthDf_3 = iterPaddedRowWidth_3;
	int HBMBufferElemNumToWriteDf_4 = HBMBufferElemNumToWrite_4, iterPaddedRowWidthDf_4 = iterPaddedRowWidth_4;
	bool appendLastResultsDf_2 = appendLastResults_2, appendLastResultsDf_3 = appendLastResults_3, appendLastResultsDf_4 = appendLastResults_4;

	int tmpHBMBufferStartIdx = (i+1)*HBMBufferElemNumToWriteDf_4;
	int readVectorStartDf = (i+iDf)*iterVdtsPerRowDf;
	int writeVectorStartDf = tmpHBMBufferStartIdx/VDATA_SIZE;

	//read next row to URowsStream
#ifndef __SYNTHESIS__
	printf("DF1. (c=%d): READ BUFFER (%d elements) SURowsHBMBufferRd[%d] ==> URowsStreamRdDf..\n",c,iterPaddedRowWidthDf,(i+iDf));
#endif

	read_vector_wide_lu(SURowsHBMBufferRd,URowsStreamRdDf,iterPaddedRowWidthDf,readVectorStartDf);

#ifndef __SYNTHESIS__
	printf("DF2. (c=%d): lu_short_auxiliary_process_array_snapshot_row_df..\n",c);
#endif
	//1st process of next row (sends results from URowsStreamRdDf -> URowsStreamTmpDf, LRowsStreamA!)
	lu_short_auxiliary_process_array_snapshot_row(cDf,iterVdtsPerRowDf,appendLastResultsDf,mybram,URowsStreamRdDf,URowsStreamTmpDf,LRowsStreamA);

#ifndef __SYNTHESIS__
	printf("DF3. (c=%d): lu_short_auxiliary_process_array_snapshot_row_df..\n",c);
#endif
	//2nd process of next row (sends results from URowsStreamTmpDf -> URowsStreamTmpDf2, LRowsStream_B!)
	lu_short_auxiliary_process_array_snapshot_row(cDf_2,iterVdtsPerRowDf_2,appendLastResults_2,mybram_2,URowsStreamTmpDf,URowsStreamTmpDf2,LRowsStreamB);

#ifndef __SYNTHESIS__
	printf("DF4. (c=%d): lu_short_auxiliary_process_array_snapshot_row_df..\n",c);
#endif
	//3rd process of next row (sends results from URowsStreamTmpDf2 -> URowsStreamWrDf, LRowsStream_C!)
	lu_short_auxiliary_process_array_snapshot_row(cDf_3,iterVdtsPerRowDf_3,appendLastResults_3,mybram_3,URowsStreamTmpDf2,URowsStreamTmpDf3,LRowsStreamC);

#ifndef __SYNTHESIS__
	printf("DF5. (c=%d): lu_short_auxiliary_process_array_snapshot_row_df..\n",c);
#endif
	//4th process of next row (sends results from URowsStreamTmpDf3 -> URowsStreamWrDf, LRowsStream_D!)
	lu_short_auxiliary_process_array_snapshot_row(cDf_4,iterVdtsPerRowDf_4,appendLastResults_4,mybram_4,URowsStreamTmpDf3,URowsStreamWrDf,LRowsStreamD);

	//write new row back to SURowsHBMBufferWr
#ifndef __SYNTHESIS__
	printf("DF6. (c=%d): appendLastResults = %d, WRITE BUFFER (%d elements) URowsStreamWrDf ==> SURowsHBMBufferWr[%d]\n",c,appendLastResultsDf_4,HBMBufferElemNumToWriteDf_4,tmpHBMBufferStartIdx/(iterVdtsPerRowDf_4*VDATA_SIZE));
#endif
	write_vector_wide_lu(SURowsHBMBufferWr,URowsStreamWrDf,HBMBufferElemNumToWriteDf_4,writeVectorStartDf);
}

extern "C" {
	void krnl_lu3d14df(
			const int N,v_dt* SURows0Out,v_dt* SLRows0Out,v_dt* SURowsHBMBuffer,v_dt* SURowsHBMBuffer2
	) {
		#pragma HLS INTERFACE s_axilite port = N
		#pragma HLS INTERFACE m_axi port = SURows0Out offset = slave bundle = hbm0
		#pragma HLS INTERFACE m_axi port = SLRows0Out offset = slave bundle = hbm1
		#pragma HLS INTERFACE m_axi port = SURowsHBMBuffer offset = slave bundle = hbm2
		#pragma HLS INTERFACE m_axi port = SURowsHBMBuffer2 offset = slave bundle = hbm3

		static v_dt mybram[LU_BRAM_DEPTH];  //stores the pivot row
		#pragma HLS array_partition variable=mybram type=complete dim=2

		static v_dt mybram_2[LU_BRAM_DEPTH];  //stores the 2nd pivot row
		#pragma HLS array_partition variable=mybram_2 type=complete dim=2

		static v_dt mybram_3[LU_BRAM_DEPTH];  //stores the 3rd pivot row
		#pragma HLS array_partition variable=mybram_3 type=complete dim=2

		static v_dt mybram_4[LU_BRAM_DEPTH];  //stores the 4th pivot row
		#pragma HLS array_partition variable=mybram_4 type=complete dim=2

		static hls::stream<float> LRowsStream("LRowsStream");
		#pragma HLS STREAM variable = LRowsStream depth = 12288
		static hls::stream<float> LRowsStream_2("LRowsStream_2");
		#pragma HLS STREAM variable = LRowsStream_2 depth = 12288
		static hls::stream<float> LRowsStream_3("LRowsStream_3");
		#pragma HLS STREAM variable = LRowsStream_3 depth = 12288
		static hls::stream<float> LRowsStream_4("LRowsStream_4");
		#pragma HLS STREAM variable = LRowsStream_4 depth = 12288

		static int inputRowIdx = 0;
		static int c = 0;  //iterates through N (outer iteration)

		static int remRows = 0;  //remaining rows
		static int lastVdtElements = 0;  //number of valid elements in each row's last vdt
		static int iterPaddedRowWidth = 0;  //padded row width for an iteration
		static int iterRowsElementsPadded = 0;  //number of SURows elements, including padding
		static int iterVdtsPerRow = 0;  //number of vdts per row including padding
		static int HBMBufferElemNumToWrite = 0;
		static bool appendLastResults = false;

		static int remRows_2 = 0;  //remaining rows
		static int lastVdtElements_2 = 0;  //number of valid elements in each row's last vdt
		static int iterPaddedRowWidth_2 = 0;  //padded row width for an iteration
		static int iterRowsElementsPadded_2 = 0;  //number of SURows elements, including padding
		static int iterVdtsPerRow_2 = 0;  //number of vdts per row including padding
		static int HBMBufferElemNumToWrite_2 = 0;
		static bool appendLastResults_2 = false;

		static int remRows_3 = 0;  //remaining rows
		static int lastVdtElements_3 = 0;  //number of valid elements in each row's last vdt
		static int iterPaddedRowWidth_3 = 0;  //padded row width for an iteration
		static int iterRowsElementsPadded_3 = 0;  //number of SURows elements, including padding
		static int iterVdtsPerRow_3 = 0;  //number of vdts per row including padding
		static int HBMBufferElemNumToWrite_3 = 0;
		static bool appendLastResults_3 = false;

		static int remRows_4 = 0;  //remaining rows
		static int lastVdtElements_4 = 0;  //number of valid elements in each row's last vdt
		static int iterPaddedRowWidth_4 = 0;  //padded row width for an iteration
		static int iterRowsElementsPadded_4 = 0;  //number of SURows elements, including padding
		static int iterVdtsPerRow_4 = 0;  //number of vdts per row including padding
		static int HBMBufferElemNumToWrite_4 = 0;
		static bool appendLastResults_4 = false;

		static bool kernelLastRow = false;  //designates if the last row is about to be processed
		static bool kernelLast2Rows = false;  //designates if the last 2 rows are about to be processed
		static bool kernelLast3Rows = false;  //designates if the last 3 rows are about to be processed
		static bool kernelLast4Rows = false;  //designates if the last 4 rows are about to be processed

		static bool kernelNotLastRound = true;
		static bool switchPipeline = false;

		static int SLRows0OutStartIdx = 0;
		static int SURows0OutStartIdx = 0;


register_and_bram_initialization:
		for (int i=0;i<LU_BRAM_DEPTH;i++) {
		#pragma HLS pipeline II=1
			for (int j=0;j<VDATA_SIZE;j++) {
			#pragma HLS unroll
				mybram[i].data[j] = 0.0f; mybram_2[i].data[j] = 0.0f; mybram_3[i].data[j] = 0.0f; mybram_4[i].data[j] = 0.0f;
			}
		}

		kernelLastRow = false; appendLastResults = false;
		c = 0; lastVdtElements = 0; remRows = 0; iterVdtsPerRow = 0; iterPaddedRowWidth = 0; SLRows0OutStartIdx = 0; SURows0OutStartIdx = 0;
		HBMBufferElemNumToWrite = 0;
		switchPipeline = false;

data_processing:
		for (c=0;c<N-1;c+=4) {
			HBMBufferElemNumToWrite = 0; HBMBufferElemNumToWrite_2 = 0; HBMBufferElemNumToWrite_3 = 0; HBMBufferElemNumToWrite_4 = 0;
			lastVdtElements = 0; lastVdtElements_2 = 0; lastVdtElements_3 = 0; lastVdtElements_4 = 0;
			iterRowsElementsPadded = 0; iterRowsElementsPadded_2 = 0; iterRowsElementsPadded_3 = 0; iterRowsElementsPadded_4 = 0;
			inputRowIdx = 0;
			kernelLastRow = (c==N-2); kernelLast2Rows = (c==N-3); kernelLast3Rows = (c==N-4); kernelLast4Rows = (c==N-5);
			kernelNotLastRound = (!kernelLastRow) && (!kernelLast2Rows) && (!kernelLast3Rows) && (!kernelLast4Rows);

			//keeps how many rows still remain to be processed, INCLUDING the pivot row
			remRows = N-c; remRows_2 = N-c-1; remRows_3 = N-c-2; remRows_4 = N-c-3;

#ifndef __SYNTHESIS__
			printf("START ======> (c=%d): kernelNotLastRound = %d, kernelLastRow=%d, kernelLast2Rows=%d, kernelLast3Rows=%d, kernelLast4Rows=%d\n",c,kernelNotLastRound,kernelLastRow,kernelLast2Rows,kernelLast3Rows,kernelLast4Rows);
			printf("START ======> (c=%d): remRows=%d, remRows_2=%d, remRows_3=%d, remRows_4=%d\n",c,remRows,remRows_2,remRows_3,remRows_4);
#endif

			dfUpdateRowBoundaries(
				remRows,remRows_2,remRows_3,remRows_4,
				&lastVdtElements,&lastVdtElements_2,&lastVdtElements_3,&lastVdtElements_4,
				&iterPaddedRowWidth,&iterPaddedRowWidth_2,&iterPaddedRowWidth_3,&iterPaddedRowWidth_4,
				&iterRowsElementsPadded,&iterRowsElementsPadded_2,&iterRowsElementsPadded_3,&iterRowsElementsPadded_4,
				&iterVdtsPerRow,&iterVdtsPerRow_2,&iterVdtsPerRow_3,&iterVdtsPerRow_4,
				&HBMBufferElemNumToWrite,&HBMBufferElemNumToWrite_2,&HBMBufferElemNumToWrite_3,&HBMBufferElemNumToWrite_4,
				&appendLastResults,&appendLastResults_2,&appendLastResults_3,&appendLastResults_4
			);

			if (!switchPipeline) {
				switchPipeline = true;

				//cache pivot row
#ifndef __SYNTHESIS__
				printf("1. (c=%d): READ BUFFER (%d elements) SURowsHBMBufferRd[%d] ==> cache 1..\n",c,iterPaddedRowWidth,inputRowIdx);
#endif
				read_vector_wide_in_float_bram2d_lu_12(SURowsHBMBuffer,mybram,iterPaddedRowWidth,inputRowIdx*iterVdtsPerRow);
				inputRowIdx+=1;

				dfLine0(
					&inputRowIdx,c,&SURows0OutStartIdx,
					iterPaddedRowWidth,iterVdtsPerRow,HBMBufferElemNumToWrite,
					SURowsHBMBuffer,mybram,SURows0Out,mybram_2,
					kernelNotLastRound,
					appendLastResults,
					LRowsStream
				);
				inputRowIdx+=1;
				SURows0OutStartIdx+=HBMBufferElemNumToWrite;

				if (kernelLastRow) {  //at this point processing is done
#ifndef __SYNTHESIS__
					printf("(c=%d): kernelLastRow = %d, done! :)\n",c,kernelLastRow);
#endif
					lu_update_L(c,SLRows0Out,LRowsStream,&SLRows0OutStartIdx,N-c-1);
					continue;
				}

				dfLine1(
					&inputRowIdx,c,&SURows0OutStartIdx,
					iterPaddedRowWidth,iterVdtsPerRow,
					iterVdtsPerRow_2,HBMBufferElemNumToWrite_2,
					SURowsHBMBuffer,mybram,SURows0Out,mybram_2,mybram_3,
					kernelNotLastRound,
					appendLastResults,appendLastResults_2,
					LRowsStream,LRowsStream_2
				);

				inputRowIdx+=1;
				SURows0OutStartIdx+=HBMBufferElemNumToWrite_2;

				if (kernelLast2Rows) {  //at this point processing is done
#ifndef __SYNTHESIS__
					printf("(c=%d): kernelLast2Rows = %d, done! :)\n",c,kernelLast2Rows);
#endif
					lu_update_L(c,SLRows0Out,LRowsStream,&SLRows0OutStartIdx,N-c-1);
					lu_update_L(c+1,SLRows0Out,LRowsStream_2,&SLRows0OutStartIdx,N-c-2);
					continue;
				}

				dfLine2(
					&inputRowIdx,c,&SURows0OutStartIdx,
					iterPaddedRowWidth,iterVdtsPerRow,
					iterVdtsPerRow_2,
					iterVdtsPerRow_3,HBMBufferElemNumToWrite_3,
					SURowsHBMBuffer,mybram,SURows0Out,mybram_2,mybram_3,mybram_4,
					kernelNotLastRound,
					appendLastResults,appendLastResults_2,appendLastResults_3,
					LRowsStream,LRowsStream_2,LRowsStream_3
				);

				inputRowIdx+=1;
				SURows0OutStartIdx+=HBMBufferElemNumToWrite_3;

				if (kernelLast3Rows) {  //at this point processing is done
#ifndef __SYNTHESIS__
					printf("(c=%d): kernelLast3Rows = %d, done! :)\n",c,kernelLast3Rows);
#endif
					lu_update_L(c,SLRows0Out,LRowsStream,&SLRows0OutStartIdx,N-c-1);
					lu_update_L(c+1,SLRows0Out,LRowsStream_2,&SLRows0OutStartIdx,N-c-2);
					lu_update_L(c+2,SLRows0Out,LRowsStream_3,&SLRows0OutStartIdx,N-c-3);
					continue;
				}

				dfLine3(
					&inputRowIdx,c,&SURows0OutStartIdx,
					iterPaddedRowWidth,iterVdtsPerRow,
					iterVdtsPerRow_2,
					iterVdtsPerRow_3,
					iterVdtsPerRow_4,HBMBufferElemNumToWrite_4,
					SURowsHBMBuffer,SURowsHBMBuffer2,mybram,SURows0Out,mybram_2,mybram_3,mybram_4,
					kernelNotLastRound,
					appendLastResults,appendLastResults_2,appendLastResults_3,appendLastResults_4,
					LRowsStream,LRowsStream_2,LRowsStream_3,LRowsStream_4
				);

				inputRowIdx+=1;
				SURows0OutStartIdx+=HBMBufferElemNumToWrite_4;

				if (kernelLast4Rows) {  //at this point processing is done
#ifndef __SYNTHESIS__
					printf("(c=%d): kernelLast4Rows = %d, done! :)\n",c,kernelLast4Rows);
#endif
					lu_update_L(c,SLRows0Out,LRowsStream,&SLRows0OutStartIdx,N-c-1);
					lu_update_L(c+1,SLRows0Out,LRowsStream_2,&SLRows0OutStartIdx,N-c-2);
					lu_update_L(c+2,SLRows0Out,LRowsStream_3,&SLRows0OutStartIdx,N-c-3);
					lu_update_L(c+3,SLRows0Out,LRowsStream_4,&SLRows0OutStartIdx,N-c-4);
					continue;
				}

				remRows-=5;  //subtract 5 rows that have already been processed

				int remRowsDf = remRows;
#ifndef __SYNTHESIS__
				printf("DATAFLOW STARTS (c=%d): process remaining rows, with i=0 to i<%d, i=i+1\n",c,remRowsDf);
#endif
				//IMPORTANT (do not delete): when using the remRows as upper boundary in the for-loop, dataflow failed with message "conditional execution of tasks is not supported".
				//By assigning remRows to a new int remRowsDf, and use it as boundary in the for-loop, it worked.
				//process remaining rows
				for (int i=0;i<remRowsDf;i++) {
					dfLoop(
						SURowsHBMBuffer,SURowsHBMBuffer2,
						i,inputRowIdx,c,iterVdtsPerRow,iterPaddedRowWidth,
						iterVdtsPerRow_2,iterPaddedRowWidth_2,
						iterVdtsPerRow_3,iterPaddedRowWidth_3,
						iterVdtsPerRow_4,HBMBufferElemNumToWrite_4,iterPaddedRowWidth_4,
						appendLastResults,appendLastResults_2,appendLastResults_3,appendLastResults_4,
						LRowsStream,LRowsStream_2,LRowsStream_3,LRowsStream_4,
						mybram,mybram_2,mybram_3,mybram_4
					);
				}
				lu_update_L(c,SLRows0Out,LRowsStream,&SLRows0OutStartIdx,N-c-1);
				lu_update_L(c+1,SLRows0Out,LRowsStream_2,&SLRows0OutStartIdx,N-c-2);
				lu_update_L(c+2,SLRows0Out,LRowsStream_3,&SLRows0OutStartIdx,N-c-3);
				lu_update_L(c+3,SLRows0Out,LRowsStream_4,&SLRows0OutStartIdx,N-c-4);
			}
			else {  //switchPipeline = true
				switchPipeline = false;

				//cache pivot row
#ifndef __SYNTHESIS__
				printf("1. (c=%d): READ BUFFER (%d elements) SURowsHBMBufferRd2[%d] ==> cache 1..\n",c,iterPaddedRowWidth,inputRowIdx);
#endif
				read_vector_wide_in_float_bram2d_lu_12(SURowsHBMBuffer2,mybram,iterPaddedRowWidth,inputRowIdx*iterVdtsPerRow);
				inputRowIdx+=1;

				dfLine0(
					&inputRowIdx,c,&SURows0OutStartIdx,
					iterPaddedRowWidth,iterVdtsPerRow,HBMBufferElemNumToWrite,
					SURowsHBMBuffer2,mybram,SURows0Out,mybram_2,
					kernelNotLastRound,
					appendLastResults,
					LRowsStream_4
				);
				inputRowIdx+=1;
				SURows0OutStartIdx+=HBMBufferElemNumToWrite;

				if (kernelLastRow) {  //at this point processing is done
#ifndef __SYNTHESIS__
					printf("(c=%d): kernelLastRow = %d, done! :)\n",c,kernelLastRow);
#endif
					lu_update_L(c,SLRows0Out,LRowsStream_4,&SLRows0OutStartIdx,N-c-1);
					continue;
				}

				dfLine1(
					&inputRowIdx,c,&SURows0OutStartIdx,
					iterPaddedRowWidth,iterVdtsPerRow,
					iterVdtsPerRow_2,HBMBufferElemNumToWrite_2,
					SURowsHBMBuffer2,mybram,SURows0Out,mybram_2,mybram_3,
					kernelNotLastRound,
					appendLastResults,appendLastResults_2,
					LRowsStream_4,LRowsStream
				);

				inputRowIdx+=1;
				SURows0OutStartIdx+=HBMBufferElemNumToWrite_2;

				if (kernelLast2Rows) {  //at this point processing is done
#ifndef __SYNTHESIS__
					printf("(c=%d): kernelLast2Rows = %d, done! :)\n",c,kernelLast2Rows);
#endif
					lu_update_L(c,SLRows0Out,LRowsStream_4,&SLRows0OutStartIdx,N-c-1);
					lu_update_L(c+1,SLRows0Out,LRowsStream,&SLRows0OutStartIdx,N-c-2);
					continue;
				}

				dfLine2(
					&inputRowIdx,c,&SURows0OutStartIdx,
					iterPaddedRowWidth,iterVdtsPerRow,
					iterVdtsPerRow_2,
					iterVdtsPerRow_3,HBMBufferElemNumToWrite_3,
					SURowsHBMBuffer2,mybram,SURows0Out,mybram_2,mybram_3,mybram_4,
					kernelNotLastRound,
					appendLastResults,appendLastResults_2,appendLastResults_3,
					LRowsStream_4,LRowsStream,LRowsStream_2
				);

				inputRowIdx+=1;
				SURows0OutStartIdx+=HBMBufferElemNumToWrite_3;

				if (kernelLast3Rows) {  //at this point processing is done
#ifndef __SYNTHESIS__
					printf("(c=%d): kernelLast3Rows = %d, done! :)\n",c,kernelLast3Rows);
#endif
					lu_update_L(c,SLRows0Out,LRowsStream_4,&SLRows0OutStartIdx,N-c-1);
					lu_update_L(c+1,SLRows0Out,LRowsStream,&SLRows0OutStartIdx,N-c-2);
					lu_update_L(c+2,SLRows0Out,LRowsStream_2,&SLRows0OutStartIdx,N-c-3);
					continue;
				}

				dfLine3(
					&inputRowIdx,c,&SURows0OutStartIdx,
					iterPaddedRowWidth,iterVdtsPerRow,
					iterVdtsPerRow_2,
					iterVdtsPerRow_3,
					iterVdtsPerRow_4,HBMBufferElemNumToWrite_4,
					SURowsHBMBuffer2,SURowsHBMBuffer,mybram,SURows0Out,mybram_2,mybram_3,mybram_4,
					kernelNotLastRound,
					appendLastResults,appendLastResults_2,appendLastResults_3,appendLastResults_4,
					LRowsStream_4,LRowsStream,LRowsStream_2,LRowsStream_3
				);

				inputRowIdx+=1;
				SURows0OutStartIdx+=HBMBufferElemNumToWrite_4;

				if (kernelLast4Rows) {  //at this point processing is done
#ifndef __SYNTHESIS__
					printf("(c=%d): kernelLast4Rows = %d, done! :)\n",c,kernelLast4Rows);
#endif
					lu_update_L(c,SLRows0Out,LRowsStream_4,&SLRows0OutStartIdx,N-c-1);
					lu_update_L(c+1,SLRows0Out,LRowsStream,&SLRows0OutStartIdx,N-c-2);
					lu_update_L(c+2,SLRows0Out,LRowsStream_2,&SLRows0OutStartIdx,N-c-3);
					lu_update_L(c+3,SLRows0Out,LRowsStream_3,&SLRows0OutStartIdx,N-c-4);
					continue;
				}

				remRows-=5;  //subtract 5 rows that have already been processed

				int remRowsDf = remRows;
#ifndef __SYNTHESIS__
				printf("DATAFLOW STARTS (c=%d): process remaining rows, with i=0 to i<%d, i=i+1\n",c,remRowsDf);
#endif
				//IMPORTANT (do not delete): when using the remRows as upper boundary in the for-loop, dataflow failed with message "conditional execution of tasks is not supported".
				//By assigning remRows to a new int remRowsDf, and use it as boundary in the for-loop, it worked.
				//process remaining rows
				for (int i=0;i<remRowsDf;i++) {
					dfLoop(
						SURowsHBMBuffer2,SURowsHBMBuffer,
						i,inputRowIdx,c,iterVdtsPerRow,iterPaddedRowWidth,
						iterVdtsPerRow_2,iterPaddedRowWidth_2,
						iterVdtsPerRow_3,iterPaddedRowWidth_3,
						iterVdtsPerRow_4,HBMBufferElemNumToWrite_4,iterPaddedRowWidth_4,
						appendLastResults,appendLastResults_2,appendLastResults_3,appendLastResults_4,
						LRowsStream_4,LRowsStream,LRowsStream_2,LRowsStream_3,
						mybram,mybram_2,mybram_3,mybram_4
					);
				}
				lu_update_L(c,SLRows0Out,LRowsStream_4,&SLRows0OutStartIdx,N-c-1);
				lu_update_L(c+1,SLRows0Out,LRowsStream,&SLRows0OutStartIdx,N-c-2);
				lu_update_L(c+2,SLRows0Out,LRowsStream_2,&SLRows0OutStartIdx,N-c-3);
				lu_update_L(c+3,SLRows0Out,LRowsStream_3,&SLRows0OutStartIdx,N-c-4);
			}
		}
	}
}
