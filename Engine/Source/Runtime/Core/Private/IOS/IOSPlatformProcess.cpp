// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

/*=============================================================================
IOSPlatformProcess.cpp: iOS implementations of Process functions
=============================================================================*/

#include "CorePrivatePCH.h"
#include "ApplePlatformRunnableThread.h"
#include "IOSAppDelegate.h"
#include <mach-o/dyld.h>

// numbers recommended by Apple
#define GAME_THREAD_PRIORITY 47
#define RENDER_THREAD_PRIORITY 45


const TCHAR* FIOSPlatformProcess::ComputerName()
{
	static TCHAR Result[256]=TEXT("");

	if( !Result[0] )
	{
		ANSICHAR AnsiResult[ARRAY_COUNT(Result)];
		gethostname(AnsiResult, ARRAY_COUNT(Result));
		FCString::Strcpy(Result, ANSI_TO_TCHAR(AnsiResult));
	}
	return Result;
}

const TCHAR* FIOSPlatformProcess::BaseDir()
{
	return TEXT("");
}

FRunnableThread* FIOSPlatformProcess::CreateRunnableThread()
{
	return new FRunnableThreadApple();
}

void FIOSPlatformProcess::LaunchURL( const TCHAR* URL, const TCHAR* Parms, FString* Error )
{
	UE_LOG(LogIOS, Log,  TEXT("LaunchURL %s %s"), URL, Parms?Parms:TEXT("") );
	CFStringRef CFUrl = FPlatformString::TCHARToCFString( URL );
	CFUrl = CFURLCreateStringByAddingPercentEscapes(NULL, CFUrl, CFSTR("#?+"), NULL, kCFStringEncodingUTF8);
	[[UIApplication sharedApplication] openURL: [NSURL URLWithString:( NSString *)CFUrl]];
	CFRelease( CFUrl );

	if (Error != nullptr)
	{
		*Error = TEXT("");
	}
}

void FIOSPlatformProcess::SetRealTimeMode()
{
	if ([IOSAppDelegate GetDelegate].OSVersion < 7 && FPlatformMisc::NumberOfCores() > 1)
	{
		mach_timebase_info_data_t TimeBaseInfo;
		mach_timebase_info( &TimeBaseInfo );
		double MsToAbs = ((double)TimeBaseInfo.denom / (double)TimeBaseInfo.numer) * 1000000.0;
		uint32 NormalProcessingTimeMs = 20;
		uint32 ConstraintProcessingTimeMs = 60;

		thread_time_constraint_policy_data_t Policy;
		Policy.period      = 0;
		Policy.computation = (uint32_t)(NormalProcessingTimeMs * MsToAbs);
		Policy.constraint  = (uint32_t)(ConstraintProcessingTimeMs * MsToAbs);
		Policy.preemptible = true;

		thread_policy_set(pthread_mach_thread_np(pthread_self()), THREAD_TIME_CONSTRAINT_POLICY, (thread_policy_t)&Policy, THREAD_TIME_CONSTRAINT_POLICY_COUNT);
	}
}

void FIOSPlatformProcess::SetupGameOrRenderThread(bool bIsRenderThread)
{
	// Set the gamethread priority to very high, slightly above the renderthread
	struct sched_param Sched;
	FMemory::Memzero(&Sched, sizeof(struct sched_param));

	// Read the current priority and policy
	int32 CurrentPolicy = SCHED_RR;
	pthread_getschedparam(pthread_self(), &CurrentPolicy, &Sched);

	// Set the new priority and policy (apple recommended FIFO for the two main non-working threads)
	int32 Policy = SCHED_FIFO;
	Sched.sched_priority = bIsRenderThread ? RENDER_THREAD_PRIORITY : GAME_THREAD_PRIORITY;
	pthread_setschedparam(pthread_self(), Policy, &Sched);
}


