import os

TARGET_DIR = "./user/build/stripped/"

import argparse

if __name__ == '__main__':
    f = open("os/link_app.S", mode="w")
    apps = os.listdir(TARGET_DIR)
    apps.sort()
    f.write(
'''
    .align 8
    .section .data
    .global user_apps
user_apps:
'''
    )

    for app in apps:
        size = os.path.getsize(TARGET_DIR + app)
        f.write(f'''
    .quad .str_{app}
    .quad .elf_{app}
    .quad {size}
'''
        )
    
    # in the end, append a NULL structure.
    f.write(
f'''
    .quad 0
    .quad 0
    .quad 0
'''
    )

    # include apps elf file.
    f.write(
'''
    .section .data.apps
'''
    )
    for app in apps:
        f.write(
f'''
.str_{app}:
    .string "{app}"
.align 8
.elf_{app}:
    .incbin "{TARGET_DIR}{app}"
'''
    )
    f.close()