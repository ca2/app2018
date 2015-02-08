#include "md4/md4_dgst.c"
#include "md4/md4_one.c"
#include "md5/md5_one.c"
#include "md5/md5_dgst.c"
#include "mdc2/mdc2dgst.c"
#include "mdc2/mdc2_one.c"
#include "ripemd/rmd_dgst.c"
#include "ripemd/rmd_one.c"
#include "sha/sha_dgst.c"
#include "sha/sha_one.c"
#include "sha/sha1_one.c"
#include "sha/sha1dgst.c"
#include "sha/sha256.c"
#include "sha/sha512.c"
#if (!defined(WIN32) || !defined(_M_AMD64)) && !defined(_LP64)
#include "whrlpool/wp_block.c"
#endif
#include "whrlpool/wp_dgst.c"

