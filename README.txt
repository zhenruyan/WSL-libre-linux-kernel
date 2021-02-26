This commit records some information pertaining to the cleaning up in
preparation of a GNU Linux-libre release.

The log file preserves the output of the cleaning up script
deblob-<kver>, while cleaning up this release.

The tar.sign file is a digital signature of the decompressed tarball
created in order to verify and publish this release.  It is created
with git archive, using the same --prefix as the upstream release it's
based on, i.e., no -libre* nor -gnu*.

The git.tar.sign file is a digital signature of a decompressed tarball
created with git archive.  See the comments in it for information on
how to recreate the tarball so as to verify the signature.
