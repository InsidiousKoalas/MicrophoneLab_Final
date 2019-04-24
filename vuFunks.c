/*
 * vuFunks.c
 *
 *  Created on: Apr 24, 2019
 *      Author: bhunt
 */


void setDispAry(int avg, int * disp){
	int vals[8] = {50,80,150,200,400,800,1300,2000};
	int i,ndx;

	// sort avg from vals, i is resultant index
	for(ndx=0; ndx<8; ndx++){
		if(avg<vals[ndx]){
			break;
		}
	}

	for(i=0; i<4; i++){
		if((i<ndx) && ((i+1)<ndx)){		// if avg is greater and i is even
			disp[i] = 0xB6;
		}
		else if((i<ndx) && ((i+1) > ndx)){
			disp[i] = 0x86;
		}
		else{
			disp[i] = 0x80;
		}

	}


//	for(i=0; i<8; i++){
//		if((avg>vals[i]) && (i%2 == 1))	{  // if avg is greater and the ndx is odd
//			disp[ndx] = 0xB0;
//		}
//		if((avg>vals[i]) && (i%2 == 0)){	 // if avg is greater and the ndx is even
//			disp[ndx] = 0xB6;
//		}
//		else{
//			disp[ndx] = 0x80;
//		}
//	}

//
//	for(i = 0;i<8;i++)
//	{
//		if(vals[i]>avg)
//		{
//			disp[ndx] = 0x80;
//		}
//	}
//	if(!((i%2) == 0))    //odd number
//	{
//		ndx = (i-1)/2;
//		disp[ndx] = 0x86;
//	} else
//	{
//		ndx = i/2;
//	}
//	for(i = 0;i<ndx;i++)
//	{
//		disp[i] = 0xB6;//setting all of the bars
//	}
}

