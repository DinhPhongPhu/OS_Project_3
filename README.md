# Project 3: Hook Systemcall
## Cách sử dụng
- Chọn folder chưa code của chương trình -> click phải chuột chọn Open In Terminal.
- Gõ make: Tiến trình make sẽ dựa vào Makefile để biên dịch ra mã nguồn, tạo ra kernel module.
- Sau khi biên dịch xong, gõ ls, ta sẽ thấy xuất hiện môt file có tên mở rộng là .ko (hook.ko). Đây chính là kernel module.
- Để lắp module vào trong kernel, ta gõ sudo insmod hook.ko. Trong suốt quá trình, ta sẽ dùng lệnh dmesg để theo dõi quá trình hoạt động của module.
- Sau cùng, gõ sudo rmmod Random.ko để tháo module ra khỏi kernel.

## Giải thích mã nguồn
```
asmlinkage int new_open(const char* filename, int flags, umode_t mode)
{
    if(strstr(filename,"in.txt")!=NULL){
        printk(KERN_INFO "New write syscall\n");
        printk(KERN_INFO "Calling process: %s\n",current->comm);
        printk(KERN_INFO "Opend file name: %s\n", filename);
    }
    return open(filename, flags, mode);
}
```
- Đây là hàm thay thế system call open

