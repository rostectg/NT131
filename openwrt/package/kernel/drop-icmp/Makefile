#
# Copyright (C) 2020 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=drop-icmp
PKG_RELEASE:=1
PKG_LICENSE:=GPL-2.0

PKG_MAINTAINER:=DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD

include $(INCLUDE_DIR)/package.mk

define KernelPackage/drop-icmp
  SECTION:=kernel
  CATEGORY:=Kernel modules
  SUBMENU:=Other modules
  TITLE:=AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
  FILES:=$(PKG_BUILD_DIR)/drop-icmp.ko
endef

define KernelPackage/drop-icmp/description
  aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
endef

define Build/Compile
  $(KERNEL_MAKE) M=$(PKG_BUILD_DIR) modules
endef

$(eval $(call KernelPackage,drop-icmp))