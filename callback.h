/*****************************************************

	looper advance
	(c) chris mccormick, 2004
	
	licensed under the terms of the GPL
	see the file gpl.txt for details
	
	chris@mccormick.cx
	http://looper.mccormick.cx/
	
	$Id$

******************************************************/

#ifndef _CALLBACK_H_
#define _CALLBACK_H_

// this is the class for a specific callback
class cCallback
{
public:
	virtual void *Execute(void *param) const =0;
};

// this is the template for a specific callback
template <class cInstance>
class TCallback : public cCallback
{
public:
	TCallback()    // constructor
	{
		pFunction = NULL;
	}
	
	// param points to the data
	// returns a pointer to the resulting data
	// callback should know what types it's handing in, and what types it's getting back
	typedef void *(cInstance::*tFunction)(void *param);
	
	virtual void *Execute(void *param) const 
	{
		void *data=NULL;
		
		if (pFunction) data = (cInst->*pFunction)(param);
		else debug("The callback function has not been defined.");
			
		return data;
	}
	
	void MakeCallback(cInstance *cInstancePointer, tFunction pFunctionPointer)
	{
		cInst     = cInstancePointer;
		pFunction = pFunctionPointer;
	}

private:
	cInstance  *cInst;
	tFunction  pFunction;
};

#endif // _CALLBACK_H_
