#pragma once

#ifndef __CONVERTFUNCTIONS__
#define __CONVERTFUNCTIONS__

	#define PARAM_COUNT 35

	enum TDataTypes { tfloat, tDouble, tpChar, tpTimestamp, tUint16, tInt16, tUint32};

	typedef struct
	{
		int		datatype;
		int		precision;
		char	*unit;
	}TConvertInfo;


	void SetCWndPosition(CWnd *Control, int x, int y, int width, int height);
	void FormatDoubleToString(const double Value, int Precision, char * unit, char *Output);
	void FormatIntToString(const int Value, char * unit, char *Output);
	void WriteParameterToEdit(CWnd *Ctrl, char *Input);
	void FreeAndNull(void *Buffer);

#endif //__CONVERTFUNCTIONS__
