/**
 * Cadex CBasic
 * (C) HyperCreeck 2020
 * This software is distributed under the GNU General Public License.
 * See the file LICENSE for details.
*/
#include <library/errno.h>
#include <library/string.h>
#include <library/objno.h>
#include <library/malloc.h>
#include <library/stdio.h>
#include <kernel/ascii.h>
#include <kernel/types.h>
#include <kernel/syscall.h>
#include <kernel/error.h>
#include <kernel/types.h>
#include "kshell.h"
#include "keyboard.h"
#include "console.h"
#include "string.h"
#include "rtc.h"
#include "kmalloc.h"
#include "page.h"
#include "process.h"
#include "main.h"
#include "fs.h"
#include "syscall_handler.h"
#include "clock.h"
#include "kernelcore.h"
#include "bcache.h"
#include "printf.h"
#include "ata.h"
#include "bitmap.h"
#include "scanf.h"

int cbasic();