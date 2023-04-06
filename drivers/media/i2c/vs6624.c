// SPDX-License-Identifier: GPL-2.0-only
/*
 * vs6624.c ST VS6624 CMOS image sensor driver
 *
 * Copyright (c) 2011 Analog Devices Inc.
 */

#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/gpio.h>
#include <linux/i2c.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/videodev2.h>

#include <media/v4l2-ctrls.h>
#include <media/v4l2-device.h>
#include <media/v4l2-mediabus.h>
#include <media/v4l2-image-sizes.h>

#include "vs6624_regs.h"

#define MAX_FRAME_RATE  30

struct vs6624 {
	struct v4l2_subdev sd;
	struct v4l2_ctrl_handler hdl;
	struct v4l2_fract frame_rate;
	struct v4l2_mbus_framefmt fmt;
	unsigned ce_pin;
};

static const struct vs6624_format {
	u32 mbus_code;
	enum v4l2_colorspace colorspace;
} vs6624_formats[] = {
	{
		.mbus_code      = MEDIA_BUS_FMT_UYVY8_2X8,
		.colorspace     = V4L2_COLORSPACE_JPEG,
	},
	{
		.mbus_code      = MEDIA_BUS_FMT_YUYV8_2X8,
		.colorspace     = V4L2_COLORSPACE_JPEG,
	},
	{
		.mbus_code      = MEDIA_BUS_FMT_RGB565_2X8_LE,
		.colorspace     = V4L2_COLORSPACE_SRGB,
	},
};

static const struct v4l2_mbus_framefmt vs6624_default_fmt = {
	.width = VGA_WIDTH,
	.height = VGA_HEIGHT,
	.code = MEDIA_BUS_FMT_UYVY8_2X8,
	.field = V4L2_FIELD_NONE,
	.colorspace = V4L2_COLORSPACE_JPEG,
};

/*(DEBLOBBED)*/

static const u16 vs6624_p2[] = {
	0x806f, 0x01,
	0x058c, 0x01,
	0x0000, 0x00,
};

static const u16 vs6624_run_setup[] = {
	0x1d18, 0x00,				/* Enableconstrainedwhitebalance */
	VS6624_PEAK_MIN_OUT_G_MSB, 0x3c,	/* Damper PeakGain Output MSB */
	VS6624_PEAK_MIN_OUT_G_LSB, 0x66,	/* Damper PeakGain Output LSB */
	VS6624_CM_LOW_THR_MSB, 0x65,		/* Damper Low MSB */
	VS6624_CM_LOW_THR_LSB, 0xd1,		/* Damper Low LSB */
	VS6624_CM_HIGH_THR_MSB, 0x66,		/* Damper High MSB */
	VS6624_CM_HIGH_THR_LSB, 0x62,		/* Damper High LSB */
	VS6624_CM_MIN_OUT_MSB, 0x00,		/* Damper Min output MSB */
	VS6624_CM_MIN_OUT_LSB, 0x00,		/* Damper Min output LSB */
	VS6624_NORA_DISABLE, 0x00,		/* Nora fDisable */
	VS6624_NORA_USAGE, 0x04,		/* Nora usage */
	VS6624_NORA_LOW_THR_MSB, 0x63,		/* Damper Low MSB Changed 0x63 to 0x65 */
	VS6624_NORA_LOW_THR_LSB, 0xd1,		/* Damper Low LSB */
	VS6624_NORA_HIGH_THR_MSB, 0x68,		/* Damper High MSB */
	VS6624_NORA_HIGH_THR_LSB, 0xdd,		/* Damper High LSB */
	VS6624_NORA_MIN_OUT_MSB, 0x3a,		/* Damper Min output MSB */
	VS6624_NORA_MIN_OUT_LSB, 0x00,		/* Damper Min output LSB */
	VS6624_F2B_DISABLE, 0x00,		/* Disable */
	0x1d8a, 0x30,				/* MAXWeightHigh */
	0x1d91, 0x62,				/* fpDamperLowThresholdHigh MSB */
	0x1d92, 0x4a,				/* fpDamperLowThresholdHigh LSB */
	0x1d95, 0x65,				/* fpDamperHighThresholdHigh MSB */
	0x1d96, 0x0e,				/* fpDamperHighThresholdHigh LSB */
	0x1da1, 0x3a,				/* fpMinimumDamperOutputLow MSB */
	0x1da2, 0xb8,				/* fpMinimumDamperOutputLow LSB */
	0x1e08, 0x06,				/* MAXWeightLow */
	0x1e0a, 0x0a,				/* MAXWeightHigh */
	0x1601, 0x3a,				/* Red A MSB */
	0x1602, 0x14,				/* Red A LSB */
	0x1605, 0x3b,				/* Blue A MSB */
	0x1606, 0x85,				/* BLue A LSB */
	0x1609, 0x3b,				/* RED B MSB */
	0x160a, 0x85,				/* RED B LSB */
	0x160d, 0x3a,				/* Blue B MSB */
	0x160e, 0x14,				/* Blue B LSB */
	0x1611, 0x30,				/* Max Distance from Locus MSB */
	0x1612, 0x8f,				/* Max Distance from Locus MSB */
	0x1614, 0x01,				/* Enable constrainer */
	0x0000, 0x00,
};

