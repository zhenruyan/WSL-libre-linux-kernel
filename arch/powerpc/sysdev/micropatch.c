// SPDX-License-Identifier: GPL-2.0

/* Microcode patches for the CPM as supplied by Motorola.
 * This is the one for IIC/SPI.  There is a newer one that
 * also relocates SMC2, but this would require additional changes
 * to uart.c, so I am holding off on that for a moment.
 */
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/param.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/interrupt.h>
#include <asm/irq.h>
#include <asm/page.h>
#include <asm/pgtable.h>
#include <asm/8xx_immap.h>
#include <asm/cpm.h>
#include <asm/cpm1.h>

/*
 * I2C/SPI relocation patch arrays.
 */

#ifdef CONFIG_I2C_SPI_UCODE_PATCH

/*(DEBLOBBED)*/

/*(DEBLOBBED)*/
#endif

/*
 * I2C/SPI/SMC1 relocation patch arrays.
 */

#ifdef CONFIG_I2C_SPI_SMC1_UCODE_PATCH

/*(DEBLOBBED)*/

/*(DEBLOBBED)*/

/*(DEBLOBBED)*/
#endif

/*
 *  USB SOF patch arrays.
 */

#ifdef CONFIG_USB_SOF_UCODE_PATCH

/*(DEBLOBBED)*/

/*(DEBLOBBED)*/
#endif

void __init cpm_load_patch(cpm8xx_t *cp)
{
	volatile uint		*dp;		/* Dual-ported RAM. */
	volatile cpm8xx_t	*commproc;
#if defined(CONFIG_I2C_SPI_UCODE_PATCH) || \
    defined(CONFIG_I2C_SPI_SMC1_UCODE_PATCH)
	volatile iic_t		*iip;
	volatile struct spi_pram *spp;
#ifdef CONFIG_I2C_SPI_SMC1_UCODE_PATCH
	volatile smc_uart_t	*smp;
#endif
#endif
	int	i;

	commproc = cp;

#ifdef CONFIG_USB_SOF_UCODE_PATCH
	commproc->cp_rccr = 0;

	dp = (uint *)(commproc->cp_dpmem);
	for (i=0; i<(sizeof(patch_2000)/4); i++)
		*dp++ = patch_2000[i];

	dp = (uint *)&(commproc->cp_dpmem[0x0f00]);
	for (i=0; i<(sizeof(patch_2f00)/4); i++)
		*dp++ = patch_2f00[i];

	commproc->cp_rccr = 0x0009;

	printk("USB SOF microcode patch installed\n");
#endif /* CONFIG_USB_SOF_UCODE_PATCH */

#if defined(CONFIG_I2C_SPI_UCODE_PATCH) || \
    defined(CONFIG_I2C_SPI_SMC1_UCODE_PATCH)

	commproc->cp_rccr = 0;

	dp = (uint *)(commproc->cp_dpmem);
	for (i=0; i<(sizeof(patch_2000)/4); i++)
		*dp++ = patch_2000[i];

	dp = (uint *)&(commproc->cp_dpmem[0x0f00]);
	for (i=0; i<(sizeof(patch_2f00)/4); i++)
		*dp++ = patch_2f00[i];

	iip = (iic_t *)&commproc->cp_dparam[PROFF_IIC];
# define RPBASE 0x0500
	iip->iic_rpbase = RPBASE;

	/* Put SPI above the IIC, also 32-byte aligned.
	*/
	i = (RPBASE + sizeof(iic_t) + 31) & ~31;
	spp = (struct spi_pram *)&commproc->cp_dparam[PROFF_SPI];
	spp->rpbase = i;

# if defined(CONFIG_I2C_SPI_UCODE_PATCH)
	commproc->cp_cpmcr1 = 0x802a;
	commproc->cp_cpmcr2 = 0x8028;
	commproc->cp_cpmcr3 = 0x802e;
	commproc->cp_cpmcr4 = 0x802c;
	commproc->cp_rccr = 1;

	printk("I2C/SPI microcode patch installed.\n");
# endif /* CONFIG_I2C_SPI_UCODE_PATCH */

# if defined(CONFIG_I2C_SPI_SMC1_UCODE_PATCH)

	dp = (uint *)&(commproc->cp_dpmem[0x0e00]);
	for (i=0; i<(sizeof(patch_2e00)/4); i++)
		*dp++ = patch_2e00[i];

	commproc->cp_cpmcr1 = 0x8080;
	commproc->cp_cpmcr2 = 0x808a;
	commproc->cp_cpmcr3 = 0x8028;
	commproc->cp_cpmcr4 = 0x802a;
	commproc->cp_rccr = 3;

	smp = (smc_uart_t *)&commproc->cp_dparam[PROFF_SMC1];
	smp->smc_rpbase = 0x1FC0;

	printk("I2C/SPI/SMC1 microcode patch installed.\n");
# endif /* CONFIG_I2C_SPI_SMC1_UCODE_PATCH) */

#endif /* some variation of the I2C/SPI patch was selected */
}

/*
 *  Take this entire routine out, since no one calls it and its
 * logic is suspect.
 */

#if 0
void
verify_patch(volatile immap_t *immr)
{
	volatile uint		*dp;
	volatile cpm8xx_t	*commproc;
	int i;

	commproc = (cpm8xx_t *)&immr->im_cpm;

	printk("cp_rccr %x\n", commproc->cp_rccr);
	commproc->cp_rccr = 0;

	dp = (uint *)(commproc->cp_dpmem);
	for (i=0; i<(sizeof(patch_2000)/4); i++)
		if (*dp++ != patch_2000[i]) {
			printk("patch_2000 bad at %d\n", i);
			dp--;
			printk("found 0x%X, wanted 0x%X\n", *dp, patch_2000[i]);
			break;
		}

	dp = (uint *)&(commproc->cp_dpmem[0x0f00]);
	for (i=0; i<(sizeof(patch_2f00)/4); i++)
		if (*dp++ != patch_2f00[i]) {
			printk("patch_2f00 bad at %d\n", i);
			dp--;
			printk("found 0x%X, wanted 0x%X\n", *dp, patch_2f00[i]);
			break;
		}

	commproc->cp_rccr = 0x0009;
}
#endif
