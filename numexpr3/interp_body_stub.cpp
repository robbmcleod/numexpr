/*********************************************************************
  Numexpr - Fast numerical array expression evaluator for NumPy.

      License: BSD
      Author:  See AUTHORS.txt

  See LICENSE.txt for details about copyright and rights to use.
**********************************************************************/

{
    int pc;
    unsigned int J;

    // set up pointers to next block of inputs and outputs
#ifdef SINGLE_ITEM_CONST_LOOP
    //mem[0] = params->output;
#else // SINGLE_ITEM_CONST_LOOP
    // use the iterator's inner loop data
    // RAM: TODO, copy iterDataPtr into the params->registers[:].mem
    //memcpy(mem, iterDataPtr, (1+params->n_ndarray)*sizeof(npy_intp));
    // TODO: this is getting expensive to set mems for each block...
    unsigned int arrayCnt = 0;
    for( J = 0; J < params->n_reg; J++ ) {
        if( params->registers[J].kind == KIND_ARRAY ) {
            params->registers[J].mem = iterDataPtr[arrayCnt];
            params->registers[J].stride = iterStrides[arrayCnt];
            arrayCnt++;
        }
    }
#  ifndef NO_OUTPUT_BUFFERING
    // if output buffering is necessary, first write to the buffer
    // DISABLE OUTPUT BUFFERING
//    if(params->outBuffer != NULL) {
//        GET_RETURN_REG(params).mem = params->outBuffer;
//        //mem[0] = params->outBuffer;
//    }
#  endif // NO_OUTPUT_BUFFERING


#endif // SINGLE_ITEM_CONST_LOOP

    // WARNING: From now on, only do references to mem[arg[123]]
    // & memsteps[arg[123]] inside the VEC_ARG[123] macros,
    // or you will risk accessing invalid addresses.
    
    // For the strings, I think they are actual string objects?
    // https://github.com/numpy/numpy/blob/c90d7c94fd2077d0beca48fa89a423da2b0bb663/numpy/core/include/numpy/npy_3kcompat.h

    for (pc = 0; pc < params->program_len; pc++ ) {
        NE_WORD     op       = params->program[pc].op;
        NE_REGISTER store_in = params->program[pc].ret;


        // Sample debug output: 
        // printf( "Arg1(%d@%p) + ARG2(%d@%p) => DEST(%d@%p)\n", arg1, x1, arg2, x2, store_in, dest );
        
        // TODO: BLOCK_SIZE1 is the number of operations, not the array block size,
        // so the memory block scales with itemsize...
        //printf( "Exec op: %d\n", op );
        switch (op) {
        case 0: 
            break;
        //GENERATOR_INSERT_POINT

        default:
            *pc_error = pc;
            return -3;
            break;
        }
    }


#ifndef NO_OUTPUT_BUFFERING
    // If output buffering was necessary, copy the buffer to the output
    //printf( "TODO: output buffering disabled.\n" );
//    if(params->outBuffer != NULL) {
//        memcpy(iterDataPtr[0], params->outBuffer, GET_RETURN_REG(params).itemsize * BLOCK_SIZE);
//    }
#endif // NO_OUTPUT_BUFFERING

}