static const u16 vs6624_default[] = {
	VS6624_CONTRAST0, 0x84,
	VS6624_SATURATION0, 0x75,
	VS6624_GAMMA0, 0x11,
	VS6624_CONTRAST1, 0x84,
	VS6624_SATURATION1, 0x75,
	VS6624_GAMMA1, 0x11,
	VS6624_MAN_RG, 0x80,
	VS6624_MAN_GG, 0x80,
	VS6624_MAN_BG, 0x80,
	VS6624_WB_MODE, 0x1,
	VS6624_EXPO_COMPENSATION, 0xfe,
	VS6624_EXPO_METER, 0x0,
	VS6624_LIGHT_FREQ, 0x64,
	VS6624_PEAK_GAIN, 0xe,
	VS6624_PEAK_LOW_THR, 0x28,
	VS6624_HMIRROR0, 0x0,
	VS6624_VFLIP0, 0x0,
	VS6624_ZOOM_HSTEP0_MSB, 0x0,
	VS6624_ZOOM_HSTEP0_LSB, 0x1,
	VS6624_ZOOM_VSTEP0_MSB, 0x0,
	VS6624_ZOOM_VSTEP0_LSB, 0x1,
	VS6624_PAN_HSTEP0_MSB, 0x0,
	VS6624_PAN_HSTEP0_LSB, 0xf,
	VS6624_PAN_VSTEP0_MSB, 0x0,
	VS6624_PAN_VSTEP0_LSB, 0xf,
	VS6624_SENSOR_MODE, 0x1,
	VS6624_SYNC_CODE_SETUP, 0x21,
	VS6624_DISABLE_FR_DAMPER, 0x0,
	VS6624_FR_DEN, 0x1,
	VS6624_FR_NUM_LSB, 0xf,
	VS6624_INIT_PIPE_SETUP, 0x0,
	VS6624_IMG_FMT0, 0x0,
	VS6624_YUV_SETUP, 0x1,
	VS6624_IMAGE_SIZE0, 0x2,
	0x0000, 0x00,
};

static inline struct vs6624 *to_vs6624(struct v4l2_subdev *sd)
{
	return container_of(sd, struct vs6624, sd);
}
static inline struct v4l2_subdev *to_sd(struct v4l2_ctrl *ctrl)
{
	return &container_of(ctrl->handler, struct vs6624, hdl)->sd;
}

#ifdef CONFIG_VIDEO_ADV_DEBUG
static int vs6624_read(struct v4l2_subdev *sd, u16 index)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	u8 buf[2];

	buf[0] = index >> 8;
	buf[1] = index;
	i2c_master_send(client, buf, 2);
	i2c_master_recv(client, buf, 1);

	return buf[0];
}
#endif

static int vs6624_write(struct v4l2_subdev *sd, u16 index,
				u8 value)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	u8 buf[3];

	buf[0] = index >> 8;
	buf[1] = index;
	buf[2] = value;

	return i2c_master_send(client, buf, 3);
}

static int vs6624_writeregs(struct v4l2_subdev *sd, const u16 *regs)
{
	u16 reg;
	u8 data;

	while (*regs != 0x00) {
		reg = *regs++;
		data = *regs++;

		vs6624_write(sd, reg, data);
	}
	return 0;
}

