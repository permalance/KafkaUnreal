/*
 * Copyright (c) 2008-2015, NVIDIA CORPORATION.  All rights reserved.
 *
 * NVIDIA CORPORATION and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA CORPORATION is strictly prohibited.
 */

// Copyright (C) 2002-2008 AGEIA Technologies, Inc. All rights reserved.
// Copyright (C) 2001-2006 NovodeX. All rights reserved.

#ifndef __FRAMEWORK_PERF_SCOPE_H___
#define __FRAMEWORK_PERF_SCOPE_H___

#include "foundation/PxSimpleTypes.h"

#ifndef PHYSX_PROFILE_SDK
#define PX_DISABLE_USER_PROFILER_CALLBACK
#endif

#include "ProfilerCallback.h"

namespace physx
{
namespace apex
{
class ApexSDK;
};
};

namespace Framework
{
void initFrameworkProfiling(physx::apex::ApexSDK*);
void releaseFrameworkProfiling();
};


#endif
