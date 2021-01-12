# Project 3: Hook Systemcall
## Cách sử dụng
- Chọn folder chưa code của chương trình -> click phải chuột chọn Open In Terminal.
- Gõ make: Tiến trình make sẽ dựa vào Makefile để biên dịch ra mã nguồn, tạo ra kernel module.
- Sau khi biên dịch xong, gõ ls, ta sẽ thấy xuất hiện môt file có tên mở rộng là .ko (hook.ko). Đây chính là kernel module.
- Để lắp module vào trong kernel, ta gõ sudo insmod hook.ko. Trong suốt quá trình, ta sẽ dùng lệnh dmesg để theo dõi quá trình hoạt động của module.
- Sau cùng, gõ sudo rmmod hook.ko để tháo module ra khỏi kernel.

## Giải thích mã nguồn
```
asmlinkage int (*open) (const char*, int, umode_t);      
```
- Đây là hàm thay thế system call open.
```
asmlinkage ssize_t (*write) (unsigned int, const char*, size_t); 
```
- Đây là hàm thay thế system call write.

