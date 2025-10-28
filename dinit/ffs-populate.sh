#!/bin/sh
# Populate (or clean up) the FunctionFS descriptors/strings expected by
# the cmtp-responder stack.
#
# This script mirrors the behaviour that the systemd unit performed via
# USBFunctionDescriptors=/USBFunctionStrings= directives. It can be
# installed to /usr/libexec/cmtp-responder/ffs-populate.sh (or any other
# suitable path) and invoked by the accompanying Dinit service file.

set -eu

SRC_DESCS=${MTP_FFS_SOURCE_DESCS:-/etc/cmtp-responder/descs}
SRC_STRS=${MTP_FFS_SOURCE_STRS:-/etc/cmtp-responder/strs}
DST=${MTP_FFS_MOUNT:-/run/ffs_mtp}

copy_payload() {
	local source=$1
	local target_name=$2

	if [ -d "$source" ]; then
		# Copy directory contents preserving attributes.
		cp -a "$source"/. "$DST/"
	elif [ -f "$source" ]; then
		cat "$source" >"$DST/$target_name"
	else
		echo "ffs-populate: missing source: $source" >&2
		exit 1
	fi
}

populate() {
	if [ ! -d "$DST" ]; then
		echo "ffs-populate: destination mount $DST not available" >&2
		exit 1
	fi

	copy_payload "$SRC_DESCS" descriptors
	copy_payload "$SRC_STRS" strings
}

cleanup() {
	rm -rf "$DST/descriptors" "$DST/strings"
}

usage() {
	echo "Usage: $0 populate|cleanup" >&2
	exit 2
}

if [ $# -ne 1 ]; then
	usage
fi

case "$1" in
	populate)
		populate
		;;
	cleanup)
		cleanup
		;;
	*)
		usage
		;;
 esac
