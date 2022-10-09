/*****************************************************************************
FILE:  easytest.cpp
AUTHOR:	Patrick S. Hamilton
REVISED:	5/13/2002
  ___________________________________________________________________________

easytest.cpp: Use bdac to generate an annotation file.
Copywrite (C) 2001 Patrick S. Hamilton
Copywrite (C) 1999 George B. Moody

This file is free software; you can redistribute it and/or modify it under
the terms of the GNU Library General Public License as published by the Free
Software Foundation; either version 2 of the License, or (at your option) any
later version.

This software is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU Library General Public License for more
details.

You should have received a copy of the GNU Library General Public License along
with this library; if not, write to the Free Software Foundation, Inc., 59
Temple Place - Suite 330, Boston, MA 02111-1307, USA.

You may contact the author by e-mail (pat@eplimited.edu) or postal mail
(Patrick Hamilton, E.P. Limited, 35 Medford St., Suite 204 Somerville,
MA 02143 USA).  For updates to this software, please visit our website
(http://www.eplimited.com).
  __________________________________________________________________________

Easytest.exe is a simple program to help test the performance of our
beat detection and classification software. Data is read from the
indicated ECG file, the channel 1 signal is fed to bdac.c, and the
resulting detections are saved in the annotation file <record>.ate.
<record>.ate may then be compared to <record>.atr to using bxb to
analyze the performance of the the beat detector and classifier detector.

Note that data in the MIT/BIH Arrythmia database file has been sampled
at 360 samples-per-second, but the beat detection and classification
software has been written for data sampled at 200 samples-per-second.
Date is converterted from 360 sps to 200 sps with the function NextSample.
Code for resampling was copied from George Moody's xform utility.  The beat
locations are then adjusted back to coincide with the original sample
rate of 360 samples/second so that the annotation files generated by
easytest can be compared to the "atruth" annotation files.

This file must be linked with object files produced from:
	wfdb software library (source available at www.physionet.org)
	analbeat.cpp
	match.cpp
	rythmchk.cpp
	classify.cpp
	bdac.cpp
	qrsfilt.cpp
	qrsdet.cpp
  __________________________________________________________________________

  Revisions
	4/13/02:
		Added conditional define statements that allow MIT/BIH or AHA
			records to be processed.
		Normalize input to 5 mV/LSB (200 A-to-D units/mV).

*******************************************************************************/

#include	"wfdb.h"
#include "ecgcodes.h"
#include "ecgmap.h"
#include "stdio.h"
#include "qrsdet.h"		// For sample rate.

#define MITDB				// Comment this line out to process AHA data.
#ifdef MITDB
#define ECG_DB_PATH	"C:\\MITDB\\"	// Path to where MIT/BIH data.
#define REC_COUNT	48
int Records[REC_COUNT] = {100,101,102,103,104,105,106,107,108,109,111,112,
							113,114,115,116,117,118,119,121,122,123,124,
							200,201,202,203,205,207,208,209,210,212,213,214,
							215,217,219,220,221,222,223,228,230,231,232,233,234} ;

#else
#define ECG_DB_PATH	"C:\\AHADAT~1\\"	// Path to where AHA data.
#define REC_COUNT	69
int Records[REC_COUNT] = {1201,1202,1203,1204,1205,1206,1207,1208,1209,1210,
									2201,2203,2204,2205,2206,2207,2208,2209,2210,
									3201,3202,3203,3204,3205,3206,3207,3208,3209,3210,
									4201,4202,4203,4204,4205,4206,4207,4208,4209,4210,
									5201,5202,5203,5204,5205,5206,5207,5208,5209,5210,
									6201,6202,6203,6204,6205,6206,6207,6208,6209,6210,
									7201,7202,7203,7204,7205,7206,7207,7208,7209,7210} ;
#endif
// External function prototypes.
void ResetBDAC(void) ;
int BeatDetectAndClassify(int ecgSample, int *beatType, int *beatMatch) ;

// Local Prototypes.
int  NextSample(int *vout,int nosig,int ifreq,
						int ofreq,int init) ;
int gcd(int x, int y);

// Global variables.

int ADCZero, ADCUnit, InputFileSampleFrequency ;


