/**
 * Copyright (C) 2019-2020 The CadexOS Project
 * This software is distributed under the GNU General Public License
 * See the file LICENSE for details
 */

/**
 * Version info about Cadex OS
 */

#define COS_SOURCE // Define that this is Cadex-OS source

#define _LTS_REL_ "0.2.0" // LTS release version; NULL if the version is not
#define _K_VER_FMT \
    "%d.%d.%d-%s" // This should be printable using the printf() function
#define __KSHELL_VER_ "1.15.1" // The Kernel shell version
#define __VER_SUFFIX \
    "release" // Version suffix; Valid values are: beta, prerelease, prebeta,
              // alpha, prealpha
#define _K_ARCH     "i686"                  // Kernel architecture
#define _K_CODENAME "Gorgeous Gastrotricha" // Kernel Codename
#define __K_BUILD_DATE \
    __DATE__           \
        ? __DATE__     \
        : "Not available" // The date when this kernel was built from source
#define __K_BUILD_TIME \
    __TIME__           \
        ? __TIME__     \
        : "Not available" // The time when this kernel was built from source
