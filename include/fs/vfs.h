#ifndef VFS_H
# define VFS_H

// The idea is to have the most simple vfs as possible,
// which could handle different fs types, and read/write
// just files in it.
// The solution I have found is to have any directory
// management : just a list of mounting points which
// are fake directories, containing just files.
// So every paths will be like that : "/<dir>/<file>".

# define NBMAX_MOUNTINGPOINT 4096

# define DEVICES             0
# define VFILES              1
# define EXT2                2
// [...]

typedef struct mounting_point
{
    const char *name;
    int type;
    void *fs;
} s_mpoint;

s_mpoint vfs[NBMAX_MOUNTINGPOINT];

#endif /* !VFS_H */