void FIOSPlatformProcess::SetThreadAffinityMask(uint64 AffinityMask)
{
	if ([IOSAppDelegate GetDelegate].OSVersion >= 8 && FPlatformMisc::NumberOfCores() > 1)
	{
		thread_affinity_policy AP;
		AP.affinity_tag = AffinityMask;
		thread_policy_set(pthread_mach_thread_np(pthread_self()), THREAD_AFFINITY_POLICY, (integer_t*)&AP, THREAD_AFFINITY_POLICY_COUNT);
	}
	else
	{
		FGenericPlatformProcess::SetThreadAffinityMask(AffinityMask);
	}
}

const TCHAR* FIOSPlatformProcess::ExecutableName(bool bRemoveExtension)
{
	static TCHAR Result[512] = TEXT("");
	if (!Result[0])
	{
		NSString *NSExeName = [[[NSBundle mainBundle] executablePath] lastPathComponent];
		FPlatformString::CFStringToTCHAR((CFStringRef)NSExeName, Result);
	}
	return Result;
}

const uint64 FIOSPlatformAffinity::GetMainGameMask()
{
	return MAKEAFFINITYMASK1(0);
}

const uint64 FIOSPlatformAffinity::GetRenderingThreadMask()
{
	static int Mask = 0;
	if (Mask == 0)
	{
		switch (FPlatformMisc::NumberOfCores())
		{
		case 2:
		case 3:
			Mask = MAKEAFFINITYMASK1(1);
			break;
		default:
			Mask = FGenericPlatformAffinity::GetRenderingThreadMask();
			break;
		}
	}

	return Mask;
}

const uint64 FIOSPlatformAffinity::GetRTHeartBeatMask()
{
	static int Mask = 0;
	if (Mask == 0)
	{
		switch (FPlatformMisc::NumberOfCores())
		{
		case 2:
		case 3:
			Mask = MAKEAFFINITYMASK1(0);
			break;
		default:
			Mask = FGenericPlatformAffinity::GetRTHeartBeatMask();
			break;
		}
	}

	return Mask;
}

const uint64 FIOSPlatformAffinity::GetPoolThreadMask()
{
	static int Mask = 0;
	if (Mask == 0)
	{
		switch (FPlatformMisc::NumberOfCores())
		{
		case 2:
			Mask = MAKEAFFINITYMASK1(1);
			break;
		case 3:
			Mask = MAKEAFFINITYMASK1(2);
			break;
		default:
			Mask = FGenericPlatformAffinity::GetPoolThreadMask();
			break;
		}
	}

	return Mask;
}

const uint64 FIOSPlatformAffinity::GetTaskGraphThreadMask()
{
	static int Mask = 0;
	if (Mask == 0)
	{
		switch (FPlatformMisc::NumberOfCores())
		{
		case 2:
			Mask = MAKEAFFINITYMASK1(1);
			break;
		case 3:
			Mask = MAKEAFFINITYMASK1(2);
			break;
		default:
			Mask = FGenericPlatformAffinity::GetTaskGraphThreadMask();
			break;
		}
	}

	return Mask;
}

const uint64 FIOSPlatformAffinity::GetStatsThreadMask()
{
	static int Mask = 0;
	if (Mask == 0)
	{
		switch (FPlatformMisc::NumberOfCores())
		{
		case 2:
			Mask = MAKEAFFINITYMASK1(0);
			break;
		case 3:
			Mask = MAKEAFFINITYMASK1(2);
			break;
		default:
			Mask = FGenericPlatformAffinity::GetStatsThreadMask();
			break;
		}
	}

	return Mask;
}

const uint64 FIOSPlatformAffinity::GetNoAffinityMask()
{
	static int Mask = 0;
	if (Mask == 0)
	{
		switch (FPlatformMisc::NumberOfCores())
		{
		case 2:
		case 3:
			Mask = (1 << FPlatformMisc::NumberOfCores()) - 1;
			break;
		default:
			Mask = FGenericPlatformAffinity::GetNoAffinityMask();
			break;
		}
	}

	return Mask;
}

