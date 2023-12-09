/********************************************************************************************************
 *  @Description                                                                                        *
 *  Functions that relate to the Memory Protection Unit.                                                *
 *  The following functions support the optional Memory Protection Unit (MPU) that is available         *
 *  on the Cortex-M23, M33, M35P processor. The MPU is used to prevent from illegal memory accesses     *
 *  that are typically caused by errors in an application software.                                     *
 *                                                                                                      *    
 *  Reference: https://www.keil.com/pack/doc/cmsis/Core/html/group__mpu8__functions.html                *
 ********************************************************************************************************/

#include "nrf.h"
#include "nrf_peripherals.h"
#include <stdbool.h>

/** Set the memory attribute encoding.
* \param idx The attribute index to be set [0-7]
* \param attr The attribute value to be set.
    ARM_MPU_SetMemAttr(uint8_t idx, uint8_t attr)
*/

/** Configure the given MPU region of the given MPU.
* \param mpu Pointer to MPU to be used.
* \param rnr Region number to be configured.
* \param rbar Value for RBAR register.
* \param rlar Value for RLAR register.
* __STATIC_INLINE void ARM_MPU_SetRegionEx(MPU_Type* mpu, uint32_t rnr, uint32_t rbar, uint32_t rlar)
*/ 

/** \brief Attribute for normal memory (outer and inner)
* \param NT Non-Transient: Set to 1 for non-transient data.
* \param WB Write-Back: Set to 1 to use write-back update policy.
* \param RA Read Allocation: Set to 1 to use cache allocation on read miss.
* \param WA Write Allocation: Set to 1 to use cache allocation on write miss.
*
* #define ARM_MPU_ATTR_MEMORY_(NT, WB, RA, WA) \
  ((((NT) & 1U) << 3U) | (((WB) & 1U) << 2U) | (((RA) & 1U) << 1U) | ((WA) & 1U))
*/

/** \brief Region Base Address Register value
* \param BASE The base address bits [31:5] of a memory region. The value is zero extended. Effective address gets 32 byte aligned.
* \param SH Defines the Shareability domain for this memory region.
* \param RO Read-Only: Set to 1 for a read-only memory region.
* \param NP Non-Privileged: Set to 1 for a non-privileged memory region.
* \oaram XN eXecute Never: Set to 1 for a non-executable memory region.
*
* #define ARM_MPU_RBAR(BASE, SH, RO, NP, XN)
*/

int main() 
{
    // Set Region 0 using Attr 0
    ARM_MPU_SetMemAttr (0UL, 
        ARM_MPU_ATTR (         /* Normal memory */
            ARM_MPU_ATTR_MEMORY_(0UL, 1UL, 1UL, 1UL), /* Outer Write-Back transient with read and write allocate */
            ARM_MPU_ATTR_MEMORY_(0UL, 0UL, 1UL, 1UL)  /* Inner Write-Through transient with read and write allocate */
        )
    );

    ARM_MPU_SetRegion (0UL,
        ARM_MPU_RBAR(0x08000000UL, ARM_MPU_SH_NON, 0UL, 1UL, 1UL),  /* Non-shareable, read/write, non-privileged, execute-never */
        ARM_MPU_RLAR(0x080FFFFFUL, 0UL)                             /* 1MB memory block using Attr 0 */
    );
    
    /* This region needs to be set to Read-only, can execute to execute code from this region */
    ARM_MPU_SetRegion (1UL,
        ARM_MPU_RBAR(0x00000000UL, ARM_MPU_SH_NON, 1UL, 1UL, 0UL),  /* Non-shareable, read only, non-privileged, can execute */
        ARM_MPU_RLAR(0x000FFFFFUL, 0UL)                             /* 1MB memory block using Attr 0 */
    );
        
    ARM_MPU_Enable(0);

    // Execute application code that is access protected by the MPU

    ARM_MPU_Disable();
}