GNU Linux-libre: <https://www.fsfla.org/selibre/linux-libre/>

This repository contains the GNU Linux-libre release archive.

It is a bit unusual as far as git repositories go, in that it pretty
much does not contain branches, only tags.  The goal is not to reflect
development history, but to preserve our release history in a far more
compact form than a collection of tarballs.

Most tags are initial commits, so that, in case we find releases
containing non-Free Software, we can pull them back without affecting
subsequent releases.

For each release, there are up to three tags:

- The tag under sources has cleaned-up source tarballs.

- The tag under scripts has the cleaning-up scripts, if there was any
change to them since the previous release in the series.

- The tag under logs records the modifications made by the scripts,
and GPG signatures of the verified tarballs.


We do have branches, too.  The branch containing this README is master.

We have branches for the cleaning-up scripts for each release series,
under scripts.


Tags are all (supposed to be) GPG-signed.  Tags dated earlier than May
30, 2020, are signed by Jason Self's key.  Later tags than that are
more likely to be signed with Alex Oliva's GNU Linux-libre key, but do
not be alarmed by new signatures by Jason Self.  Run

  gpg --import keyring.gpg

to be able to verify signatures.
