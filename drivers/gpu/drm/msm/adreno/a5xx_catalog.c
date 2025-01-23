// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2013-2014 Red Hat
 * Author: Rob Clark <robdclark@gmail.com>
 *
 * Copyright (c) 2014,2017 The Linux Foundation. All rights reserved.
 */

#include "adreno_gpu.h"

static const struct adreno_info a5xx_gpus[] = {
	{
		.chip_ids = ADRENO_CHIP_IDS(0x05000500),
		.family = ADRENO_5XX,
		.revn = 505,
		.fw = {
			[ADRENO_FW_PM4] = "/*(DEBLOBBED)*/",
			[ADRENO_FW_PFP] = "/*(DEBLOBBED)*/",
		},
		.gmem = (SZ_128K + SZ_8K),
		.inactive_period = DRM_MSM_INACTIVE_PERIOD,
		.quirks = ADRENO_QUIRK_TWO_PASS_USE_WFI |
			  ADRENO_QUIRK_LMLOADKILL_DISABLE,
		.init = a5xx_gpu_init,
	}, {
		.chip_ids = ADRENO_CHIP_IDS(0x05000600),
		.family = ADRENO_5XX,
		.revn = 506,
		.fw = {
			[ADRENO_FW_PM4] = "/*(DEBLOBBED)*/",
			[ADRENO_FW_PFP] = "/*(DEBLOBBED)*/",
		},
		.gmem = (SZ_128K + SZ_8K),
		/*
		 * Increase inactive period to 250 to avoid bouncing
		 * the GDSC which appears to make it grumpy
		 */
		.inactive_period = 250,
		.quirks = ADRENO_QUIRK_TWO_PASS_USE_WFI |
			  ADRENO_QUIRK_LMLOADKILL_DISABLE,
		.init = a5xx_gpu_init,
		.zapfw = "/*(DEBLOBBED)*/",
	}, {
		.chip_ids = ADRENO_CHIP_IDS(0x05000800),
		.family = ADRENO_5XX,
		.revn = 508,
		.fw = {
			[ADRENO_FW_PM4] = "/*(DEBLOBBED)*/",
			[ADRENO_FW_PFP] = "/*(DEBLOBBED)*/",
		},
		.gmem = (SZ_128K + SZ_8K),
		/*
		 * Increase inactive period to 250 to avoid bouncing
		 * the GDSC which appears to make it grumpy
		 */
		.inactive_period = 250,
		.quirks = ADRENO_QUIRK_LMLOADKILL_DISABLE,
		.init = a5xx_gpu_init,
		.zapfw = "/*(DEBLOBBED)*/",
	}, {
		.chip_ids = ADRENO_CHIP_IDS(0x05000900),
		.family = ADRENO_5XX,
		.revn = 509,
		.fw = {
			[ADRENO_FW_PM4] = "/*(DEBLOBBED)*/",
			[ADRENO_FW_PFP] = "/*(DEBLOBBED)*/",
		},
		.gmem = (SZ_256K + SZ_16K),
		/*
		 * Increase inactive period to 250 to avoid bouncing
		 * the GDSC which appears to make it grumpy
		 */
		.inactive_period = 250,
		.quirks = ADRENO_QUIRK_LMLOADKILL_DISABLE,
		.init = a5xx_gpu_init,
		/* Adreno 509 uses the same ZAP as 512 */
		.zapfw = "/*(DEBLOBBED)*/",
	}, {
		.chip_ids = ADRENO_CHIP_IDS(0x05010000),
		.family = ADRENO_5XX,
		.revn = 510,
		.fw = {
			[ADRENO_FW_PM4] = "/*(DEBLOBBED)*/",
			[ADRENO_FW_PFP] = "/*(DEBLOBBED)*/",
		},
		.gmem = SZ_256K,
		/*
		 * Increase inactive period to 250 to avoid bouncing
		 * the GDSC which appears to make it grumpy
		 */
		.inactive_period = 250,
		.init = a5xx_gpu_init,
	}, {
		.chip_ids = ADRENO_CHIP_IDS(0x05010200),
		.family = ADRENO_5XX,
		.revn = 512,
		.fw = {
			[ADRENO_FW_PM4] = "/*(DEBLOBBED)*/",
			[ADRENO_FW_PFP] = "/*(DEBLOBBED)*/",
		},
		.gmem = (SZ_256K + SZ_16K),
		/*
		 * Increase inactive period to 250 to avoid bouncing
		 * the GDSC which appears to make it grumpy
		 */
		.inactive_period = 250,
		.quirks = ADRENO_QUIRK_LMLOADKILL_DISABLE,
		.init = a5xx_gpu_init,
		.zapfw = "/*(DEBLOBBED)*/",
	}, {
		.chip_ids = ADRENO_CHIP_IDS(
			0x05030002,
			0x05030004
		),
		.family = ADRENO_5XX,
		.revn = 530,
		.fw = {
			[ADRENO_FW_PM4] = "/*(DEBLOBBED)*/",
			[ADRENO_FW_PFP] = "/*(DEBLOBBED)*/",
			[ADRENO_FW_GPMU] = "/*(DEBLOBBED)*/",
		},
		.gmem = SZ_1M,
		/*
		 * Increase inactive period to 250 to avoid bouncing
		 * the GDSC which appears to make it grumpy
		 */
		.inactive_period = 250,
		.quirks = ADRENO_QUIRK_TWO_PASS_USE_WFI |
			ADRENO_QUIRK_FAULT_DETECT_MASK,
		.init = a5xx_gpu_init,
		.zapfw = "/*(DEBLOBBED)*/",
	}, {
		.chip_ids = ADRENO_CHIP_IDS(0x05040001),
		.family = ADRENO_5XX,
		.revn = 540,
		.fw = {
			[ADRENO_FW_PM4] = "/*(DEBLOBBED)*/",
			[ADRENO_FW_PFP] = "/*(DEBLOBBED)*/",
			[ADRENO_FW_GPMU] = "/*(DEBLOBBED)*/",
		},
		.gmem = SZ_1M,
		/*
		 * Increase inactive period to 250 to avoid bouncing
		 * the GDSC which appears to make it grumpy
		 */
		.inactive_period = 250,
		.quirks = ADRENO_QUIRK_LMLOADKILL_DISABLE,
		.init = a5xx_gpu_init,
		.zapfw = "/*(DEBLOBBED)*/",
	}
};
DECLARE_ADRENO_GPULIST(a5xx);

/*(DEBLOBBED)*/
