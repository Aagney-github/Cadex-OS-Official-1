/*
Copyright (C) 2019-2020 The CadexOS Project
This software is distributed under the GNU General Public License.
See the file LICENSE for details.
*/

#ifndef KMALLOC_H
#define KMALLOC_H

void *kmalloc(int length);
void kfree(void *ptr);

void kmalloc_init(char *start, int length);
void kmalloc_debug();
int kmalloc_test();

#endif
