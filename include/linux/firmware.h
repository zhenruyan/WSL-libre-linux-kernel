/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_FIRMWARE_H
#define _LINUX_FIRMWARE_H

#include <linux/types.h>
#include <linux/compiler.h>
#include <linux/gfp.h>

#define FW_ACTION_NOUEVENT 0
#define FW_ACTION_UEVENT 1

struct firmware {
	size_t size;
	const u8 *data;

	/* firmware loader private fields */
	void *priv;
};

/**
 * enum fw_upload_err - firmware upload error codes
 * @FW_UPLOAD_ERR_NONE: returned to indicate success
 * @FW_UPLOAD_ERR_HW_ERROR: error signalled by hardware, see kernel log
 * @FW_UPLOAD_ERR_TIMEOUT: SW timed out on handshake with HW/firmware
 * @FW_UPLOAD_ERR_CANCELED: upload was cancelled by the user
 * @FW_UPLOAD_ERR_BUSY: there is an upload operation already in progress
 * @FW_UPLOAD_ERR_INVALID_SIZE: invalid firmware image size
 * @FW_UPLOAD_ERR_RW_ERROR: read or write to HW failed, see kernel log
 * @FW_UPLOAD_ERR_WEAROUT: FLASH device is approaching wear-out, wait & retry
 * @FW_UPLOAD_ERR_MAX: Maximum error code marker
 */
enum fw_upload_err {
	FW_UPLOAD_ERR_NONE,
	FW_UPLOAD_ERR_HW_ERROR,
	FW_UPLOAD_ERR_TIMEOUT,
	FW_UPLOAD_ERR_CANCELED,
	FW_UPLOAD_ERR_BUSY,
	FW_UPLOAD_ERR_INVALID_SIZE,
	FW_UPLOAD_ERR_RW_ERROR,
	FW_UPLOAD_ERR_WEAROUT,
	FW_UPLOAD_ERR_MAX
};

struct fw_upload {
	void *dd_handle; /* reference to parent driver */
	void *priv;	 /* firmware loader private fields */
};

/**
 * struct fw_upload_ops - device specific operations to support firmware upload
 * @prepare:		  Required: Prepare secure update
 * @write:		  Required: The write() op receives the remaining
 *			  size to be written and must return the actual
 *			  size written or a negative error code. The write()
 *			  op will be called repeatedly until all data is
 *			  written.
 * @poll_complete:	  Required: Check for the completion of the
 *			  HW authentication/programming process.
 * @cancel:		  Required: Request cancellation of update. This op
 *			  is called from the context of a different kernel
 *			  thread, so race conditions need to be considered.
 * @cleanup:		  Optional: Complements the prepare()
 *			  function and is called at the completion
 *			  of the update, on success or failure, if the
 *			  prepare function succeeded.
 */
struct fw_upload_ops {
	enum fw_upload_err (*prepare)(struct fw_upload *fw_upload,
				      const u8 *data, u32 size);
	enum fw_upload_err (*write)(struct fw_upload *fw_upload,
				    const u8 *data, u32 offset,
				    u32 size, u32 *written);
	enum fw_upload_err (*poll_complete)(struct fw_upload *fw_upload);
	void (*cancel)(struct fw_upload *fw_upload);
	void (*cleanup)(struct fw_upload *fw_upload);
};

struct module;
struct device;

/*
 * Built-in firmware functionality is only available if FW_LOADER=y, but not
 * FW_LOADER=m
 */
#ifdef CONFIG_FW_LOADER
bool firmware_request_builtin(struct firmware *fw, const char *name);
#else
static inline bool firmware_request_builtin(struct firmware *fw,
					    const char *name)
{
	return false;
}
#endif

#if IS_REACHABLE(CONFIG_FW_LOADER)
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

#ifdef CONFIG_FW_UPLOAD

struct fw_upload *
firmware_upload_register(struct module *module, struct device *parent,
			 const char *name, const struct fw_upload_ops *ops,
			 void *dd_handle);
void firmware_upload_unregister(struct fw_upload *fw_upload);

#else

static inline struct fw_upload *
firmware_upload_register(struct module *module, struct device *parent,
			 const char *name, const struct fw_upload_ops *ops,
			 void *dd_handle)
{
		return ERR_PTR(-EINVAL);
}

static inline void firmware_upload_unregister(struct fw_upload *fw_upload)
{
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
static inline bool
firmware_reject_builtin(struct firmware *fw, const char *name)
{
	return false;
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
