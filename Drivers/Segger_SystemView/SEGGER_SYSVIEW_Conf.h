/*********************************************************************
*               SEGGER MICROCONTROLLER GmbH & Co. KG                 *
*       Solutions for real time microcontroller applications         *
**********************************************************************
*                                                                    *
*       (c) 2015  SEGGER Microcontroller GmbH & Co. KG               *
*                                                                    *
*       www.segger.com     Support: support@segger.com               *
*                                                                    *
**********************************************************************
*                                                                    *
*       SEGGER SystemView * Real-time application analysis           *
*                                                                    *
**********************************************************************
*                                                                    *
* All rights reserved.                                               *
*                                                                    *
* * This software may in its unmodified form be freely redistributed *
*   in source form.                                                  *
* * The source code may be modified, provided the source code        *
*   retains the above copyright notice, this list of conditions and  *
*   the following disclaimer.                                        *
* * Modified versions of this software in source or linkable form    *
*   may not be distributed without prior consent of SEGGER.          *
*                                                                    *
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS "AS IS" AND     *
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,  *
* THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A        *
* PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL               *
* SEGGER Microcontroller BE LIABLE FOR ANY DIRECT, INDIRECT,         *
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES           *
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS    *
* OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS            *
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,       *
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING          *
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS *
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.       *
*                                                                    *
**********************************************************************
*                                                                    *
*       SystemView version: V2.12                                    *
*                                                                    *
**********************************************************************
----------------------------------------------------------------------
File        : SEGGER_SYSVIEW_Conf.h
Purpose     : SEGGER SysView configuration.
--------  END-OF-HEADER  ---------------------------------------------
*/

#ifndef SEGGER_SYSVIEW_CONF_H
#define SEGGER_SYSVIEW_CONF_H

#ifdef __ICCARM__
  #include <intrinsics.h>
#endif

#include "SEGGER_RTT_Conf.h"

/*********************************************************************
*
*       Defines, configurable
*
**********************************************************************
*/
/*********************************************************************
*
*       SysView buffer configuration
*/
// Number of bytes that SysView uses for a buffer.
#define SEGGER_SYSVIEW_RTT_BUFFER_SIZE    %RttBufferSize

// The RTT channel that SysView will use.
#define SEGGER_SYSVIEW_RTT_CHANNEL        %RttChannelIndex
#if SEGGER_SYSVIEW_RTT_CHANNEL>=SEGGER_RTT_MAX_NUM_UP_BUFFERS
  #error "Not enough RTT buffers allocated in SEGGER_RTT_Conf.h!"
#endif


/*********************************************************************
*
*       SysView timestamp configuration
*/
#if SEGGER_RTT_CORE_M4
  #define SEGGER_SYSVIEW_GET_TIMESTAMP()      ((*(uint32_t *)(0xE0001004)) >> 4)       // Retrieve a system timestamp. Cortex-M cycle counter. Shifted by 4 to save bandwith.
  #define SEGGER_SYSVIEW_TIMESTAMP_BITS       28                                  // Define number of valid bits low-order delivered by clock source
#elif SEGGER_RTT_CORE_M0
  extern uint32_t SEGGER_uxGetTickCounterValue(void);
  #define SEGGER_SYSVIEW_GET_TIMESTAMP()      SEGGER_uxGetTickCounterValue()
  #define SEGGER_SYSVIEW_TIMESTAMP_BITS       24
  #warning "experimental only for Cortex-M0+!"
#else
  #error "Unknown ARM core!"
#endif

/*********************************************************************
*
*       SysView Id configuration
*/
#define SEGGER_SYSVIEW_ID_BASE         (0x%#l%SysViewIDBase)                          // Default value for the lowest Id reported by the application. Can be overridden by the application via SEGGER_SYSVIEW_SetRAMBase(). (i.e. 0x20000000 when all Ids are an address in this RAM)
#define SEGGER_SYSVIEW_ID_SHIFT        (%SysViewIDShift)                                   // Number of bits to shift the Id to save bandwidth. (i.e. 2 when Ids are 4 byte aligned)

/*********************************************************************
*
*       SysView interrupt configuration
*/
#if SEGGER_RTT_CORE_M4
  #define SEGGER_SYSVIEW_GET_INTERRUPT_ID()   ((*(U32 *)(0xE000ED04)) & 0x1FF)    // Get the currently active interrupt Id. (i.e. read Cortex-M ICSR[8:0] = active vector)
#elif SEGGER_RTT_CORE_M0
  #define SEGGER_SYSVIEW_GET_INTERRUPT_ID()   ((*(U32 *)(0xE000ED04)) & 0x3F)    // Get the currently active interrupt Id. (i.e. read Cortex-M ICSR[8:0] = active vector)
#else
  #error "Unknown ARM core!"
#endif

%- %if defined(LockUnlockEnabled) & %LockUnlockEnabled='yes' & defined(CriticalSection)
%- #include "%@CriticalSection@'ModuleName'.h"
%- #define SEGGER_SYSVIEW_LOCK()     %@CriticalSection@'ModuleName'%.CriticalVariable(); %@CriticalSection@'ModuleName'%.EnterCritical()
%- #define SEGGER_SYSVIEW_UNLOCK()   %@CriticalSection@'ModuleName'%.ExitCritical()
%- %endif

#endif

/*************************** End of file ****************************/