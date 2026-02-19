/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_FIRMWARE_H
#define _LINUX_FIRMWARE_H

#include <linux/types.h>
#include <linux/compiler.h>
#include <linux/gfp.h>

#define FW_ACTION_NOHOTPLUG 0
#define FW_ACTION_HOTPLUG 1

struct firmware {
	size_t size;
	const u8 *data;

	/* firmware loader private fields */
	void *priv;
};

struct module;
struct device;

struct builtin_fw {
	char *name;
	void *data;
	unsigned long size;
};

/* We have to play tricks here much like stringify() to get the
   __COUNTER__ macro to be expanded as we want it */
#define __fw_concat1(x, y) x##y
#define __fw_concat(x, y) __fw_concat1(x, y)

#define DECLARE_BUILTIN_FIRMWARE(name, blob)				     \
	DECLARE_BUILTIN_FIRMWARE_SIZE(name, &(blob), sizeof(blob))

#define DECLARE_BUILTIN_FIRMWARE_SIZE(name, blob, size)			     \
	static const struct builtin_fw __fw_concat(__builtin_fw,__COUNTER__) \
	__used __section(".builtin_fw") = { name, blob, size }

#if defined(CONFIG_FW_LOADER) || (defined(CONFIG_FW_LOADER_MODULE) && defined(MODULE))
int request_firmware(const struct firmware **fw, const char *name,
		     struct device *device);
int firmware_request_nowarn(const struct firmware **fw, const char *name,
			    struct device *device);
int firmware_request_platform(const struct firmware **fw, const char *name,
			      struct device *device);
int request_firmware_nowait(
	struct module *module, bool uevent,
	const char *name, struct device *device, gfp_t gfp, void *context,
	void (*cont)(const struct firmware *fw, void *context));
int request_firmware_direct(const struct firmware **fw, const char *name,
			    struct device *device);
int request_firmware_into_buf(const struct firmware **firmware_p,
	const char *name, struct device *device, void *buf, size_t size);
int request_partial_firmware_into_buf(const struct firmware **firmware_p,
				      const char *name, struct device *device,
				      void *buf, size_t size, size_t offset);

void release_firmware(const struct firmware *fw);
#else
static inline int request_firmware(const struct firmware **fw,
				   const char *name,
				   struct device *device)
{
	return -EINVAL;
}

static inline int firmware_request_nowarn(const struct firmware **fw,
					  const char *name,
					  struct device *device)
{
	return -EINVAL;
}

static inline int firmware_request_platform(const struct firmware **fw,
					    const char *name,
					    struct device *device)
{
	return -EINVAL;
}

static inline int request_firmware_nowait(
	struct module *module, bool uevent,
	const char *name, struct device *device, gfp_t gfp, void *context,
	void (*cont)(const struct firmware *fw, void *context))
{
	return -EINVAL;
}

static inline void release_firmware(const struct firmware *fw)
{
}

static inline int request_firmware_direct(const struct firmware **fw,
					  const char *name,
					  struct device *device)
{
	return -EINVAL;
}

static inline int request_firmware_into_buf(const struct firmware **firmware_p,
	const char *name, struct device *device, void *buf, size_t size)
{
	return -EINVAL;
}

static inline int request_partial_firmware_into_buf
					(const struct firmware **firmware_p,
					 const char *name,
					 struct device *device,
					 void *buf, size_t size, size_t offset)
{
	return -EINVAL;
}

#endif

int firmware_request_cache(struct device *device, const char *name);

#ifndef _LINUX_LIBRE_FIRMWARE_H
#define _LINUX_LIBRE_FIRMWARE_H

#include <linux/device.h>

#define NONFREE_FIRMWARE "/*(DEBLOBBED)*/"

static inline int
is_nonfree_firmware(const char *name)
{
	return strstr(name, NONFREE_FIRMWARE) != 0;
}