#ifdef __STDC__
#define MAINTYPE int
#else
#define MAINTYPE void
#endif
MAINTYPE main()
	{
	char record[10], fname[20] ;
	int i, ecg[2], delay, recNum ;
	WFDB_Siginfo s[2] ;
	WFDB_Anninfo a[2] ;
	WFDB_Annotation annot ;

	unsigned char byte ;
	FILE *newAnn0, *newAnn1 ;
	long SampleCount = 0, lTemp, DetectionTime ;
	int beatType, beatMatch ;

	// Set up path to database directory

	setwfdb(ECG_DB_PATH) ;

	// Analyze all 48 MIT/BIH Records.

	for(recNum = 0; recNum < REC_COUNT; ++recNum)
		{
		sprintf(record,"%d",Records[recNum]) ;
		printf("Record %d\n",Records[recNum]) ;

		// Open a 2 channel record

		if(isigopen(record,s,2) < 1)
			{
			printf("Couldn't open %s\n",record) ;
			return ;
			}

		ADCZero = s[0].adczero ;
		ADCUnit = s[0].gain ;
		InputFileSampleFrequency = sampfreq(record) ;

		// Setup for output annotations

		a[0].name = "atest"; a[0].stat = WFDB_WRITE ;

		if(annopen(record, a, 1) < 0)
			return ;

		// Initialize sampling frequency adjustment.

		NextSample(ecg,2,InputFileSampleFrequency,SAMPLE_RATE,1) ;

		// Initialize beat detection and classification.

		ResetBDAC() ;
		SampleCount = 0 ;

		// Read data from MIT/BIH file until there is none left.

		while(NextSample(ecg,2,InputFileSampleFrequency,SAMPLE_RATE,0) >= 0)
			{
			++SampleCount ;

			// Set baseline to 0 and resolution to 5 mV/lsb (200 units/mV)

			lTemp = ecg[0]-ADCZero ;
			lTemp *= 200 ;
			lTemp /= ADCUnit ;
			ecg[0] = lTemp ;

			// Pass sample to beat detection and classification.

			delay = BeatDetectAndClassify(ecg[0], &beatType, &beatMatch) ;

			// If a beat was detected, annotate the beat location
			// and type.

			if(delay != 0)
				{
				DetectionTime = SampleCount - delay ;

				// Convert sample count to input file sample
				// rate.

				DetectionTime *= InputFileSampleFrequency ;
				DetectionTime /= SAMPLE_RATE ;
				annot.time = DetectionTime ;
				annot.anntyp = beatType ;
				annot.aux = NULL ;
				putann(0,&annot) ;
				}
			}

		// Reset database after record is done.

		wfdbquit() ;

		// Copy "atest.<record>" to "<record>.ate" for future ascess.
		// (This is necessary for PC files)

		sprintf(fname,"%s.ate",record) ;
		newAnn0 = fopen(fname,"rb") ;
		sprintf(fname,"%s%s.ate",ECG_DB_PATH,record) ;
		newAnn1 = fopen(fname,"wb") ;

		// Copy byte image of annotation file in this
		// directory to a correctly named file in the
		// database directory.

		while(fread(&byte,sizeof(char),1,newAnn0) == 1)
			fwrite(&byte,sizeof(char),1,newAnn1) ;

		fclose(newAnn0) ;
      fclose(newAnn1) ;
		}
	}

/**********************************************************************
	NextSample reads MIT/BIH Arrhythmia data from a file of data
	sampled at ifreq and returns data sampled at ofreq.  Data is
	returned in vout via *vout.  NextSample must be initialized by
	passing in a nonzero value in init.  NextSample returns -1 when
   there is no more data left.
***********************************************************************/

int  NextSample(int *vout,int nosig,int ifreq,
						int ofreq,int init)
	{
	int i ;
	static int m, n, mn, ot, it, vv[WFDB_MAXSIG], v[WFDB_MAXSIG], rval ;

	if(init)
		{
		i = gcd(ifreq, ofreq);
		m = ifreq/i;
		n = ofreq/i;
		mn = m*n;
		ot = it = 0 ;
		getvec(vv) ;
		rval = getvec(v) ;
		}

	else
		{
		while(ot > it)
			{
	    	for(i = 0; i < nosig; ++i)
	    		vv[i] = v[i] ;
			rval = getvec(v) ;
		    if (it > mn) { it -= mn; ot -= mn; }
		    it += n;
		    }		
	    for(i = 0; i < nosig; ++i)
	    	vout[i] = vv[i] + (ot%n)*(v[i]-vv[i])/n;
		ot += m;
		}

	return(rval) ;
	}

// Greatest common divisor of x and y (Euclid's algorithm)

int gcd(int x, int y)
	{
	while (x != y) {
		if (x > y) x-=y;
		else y -= x;
		}
	return (x);
	}


