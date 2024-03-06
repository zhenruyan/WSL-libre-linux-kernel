// SPDX-License-Identifier: GPL-2.0

/*
 * Microcode patches for the CPM as supplied by Motorola.
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
#include <asm/8xx_immap.h>
#include <asm/cpm.h>
#include <asm/cpm1.h>

struct patch_params {
	ushort rccr;
	ushort cpmcr1;
	ushort cpmcr2;
	ushort cpmcr3;
	ushort cpmcr4;
};

/*
 * I2C/SPI relocation patch arrays.
 */

#ifdef CONFIG_I2C_SPI_UCODE_PATCH

static char patch_name[] __initdata = "I2C/SPI";

static struct patch_params patch_params __initdata = {
	1, 0x802a, 0x8028, 0x802e, 0x802c,
};

/*(DEBLOBBED)*/

/*(DEBLOBBED)*/

/*(DEBLOBBED)*/
#endif

/*
 * I2C/SPI/SMC1 relocation patch arrays.
 */

#ifdef CONFIG_I2C_SPI_SMC1_UCODE_PATCH

static char patch_name[] __initdata = "I2C/SPI/SMC1";

static struct patch_params patch_params __initdata = {
	3, 0x8080, 0x808a, 0x8028, 0x802a,
};

/*(DEBLOBBED)*/

/*(DEBLOBBED)*/

/*(DEBLOBBED)*/
#endif

/*
 *  USB SOF patch arrays.
 */

#ifdef CONFIG_USB_SOF_UCODE_PATCH

static char patch_name[] __initdata = "USB SOF";

static struct patch_params patch_params __initdata = {
	9,
};

/*(DEBLOBBED)*/

/*(DEBLOBBED)*/

/*(DEBLOBBED)*/
#endif

/*
 * SMC relocation patch arrays.
 */

#ifdef CONFIG_SMC_UCODE_PATCH

static char patch_name[] __initdata = "SMC";

static struct patch_params patch_params __initdata = {
	2, 0x8080, 0x8088,
};

/*(DEBLOBBED)*/

/*(DEBLOBBED)*/

/*(DEBLOBBED)*/
#endif

static void __init cpm_write_patch(cpm8xx_t *cp, int offset, uint *patch, int len)
{
	if (!len)
		return;
	memcpy_toio(cp->cp_dpmem + offset, patch, len);
}

void __init cpm_load_patch(cpm8xx_t *cp)
{
	out_be16(&cp->cp_rccr, 0);

	/*(DEBLOBBED)*/
	/*(DEBLOBBED)*/
	/*(DEBLOBBED)*/

	if (IS_ENABLED(CONFIG_I2C_SPI_UCODE_PATCH) ||
	    IS_ENABLED(CONFIG_I2C_SPI_SMC1_UCODE_PATCH)) {
		u16 rpbase = 0x500;
		iic_t *iip;
		struct spi_pram *spp;

		iip = (iic_t *)&cp->cp_dparam[PROFF_IIC];
		out_be16(&iip->iic_rpbase, rpbase);

		/* Put SPI above the IIC, also 32-byte aligned. */
		spp = (struct spi_pram *)&cp->cp_dparam[PROFF_SPI];
		out_be16(&spp->rpbase, (rpbase + sizeof(iic_t) + 31) & ~31);

		if (IS_ENABLED(CONFIG_I2C_SPI_SMC1_UCODE_PATCH)) {
			smc_uart_t *smp;

			smp = (smc_uart_t *)&cp->cp_dparam[PROFF_SMC1];
			out_be16(&smp->smc_rpbase, 0x1FC0);
		}
	}

	if (IS_ENABLED(CONFIG_SMC_UCODE_PATCH)) {
		smc_uart_t *smp;

		if (IS_ENABLED(CONFIG_PPC_EARLY_DEBUG_CPM)) {
			int i;

			for (i = 0; i < sizeof(*smp); i += 4) {
				u32 __iomem *src = (u32 __iomem *)&cp->cp_dparam[PROFF_SMC1 + i];
				u32 __iomem *dst = (u32 __iomem *)&cp->cp_dparam[PROFF_DSP1 + i];

				out_be32(dst, in_be32(src));
			}
		}

		smp = (smc_uart_t *)&cp->cp_dparam[PROFF_SMC1];
		out_be16(&smp->smc_rpbase, 0x1ec0);
		smp = (smc_uart_t *)&cp->cp_dparam[PROFF_SMC2];
		out_be16(&smp->smc_rpbase, 0x1fc0);
	}

	out_be16(&cp->cp_cpmcr1, patch_params.cpmcr1);
	out_be16(&cp->cp_cpmcr2, patch_params.cpmcr2);
	out_be16(&cp->cp_cpmcr3, patch_params.cpmcr3);
	out_be16(&cp->cp_cpmcr4, patch_params.cpmcr4);

	out_be16(&cp->cp_rccr, patch_params.rccr);

	pr_info("%s microcode patch installed\n", patch_name);
}