static inline int
report_missing_free_firmware(const char *name, const char *what)
{
	printk(KERN_ERR "%s: Missing Free %s (non-Free firmware loading is disabled)\n", name,
	       what ? what : "firmware");
	return -ENOENT;
}
static inline int
firmware_reject_nowarn(const struct firmware **fw,
		       const char *name, struct device *device)
{
	const struct firmware *xfw = NULL;
	int retval, retval0 = -ENOENT;
	if (fw) *fw = xfw;
	retval = firmware_request_nowarn(&xfw, NONFREE_FIRMWARE, device);
	if (!retval) {
		release_firmware(xfw);
		retval = retval0;
	}
	return retval;
}
static inline int
reject_firmware(const struct firmware **fw,
		const char *name, struct device *device)
{
	int retval, retval0;
	retval0 = report_missing_free_firmware(dev_name(device), NULL);
	retval = firmware_reject_nowarn(fw, name, device);
	if (!retval) {
		retval = retval0;
	}
	return retval;
}
static inline int
maybe_reject_firmware(const struct firmware **fw,
		      const char *name, struct device *device)
{
	if (is_nonfree_firmware(name))
		return reject_firmware(fw, name, device);
	else
		return request_firmware(fw, name, device);
}
static inline int
reject_firmware_direct(const struct firmware **fw,
		const char *name, struct device *device)
{
	const struct firmware *xfw = NULL;
	int retval, retval0;
	if (fw) *fw = xfw;
	retval0 = report_missing_free_firmware(dev_name(device), NULL);
	retval = request_firmware_direct(&xfw, NONFREE_FIRMWARE, device);
	if (!retval) {
		release_firmware(xfw);
		retval = retval0;
	}
	return retval;
}
static inline int
reject_firmware_nowait(struct module *module, int uevent,
		       const char *name, struct device *device,
		       gfp_t gfp, void *context,
		       void (*cont)(const struct firmware *fw,
				    void *context))
{
	report_missing_free_firmware(dev_name(device), NULL);
	/* We assume NONFREE_FIRMWARE will not be found; how could it?  */
	return request_firmware_nowait(module, uevent, NONFREE_FIRMWARE,
				       device, gfp, context, cont);
}
static inline int
maybe_reject_firmware_nowait(struct module *module, int uevent,
			     const char *name, struct device *device,
			     gfp_t gfp, void *context,
			     void (*cont)(const struct firmware *fw,
					  void *context))
{
	if (is_nonfree_firmware(name))
		return reject_firmware_nowait(module, uevent, name,
					      device, gfp, context, cont);
	else
		return request_firmware_nowait(module, uevent, name,
					       device, gfp, context, cont);
}
static inline int
reject_firmware_into_buf(const struct firmware **firmware_p, const char *name,
			 struct device *device, void *buf, size_t size)
{
	const struct firmware *xfw = NULL;
	int retval, retval0;
	if (firmware_p) *firmware_p = xfw;
	retval0 = report_missing_free_firmware(dev_name(device), NULL);
	retval = request_firmware_into_buf(&xfw, NONFREE_FIRMWARE, device, buf, size);
	if (!retval) {
		release_firmware(xfw);
		retval = retval0;
	}
	return retval;
}
static inline int
maybe_reject_firmware_into_buf(const struct firmware **firmware_p, const char *name,
			       struct device *device, void *buf, size_t size)
{
	if (is_nonfree_firmware(name))
		return reject_firmware_into_buf(firmware_p, name, device, buf, size);
	else
		return request_firmware_into_buf(firmware_p, name, device, buf, size);
}
static inline int
reject_partial_firmware_into_buf(const struct firmware **firmware_p, const char *name,
				 struct device *device, void *buf, size_t size, size_t offset)
{
	const struct firmware *xfw = NULL;
	int retval, retval0;
	if (firmware_p) *firmware_p = xfw;
	retval0 = report_missing_free_firmware(dev_name(device), NULL);
	retval = request_partial_firmware_into_buf(&xfw, NONFREE_FIRMWARE, device, buf, size, offset);
	if (!retval) {
		release_firmware(xfw);
		retval = retval0;
	}
	return retval;
}
static inline int
maybe_reject_partial_firmware_into_buf(const struct firmware **firmware_p, const char *name,
				       struct device *device, void *buf, size_t size, size_t offset)
{
	if (is_nonfree_firmware(name))
		return reject_partial_firmware_into_buf(firmware_p, name, device, buf, size, offset);
	else
		return request_partial_firmware_into_buf(firmware_p, name, device, buf, size, offset);
}

#endif /* _LINUX_LIBRE_FIRMWARE_H */

#endif
