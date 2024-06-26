# Tổng quan
## Firewall
...firewall là gì bla bla... nhằm mục đích bla bla...
## IPTables & NetFilter
**Netfilter** là một framework trong **Linux kernel**. Nhiệm vụ của nó là lọc gói tin (packet filtering), thực hiện NAT / PAT, và các thao tác trên gói tin khác.
Vì Netfilter là một hoạt động ở tầng kernel, đòi hỏi phải có một phần mềm "frontend" cho người dùng sử dụng. 
**IPTables** chính là giao diện để người dùng có thể tương tác với Netfilter. IPTables hoạt động ở user-space, sử dụng thư viện *libiptc* để giao tiếp với Netfilter.

# Chi tiết kĩ thuật
## Netfilter Hooks
Có 5 **hook** trong framework Netfilter mà một **kernel module** có thể "đăng kí" vào.
Khi một gói tin đi vào networking stack, tùy theo các thông số (in/out, source/dest,..) của gói tin mà hệ thống sẽ xử lý và kích hoạt **hook**, đẩy thông tin và quyền xử lý cho các module đã đăng kí vào **hook** đó.
Các **hook** bao gồm:
- `NF_IP_PRE_ROUTING` - Sẽ kích hoạt sớm ngay khi gói tin đi vào hệ thống, trước tất cả các bước routing sẽ diễn ra.
- `NF_IP_LOCAL_IN` - Sẽ kích hoạt sau khi routing, và khi đích đến của gói tin chính là hệ thống hiện tại (localhost)
- `NF_IP_FORWARD` - Sẽ kích hoạt sau khi routing, và khi đích đến của gói tin là hệ thống khác, sẽ được chuyển tiếp đi.
- `NF_IP_LOCAL_OUT` - Sẽ kích hoạt khi gói tin được gửi đi từ chính hệ thống cục bộ hiện tại (localhost), ngay khi gói tin đi vào network stack.
- `NF_IP_POST_ROUTING` - Sẽ kích hoạt khi gói tin chuẩn bị được gửi ra khỏi hệ thống.

## IPTables + Chains
Trong IPTables, các **rule** sẽ được sắp xếp gán vào các **table** (bảng). Các table này mục đích là để phân loại rule theo *mục đích*.
Ví dụ - Các rule được viết với mục đích allow hoặc block gói tin, thường sẽ được xếp vào table `filter`. Ngoài ra có các table khác như: `NAT`, `mangle`, `security`,...
Trong mỗi **table**, các **rule** sẽ được tiếp tục được chia ra thành nhiều **chain** riêng. Các **chain** này sẽ thể hiện **netfilter hook** nào sẽ kích hoạt **rule**. Do đó, tên các chain cũng khá tương đồng với tên các netfilter hook:
- `PREROUTING`: Tương ứng với hook `NF_IP_PRE_ROUTING`.
- `INPUT`: Tương ứng với hook `NF_IP_LOCAL_IN`.
- `FORWARD`: Tương ứng với hook `NF_IP_FORWARD`.
- `OUTPUT`: Tương ứng với hook `NF_IP_LOCAL_OUT`.
- `POSTROUTING`: Tương ứng với hook `NF_IP_POST_ROUTING`.
Nhờ sử dụng các chain này, sysadmin có thể kiểm soát được *khi nào* rule sẽ được áp dụng lên gói tin.
Như vậy IPTables là giao diện người dùng, cho phép ta có thể *đăng kí* vào các netfilter hook chạy ở kernel, để tương tác với các gói tin đi qua hệ thống.




# Xây dựng đồ án môn học
## 1. Kernel module
**Mục tiêu:** Xây dựng một kernel module đơn giản, tương tác với Netfilter để thông qua đó block một vài gói tin.

**Chuẩn bị:**
Build toolings:
```bash
sudo apt install bc binutils bison dwarves flex gcc git gnupg2 gzip libelf-dev libncurses5-dev libssl-dev make openssl pahole perl-base rsync tar xz-utils
```

"KBuild" Makefile
```sh
obj-m += module_name.o
PWD := $(CURDIR)
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
```


## 2. Linux kernel
**Mục tiêu:** Chỉnh sửa trực tiếp source code linux kernel, build lại kernel rồi boot thành công.

**Chuẩn bị:**
1. Config file:
```bash
cp /boot/config-$(uname -r) .config
make olddefconfig
```
%%
2. Disable module signing
```bash
./scripts/config --file .config --disable MODULE_SIG
./scripts/config --file .config --disable SYSTEM_TRUSTED_KEYS
./scripts/config --file .config --disable SYSTEM_REVOCATION_KEYS
```
%%
3. Tag tên cho kernel sắp build
```bash
./scripts/config --file .config --set-str LOCALVERSION "-custom-nt131"
```

4. Build kernel
```bash
make -j$(nproc) 2>&1 | tee log
```

5. Cài các module bắt buộc
```
sudo make modules_install
```

7. Cài đặt kernel
```
sudo make install
```

./scripts/config --file .config --set-str LOCALVERSION "-pratham"

---
Load kernel module
```
insmod module_name.ko
```

Unload kernel module
```
rmmod module_name
```


---

### Lỗi thường gặp
```
$ dmesg
module: x86/modules: Skipping invalid relocation target, existing value is nonzero for type 1, loc 00000000f2e333ea, val ffffffffc09f9000
```
=> Gỡ & cài lại linux headers
```sh
sudo apt update && sudo apt upgrade
sudo apt remove --purge linux-headers-*
sudo apt autoremove && sudo apt autoclean
sudo apt install linux-headers-`uname -r`
```


# Demo





# Nguồn
- https://www.digitalocean.com/community/tutorials/a-deep-dive-into-iptables-and-netfilter-architecture
- https://blogs.oracle.com/linux/post/introduction-to-netfilter
- https://www.netfilter.org/documentation/HOWTO/
- https://en.wikipedia.org/wiki/Netfilter
- https://github.com/konstantin89/linux-kernel-netfilter

- https://web.archive.org/web/20160315063859/http://www.paulkiddie.com/2009/10/creating-a-simple-hello-world-netfilter-module/
- https://web.archive.org/web/20160315000231/http://www.paulkiddie.com/2009/11/creating-a-netfilter-kernel-module-which-filters-udp-packets/
- https://github.com/wangm8/Netfilter-Kernel-Module
- https://github.com/konstantin89/linux-kernel-netfilter
- https://medium.com/@GoldenOak/linux-kernel-communication-part-1-netfilter-hooks-15c07a5a5c4e
- "Write a Linux firewall from scratch based on Netfilter" by ChrisBao
	- https://organicprogrammer.com/2022/05/04/how-to-write-a-netfilter-firewall-part1/
	- https://organicprogrammer.com/2022/05/05/how-to-write-a-netfilter-firewall-part2/
	- https://organicprogrammer.com/2022/06/08/how-to-write-a-netfilter-firewall-part3/
- https://itsfoss.com/compile-linux-kernel/