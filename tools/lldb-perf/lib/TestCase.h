//===-- TestCase.h ----------------------------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef __PerfTestDriver__TestCase__
#define __PerfTestDriver__TestCase__

#include "lldb/API/LLDB.h"
#include "Measurement.h"

namespace lldb_perf {

class Results;
    
class TestCase
{
public:
    TestCase();
    
    struct ActionWanted
	{
		enum class Type
		{
			eStepOver,
			eContinue,
            eStepOut,
			eKill
		} type;
		lldb::SBThread thread;
        
        ActionWanted () :
            type (Type::eContinue),
            thread ()
        {
        }
        
        void
        Continue()
        {
            type = Type::eContinue;
            thread = lldb::SBThread();
        }
        
        void
        StepOver (lldb::SBThread t)
        {
            type = Type::eStepOver;
            thread = t;
        }

        void
        StepOut (lldb::SBThread t)
        {
            type = Type::eStepOut;
            thread = t;
        }
        
        void
        Kill ()
        {
            type = Type::eKill;
            thread = lldb::SBThread();
        }
	};
    
    virtual
    ~TestCase ()
    {
    }
    
	virtual bool
	Setup (int argc, const char** argv);
    
	virtual void
	TestStep (int counter, ActionWanted &next_action) = 0;
	
	bool
	Launch (lldb::SBLaunchInfo &launch_info);
	
	void
	Loop();
    
    void
    SetVerbose (bool);
    
    bool
    GetVerbose ();
    
    virtual void
    WriteResults (Results &results) = 0;
    
    template <typename G,typename A>
    Measurement<G,A> CreateMeasurement (A a, const char* name = NULL, const char* description = NULL)
    {
        return Measurement<G,A> (a,name, description);
    }
    
    template <typename A>
    TimeMeasurement<A> CreateTimeMeasurement (A a, const char* name = NULL, const char* description = NULL)
    {
        return TimeMeasurement<A> (a,name, description);
    }
    
    template <typename A>
    MemoryMeasurement<A> CreateMemoryMeasurement (A a, const char* name = NULL, const char* description = NULL)
    {
        return MemoryMeasurement<A> (a,name, description);
    }
    
    static void
    Run (TestCase& test, int argc, const char** argv);
    
    lldb::SBDebugger &
    GetDebugger()
    {
        return m_debugger;
    }

    lldb::SBTarget &
    GetTarget()
    {
        return m_target;
    }
    
    lldb::SBProcess &
    GetProcess ()
    {
        return m_process;
    }
    
    lldb::SBThread &
    GetThread ()
    {
        return m_thread;
    }
    
    int
    GetStep ()
    {
        return m_step;
    }
    
protected:
    lldb::SBDebugger m_debugger;
	lldb::SBTarget m_target;
	lldb::SBProcess m_process;
	lldb::SBThread m_thread;
	lldb::SBListener m_listener;
    bool m_verbose;
    int m_step;
};
}

#endif /* defined(__PerfTestDriver__TestCase__) */