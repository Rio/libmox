/*
 *  Types.h
 *  MoxMxf
 *
 *  Created by Brendan Bolles on 4/16/15.
 *  Copyright 2015 fnord. All rights reserved.
 *
 */


#ifndef MOXMXF_TYPES_H
#define MOXMXF_TYPES_H


//#include <mxflib/system.h>
//#include <mxflib/types.h>


namespace MoxMxf
{
/*
	typedef mxflib::UInt32		UInt32;
	typedef mxflib::UInt16		UInt16;
	typedef mxflib::UInt8		UInt8;
	
	typedef mxflib::Int32		Int32;
	typedef mxflib::Int16		Int16;
	typedef mxflib::Int8		Int8;
	
	typedef mxflib::Int64		Int64;
	typedef mxflib::UInt64		UInt64;
	
	typedef mxflib::Length		Length;
	typedef mxflib::Position	Position;
	
	typedef mxflib::Rational	Rational;
	
	
	typedef UInt32 FileHandle;
*/

	// copied from system.h in mxflib
	typedef unsigned int UInt32;	
	typedef unsigned short int UInt16;
	typedef unsigned char UInt8;	

	typedef int Int32;				
	typedef short int Int16;		
	typedef signed char Int8;		

#ifdef _MSC_VER
	typedef __int64 Int64;
	typedef unsigned __int64 UInt64;
#else
	typedef long long Int64;
	typedef unsigned long long UInt64;
#endif

	typedef Int64 Length;		// Lenth of an item in bytes
	typedef Int64 Position;		// Position within an MXF file

	typedef UInt32 FileHandle; // same as system.h in mxflib when MXFLIB_NO_FILE_IO
	
	
	typedef Int32 TrackNum;
	typedef UInt32 SID;
	
	
	class Rational
	{
	  public:
		Int32 Numerator;
		Int32 Denominator;
		
		Rational() : Numerator(0), Denominator(1) {};
		Rational(Int32 Num, Int32 Den = 1) : Numerator(Num), Denominator(Den) {};
		
		inline bool operator==(const Rational &RHS)
		{
			return (Numerator == RHS.Numerator) && (Denominator == RHS.Denominator);
		}
		
		inline bool operator!=(const Rational &RHS)
		{
			return !operator==(RHS);
		}
	};
}


#endif // MOXMXF_TYPES_H