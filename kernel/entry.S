	# qemu -kernel loads the kernel at 0x80000000
        # and causes each CPU to jump there.
        # kernel.ld causes the following code to
        # be placed at 0x80000000.
.section .text
.global _entry
_entry:
	# set up a stack for C.
        # stack0 is declared in start.c,
        # with a 4096-byte stack per CPU.
        # sp = stack0 + (hartid * 4096)
        la sp, stack0
        li a0, 1024*4
	csrr a1, mhartid    # 加载当前 CPU 的 ID 到 a1
        addi a1, a1, 1  # 将当前 CPU 的 ID 增加 1
        mul a0, a0, a1
        add sp, sp, a0  # 移动sp到（ID）CPU 的 stack 空间
	# jump to start() in start.c
        call start
spin:
        j spin     # 无限循环
