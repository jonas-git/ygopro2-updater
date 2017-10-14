#ifndef PATH_SPLITTER
#define PATH_SPLITTER

#include <stdbool.h>

#define PATH_ROOT_DIR "/"
#define PATH_DEFAULT_DIR "."
#define PATH_DEFAULT_NAME NULL
#define PATH_DEFAULT_EXT NULL

struct path_segments {
    char *dir;
    char *name;
    char *ext;
};

/* path_split_name(seg, name):
 |   Splits a file name into it's name and extension parts.
 |   The return value indicates if the name is valid.
 | seg:  Valid pointer to a [struct path_segments].
 |       Pointers to the beginning of the name and extension
 |       are assigned to the respective fields.
 | name: C-style string that should be free for modification like with
 |       the other 'path_split_*' functions.
 */
bool path_split_name(struct path_segments *seg, char *name);

/* path_split(seg, path):
 |   Splits a path into it's directory, name and extension parts.
 | seg:  Valid pointer to a [struct path_segments].
 |       Pointers to the beginning of the directory, name and
 |       extension are assigned to the respective fields.
 | name: C-style string that should be free for modification.
 |       Some characters are replaced with null-terminators for separation.
 */
void path_split(struct path_segments *seg, char *path);

#endif /* PATH_SPLITTER */
