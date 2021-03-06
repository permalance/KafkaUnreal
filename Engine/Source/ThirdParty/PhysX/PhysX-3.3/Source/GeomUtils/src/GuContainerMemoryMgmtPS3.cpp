/*
 * Copyright (c) 2008-2015, NVIDIA CORPORATION.  All rights reserved.
 *
 * NVIDIA CORPORATION and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA CORPORATION is strictly prohibited.
 */
// Copyright (c) 2004-2008 AGEIA Technologies, Inc. All rights reserved.
// Copyright (c) 2001-2004 NovodeX AG. All rights reserved.  

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "GuContainer.h"

#ifdef PX_PS3

using namespace physx;
using namespace Gu;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Initializes the container so that it uses an external memory buffer. The container doesn't own the memory, resizing is disabled.
 *	\param		max_entries		[in] max number of entries in the container
 *	\param		entries			[in] external memory buffer
 *	\param		allow_resize	[in] false to prevent resizing the array (forced fixed length), true to enable it. In that last case the
 *									initial memory is NOT released, it's up to the caller. That way the initial memory can be on the stack.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Container::InitSharedBuffers(PxU32 max_entries, PxU32* entries, bool allow_resize)
{
	PX_UNUSED(allow_resize);
	mCurNbEntries	= 0;
	mMaxNbEntries	= max_entries;
	mEntries		= entries;
	mGrowthFactor	= -1.0f;	// Negative growth ==> resize is disabled
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Resizes the container.
 *	\param		needed	[in] assume the container can be added at least "needed" values
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Container::Resize(PxU32 needed)
{
	PX_UNUSED(needed);
	PX_ASSERT(mCurNbEntries + needed < mMaxNbEntries);
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Clears the container. All stored values are deleted, and it frees used ram.
 *	\see		Reset()
 *	\return		Self-Reference
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Container& Container::Empty()
{
	mCurNbEntries = mMaxNbEntries = 0;
	return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Sets the initial size of the container. If it already contains something, it's discarded.
 *	\param		nb		[in] Number of entries
 *	\return		true if success
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Container::SetSize(PxU32 nb)
{
	PX_UNUSED(nb);
	PX_ASSERT(false);
	return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Refits the container and get rid of unused bytes.
 *	\return		true if success
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Container::Refit()
{
	// Get just enough entries
	mMaxNbEntries = mCurNbEntries;
	return mMaxNbEntries;
}

#endif
