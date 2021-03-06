/*
  AWind.h - Arduino window library support for Color TFT LCD Boards
  Copyright (C)2015 Andrei Degtiarev. All right reserved
  

  You can always find the latest version of the library at 
  https://github.com/AndreiDegtiarev/AWind


  This library is free software; you can redistribute it and/or
  modify it under the terms of the CC BY-NC-SA 3.0 license.
  Please see the included documents for further information.

  Commercial use of this library requires you to buy a license that
  will allow commercial use. This includes using the library,
  modified or not, as a tool to sell products.

  The license applies to all part of the library including the 
  examples and tools supplied with the library.
*/
#pragma once

#include <math.h>
#include "IDataBuffer.h"

///Buffer for sensor data. It is used by chart control.
class TimeSerieBuffer : public IDataBuffer
{
	int _reserved_size;
	int _size;
	int   *_data_y;
	float _time_step;
	float _factor_y;
public:
	///Constructor
	/**
	\param time_step time interval between samples in buffer 
	\param factor_y scale factor for measurements. Data can be stored as volts measured directly on board pins and this factor transform them into phisical units
	\param reserved_size expected buffer size. This value allows buffer allocation only once. This is highly recomended because memory allocation is expencive operation. Mutiple allocation leads also to memory fragmentation.
	\param size actual buffer size
	*/
	TimeSerieBuffer(float time_step,float factor_y,int reserved_size,int size)
	{
		initialize(time_step,factor_y,reserved_size,size);
	}
	///Constructor
	/**
	\param time_step time interval between samples in buffer 
	\param factor_y scale factor for measurements. Data can be stored as volts measured directly on board pins and this factor transform them into phisical units
	\param size actual buffer size. reserved_size=size
	*/
	TimeSerieBuffer(float time_step,float factor_y,int size)
	{
		initialize(time_step,factor_y,size,size);

	}
protected:
	///Initialize internal buffer
	void initialize(float time_step,float factor_y,int reserved_size,int size)
	{
		_factor_y=factor_y;
		_size=size;
		_reserved_size=reserved_size;
		_time_step=time_step;
		_data_y=new int[reserved_size];
		for(int i=0;i<_size;i++)
			_data_y[i]=0;
	}
public:
	///Sets actual buffer size. It has to be less or equal to reserved_size  
	bool SetSize(int size)
	{
		if(size<=_reserved_size)
			_size=size;
		else
		{
			out<<F("Error: buffer size is too big")<<endln;
			return false;
		}
		return true;
	}
	///Sets time interval between samples in buffer 
	float SetTimeStep(float time_step)
	{
		_time_step=time_step;
	}
	///Sets scale factor for mesaurements
	float SetFactorY(float factor_y)
	{
		_factor_y=factor_y;
	}
	///Puts value into buffer
	/**
	\param index index in buffer. It has to be less than size
	\param value value to save in the buffer
	*/
	void Set(unsigned int index,float value)
	{
		if(index>=Size())
		{
			out<<F("Error: index outside of array bounds: ")<<index<<endln;
			return;
		}
		_data_y[index]=(value*_factor_y);
	}
	///Returns point to vaues array. It is used for direct access to internal buffer
	int *Y()
	{
		return _data_y;
	}
	///Returns min and max value for buffer data
	void MinMax(float &min_x,float &max_x,float &min_y,float &max_y)
	{
 		min_x=0;
		max_x=_time_step*_size;
 		min_y=_data_y[_size-1];
		max_y=_data_y[_size-1];
		for(int i=0;i<_size;i++)
		{
			min_y=min(min_y,_data_y[i]);
			max_y=max(max_y,_data_y[i]);
		}
		min_y/=_factor_y;
		max_y/=_factor_y;
	}
	///Returns X that is calculated from index in buffer (see time_step parameter)
	float X(unsigned int index)
	{
		if(index>=Size())
		{
			out<<F("Error: index outside of array bounds: ")<<index<<endln;
			return 0;
		}
		return _time_step*index;
	}
	///Returns  scaled value Y from buffer for specified index
	float Y(unsigned int index)
	{
		if(index>=Size())
		{
			out<<F("Error: index outside of array bounds: ")<<index<<endln;
			return 0;
		}
		return _data_y[index]/_factor_y;
	}
	///Returns start index in buffer. For this class is always 0
	unsigned int StartIndex()
	{
		return 0;
	}
	///Returns buffer size
	unsigned int Size()
	{
		return _size;
	}
};