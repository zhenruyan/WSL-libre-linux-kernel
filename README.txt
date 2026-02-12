This commit records some information pertaining to the cleaning up in
preparation of a GNU Linux-libre release.

The log file preserves the output of the cleaning up script
deblob-<kver>, while cleaning up this release.

The tar.sign file is a digital signature of the tarball created in
order to verify and publish this release.  The tarball is created with
git archive, using (for historical reasons) the same --prefix as the
upstream release it's based on, i.e., no -libre* nor -gnu*.