static int vs6624_s_ctrl(struct v4l2_ctrl *ctrl)
{
	struct v4l2_subdev *sd = to_sd(ctrl);

	switch (ctrl->id) {
	case V4L2_CID_CONTRAST:
		vs6624_write(sd, VS6624_CONTRAST0, ctrl->val);
		break;
	case V4L2_CID_SATURATION:
		vs6624_write(sd, VS6624_SATURATION0, ctrl->val);
		break;
	case V4L2_CID_HFLIP:
		vs6624_write(sd, VS6624_HMIRROR0, ctrl->val);
		break;
	case V4L2_CID_VFLIP:
		vs6624_write(sd, VS6624_VFLIP0, ctrl->val);
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

static int vs6624_enum_mbus_code(struct v4l2_subdev *sd,
		struct v4l2_subdev_state *sd_state,
		struct v4l2_subdev_mbus_code_enum *code)
{
	if (code->pad || code->index >= ARRAY_SIZE(vs6624_formats))
		return -EINVAL;

	code->code = vs6624_formats[code->index].mbus_code;
	return 0;
}

static int vs6624_set_fmt(struct v4l2_subdev *sd,
		struct v4l2_subdev_state *sd_state,
		struct v4l2_subdev_format *format)
{
	struct v4l2_mbus_framefmt *fmt = &format->format;
	struct vs6624 *sensor = to_vs6624(sd);
	int index;

	if (format->pad)
		return -EINVAL;

	for (index = 0; index < ARRAY_SIZE(vs6624_formats); index++)
		if (vs6624_formats[index].mbus_code == fmt->code)
			break;
	if (index >= ARRAY_SIZE(vs6624_formats)) {
		/* default to first format */
		index = 0;
		fmt->code = vs6624_formats[0].mbus_code;
	}

	/* sensor mode is VGA */
	if (fmt->width > VGA_WIDTH)
		fmt->width = VGA_WIDTH;
	if (fmt->height > VGA_HEIGHT)
		fmt->height = VGA_HEIGHT;
	fmt->width = fmt->width & (~3);
	fmt->height = fmt->height & (~3);
	fmt->field = V4L2_FIELD_NONE;
	fmt->colorspace = vs6624_formats[index].colorspace;

	if (format->which == V4L2_SUBDEV_FORMAT_TRY) {
		sd_state->pads->try_fmt = *fmt;
		return 0;
	}

	/* set image format */
	switch (fmt->code) {
	case MEDIA_BUS_FMT_UYVY8_2X8:
		vs6624_write(sd, VS6624_IMG_FMT0, 0x0);
		vs6624_write(sd, VS6624_YUV_SETUP, 0x1);
		break;
	case MEDIA_BUS_FMT_YUYV8_2X8:
		vs6624_write(sd, VS6624_IMG_FMT0, 0x0);
		vs6624_write(sd, VS6624_YUV_SETUP, 0x3);
		break;
	case MEDIA_BUS_FMT_RGB565_2X8_LE:
		vs6624_write(sd, VS6624_IMG_FMT0, 0x4);
		vs6624_write(sd, VS6624_RGB_SETUP, 0x0);
		break;
	default:
		return -EINVAL;
	}

	/* set image size */
	if ((fmt->width == VGA_WIDTH) && (fmt->height == VGA_HEIGHT))
		vs6624_write(sd, VS6624_IMAGE_SIZE0, 0x2);
	else if ((fmt->width == QVGA_WIDTH) && (fmt->height == QVGA_HEIGHT))
		vs6624_write(sd, VS6624_IMAGE_SIZE0, 0x4);
	else if ((fmt->width == QQVGA_WIDTH) && (fmt->height == QQVGA_HEIGHT))
		vs6624_write(sd, VS6624_IMAGE_SIZE0, 0x6);
	else if ((fmt->width == CIF_WIDTH) && (fmt->height == CIF_HEIGHT))
		vs6624_write(sd, VS6624_IMAGE_SIZE0, 0x3);
	else if ((fmt->width == QCIF_WIDTH) && (fmt->height == QCIF_HEIGHT))
		vs6624_write(sd, VS6624_IMAGE_SIZE0, 0x5);
	else if ((fmt->width == QQCIF_WIDTH) && (fmt->height == QQCIF_HEIGHT))
		vs6624_write(sd, VS6624_IMAGE_SIZE0, 0x7);
	else {
		vs6624_write(sd, VS6624_IMAGE_SIZE0, 0x8);
		vs6624_write(sd, VS6624_MAN_HSIZE0_MSB, fmt->width >> 8);
		vs6624_write(sd, VS6624_MAN_HSIZE0_LSB, fmt->width & 0xFF);
		vs6624_write(sd, VS6624_MAN_VSIZE0_MSB, fmt->height >> 8);
		vs6624_write(sd, VS6624_MAN_VSIZE0_LSB, fmt->height & 0xFF);
		vs6624_write(sd, VS6624_CROP_CTRL0, 0x1);
	}

	sensor->fmt = *fmt;

	return 0;
}

static int vs6624_get_fmt(struct v4l2_subdev *sd,
		struct v4l2_subdev_state *sd_state,
		struct v4l2_subdev_format *format)
{
	struct vs6624 *sensor = to_vs6624(sd);

	if (format->pad)
		return -EINVAL;

	format->format = sensor->fmt;
	return 0;
}

static int vs6624_g_frame_interval(struct v4l2_subdev *sd,
				   struct v4l2_subdev_frame_interval *ival)
{
	struct vs6624 *sensor = to_vs6624(sd);

	ival->interval.numerator = sensor->frame_rate.denominator;
	ival->interval.denominator = sensor->frame_rate.numerator;
	return 0;
}

static int vs6624_s_frame_interval(struct v4l2_subdev *sd,
				   struct v4l2_subdev_frame_interval *ival)
{
	struct vs6624 *sensor = to_vs6624(sd);
	struct v4l2_fract *tpf = &ival->interval;


	if (tpf->numerator == 0 || tpf->denominator == 0
		|| (tpf->denominator > tpf->numerator * MAX_FRAME_RATE)) {
		/* reset to max frame rate */
		tpf->numerator = 1;
		tpf->denominator = MAX_FRAME_RATE;
	}
	sensor->frame_rate.numerator = tpf->denominator;
	sensor->frame_rate.denominator = tpf->numerator;
	vs6624_write(sd, VS6624_DISABLE_FR_DAMPER, 0x0);
	vs6624_write(sd, VS6624_FR_NUM_MSB,
			sensor->frame_rate.numerator >> 8);
	vs6624_write(sd, VS6624_FR_NUM_LSB,
			sensor->frame_rate.numerator & 0xFF);
	vs6624_write(sd, VS6624_FR_DEN,
			sensor->frame_rate.denominator & 0xFF);
	return 0;
}

static int vs6624_s_stream(struct v4l2_subdev *sd, int enable)
{
	if (enable)
		vs6624_write(sd, VS6624_USER_CMD, 0x2);
	else
		vs6624_write(sd, VS6624_USER_CMD, 0x4);
	udelay(100);
	return 0;
}

#ifdef CONFIG_VIDEO_ADV_DEBUG
static int vs6624_g_register(struct v4l2_subdev *sd, struct v4l2_dbg_register *reg)
{
	reg->val = vs6624_read(sd, reg->reg & 0xffff);
	reg->size = 1;
	return 0;
}

static int vs6624_s_register(struct v4l2_subdev *sd, const struct v4l2_dbg_register *reg)
{
	vs6624_write(sd, reg->reg & 0xffff, reg->val & 0xff);
	return 0;
}
#endif

static const struct v4l2_ctrl_ops vs6624_ctrl_ops = {
	.s_ctrl = vs6624_s_ctrl,
};

static const struct v4l2_subdev_core_ops vs6624_core_ops = {
#ifdef CONFIG_VIDEO_ADV_DEBUG
	.g_register = vs6624_g_register,
	.s_register = vs6624_s_register,
#endif
};

static const struct v4l2_subdev_video_ops vs6624_video_ops = {
	.s_frame_interval = vs6624_s_frame_interval,
	.g_frame_interval = vs6624_g_frame_interval,
	.s_stream = vs6624_s_stream,
};

static const struct v4l2_subdev_pad_ops vs6624_pad_ops = {
	.enum_mbus_code = vs6624_enum_mbus_code,
	.get_fmt = vs6624_get_fmt,
	.set_fmt = vs6624_set_fmt,
};

static const struct v4l2_subdev_ops vs6624_ops = {
	.core = &vs6624_core_ops,
	.video = &vs6624_video_ops,
	.pad = &vs6624_pad_ops,
};

static int vs6624_probe(struct i2c_client *client)
{
	struct vs6624 *sensor;
	struct v4l2_subdev *sd;
	struct v4l2_ctrl_handler *hdl;
	const unsigned *ce;
	int ret;

	/* Check if the adapter supports the needed features */
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C))
		return -EIO;

	ce = client->dev.platform_data;
	if (ce == NULL)
		return -EINVAL;

	ret = devm_gpio_request_one(&client->dev, *ce, GPIOF_OUT_INIT_HIGH,
				    "VS6624 Chip Enable");
	if (ret) {
		v4l_err(client, "failed to request GPIO %d\n", *ce);
		return ret;
	}
	/* wait 100ms before any further i2c writes are performed */
	msleep(100);

	sensor = devm_kzalloc(&client->dev, sizeof(*sensor), GFP_KERNEL);
	if (sensor == NULL)
		return -ENOMEM;

	sd = &sensor->sd;
	v4l2_i2c_subdev_init(sd, client, &vs6624_ops);

	/*(DEBLOBBED)*/
	vs6624_write(sd, VS6624_MICRO_EN, 0x2);
	vs6624_write(sd, VS6624_DIO_EN, 0x1);
	usleep_range(10000, 11000);
	vs6624_writeregs(sd, vs6624_p2);

	vs6624_writeregs(sd, vs6624_default);
	vs6624_write(sd, VS6624_HSYNC_SETUP, 0xF);
	vs6624_writeregs(sd, vs6624_run_setup);

	/* set frame rate */
	sensor->frame_rate.numerator = MAX_FRAME_RATE;
	sensor->frame_rate.denominator = 1;
	vs6624_write(sd, VS6624_DISABLE_FR_DAMPER, 0x0);
	vs6624_write(sd, VS6624_FR_NUM_MSB,
			sensor->frame_rate.numerator >> 8);
	vs6624_write(sd, VS6624_FR_NUM_LSB,
			sensor->frame_rate.numerator & 0xFF);
	vs6624_write(sd, VS6624_FR_DEN,
			sensor->frame_rate.denominator & 0xFF);

	sensor->fmt = vs6624_default_fmt;
	sensor->ce_pin = *ce;

	v4l_info(client, "chip found @ 0x%02x (%s)\n",
			client->addr << 1, client->adapter->name);

	hdl = &sensor->hdl;
	v4l2_ctrl_handler_init(hdl, 4);
	v4l2_ctrl_new_std(hdl, &vs6624_ctrl_ops,
			V4L2_CID_CONTRAST, 0, 0xFF, 1, 0x87);
	v4l2_ctrl_new_std(hdl, &vs6624_ctrl_ops,
			V4L2_CID_SATURATION, 0, 0xFF, 1, 0x78);
	v4l2_ctrl_new_std(hdl, &vs6624_ctrl_ops,
			V4L2_CID_HFLIP, 0, 1, 1, 0);
	v4l2_ctrl_new_std(hdl, &vs6624_ctrl_ops,
			V4L2_CID_VFLIP, 0, 1, 1, 0);
	/* hook the control handler into the driver */
	sd->ctrl_handler = hdl;
	if (hdl->error) {
		int err = hdl->error;

		v4l2_ctrl_handler_free(hdl);
		return err;
	}

	/* initialize the hardware to the default control values */
	ret = v4l2_ctrl_handler_setup(hdl);
	if (ret)
		v4l2_ctrl_handler_free(hdl);
	return ret;
}

static void vs6624_remove(struct i2c_client *client)
{
	struct v4l2_subdev *sd = i2c_get_clientdata(client);

	v4l2_device_unregister_subdev(sd);
	v4l2_ctrl_handler_free(sd->ctrl_handler);
}

static const struct i2c_device_id vs6624_id[] = {
	{"vs6624", 0},
	{},
};

MODULE_DEVICE_TABLE(i2c, vs6624_id);

static struct i2c_driver vs6624_driver = {
	.driver = {
		.name   = "vs6624",
	},
	.probe_new      = vs6624_probe,
	.remove         = vs6624_remove,
	.id_table       = vs6624_id,
};

module_i2c_driver(vs6624_driver);

MODULE_DESCRIPTION("VS6624 sensor driver");
MODULE_AUTHOR("Scott Jiang <Scott.Jiang.Linux@gmail.com>");
MODULE_LICENSE("GPL v2");
