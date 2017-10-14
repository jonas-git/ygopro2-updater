#include <stdbool.h>
#include <string.h>
#include "path.h"

/* trim_trailing(str, ptr, c):
 |   Indirectly trims trailing characters 'c' from
 |   the string 'str' starting from 'ptr' going backwards
 |   by returning a pointer to the last occurence.
 |   It is expected that '*ptr' equals 'c'
 | begin: The beginning of the string
 | ptr:   A pointer to a character that is part of the string 'begin'
 | c:     Any character that should be trimmed
 */
static
char *trim_trailing(const char *str, const char *ptr, char c)
{
    for (; ptr != str; --ptr)
        if (ptr[-1] != c)
            break;
    return (char *)ptr;
}

bool path_split_name(struct path_segments *seg, char *name)
{
    seg->name = PATH_DEFAULT_NAME;
    seg->ext = PATH_DEFAULT_EXT;
    
    // Invalid file names: "", "." and ".."
    if (!name[0] || (name[0] == '.' &&
            (!name[1] || (name[1] == '.' && !name[2]))))
        return false;
    
    char *last_dot = strrchr(name, '.');
    
    if (!last_dot)
        seg->name = name;
    else {
        if (last_dot[1])
            seg->ext = last_dot + 1;
        if (last_dot != name) {
            seg->name = name;
            *last_dot = 0;
        }
    }
    
    return true;
}

void path_split(struct path_segments *seg, char *path)
{
    if (!path || !*path) {
        seg->dir = PATH_DEFAULT_DIR;
        seg->name = PATH_DEFAULT_NAME;
        seg->ext = PATH_DEFAULT_EXT;
        return;
    }

    char *last_slash = strrchr(path, '/');

    if (last_slash) {
        // If the path ends with a slash then it's definitely a directory.
        bool is_dir = !last_slash[1];

        // Make sure that 'last_slash' points to the last slash
        // that is inside the path string (not at the end).
        if (is_dir && last_slash != path) {
            seg->name = PATH_DEFAULT_NAME;
            seg->ext = PATH_DEFAULT_EXT;

            last_slash = trim_trailing(path, last_slash, '/');

            if (last_slash == path)
                last_slash[1] = 0;
            else {
                *last_slash = 0;
                last_slash = strrchr(path, '/');
            }
        }
        
        // Make sure if this is really
        // not a directory by checking the base name.
        if (!is_dir) {
            // This is the same as 'basename(path)'.
            char *base = last_slash ? last_slash + 1 : path;
            is_dir = !path_split_name(seg, base);
        }

        if (is_dir)
            seg->dir = path;
        else if (last_slash == path)
            seg->dir = PATH_ROOT_DIR;
        else {
            last_slash = trim_trailing(path, last_slash, '/');
            *last_slash = 0;
            seg->dir = path;
        }
    }
    else {
        seg->dir = PATH_DEFAULT_DIR;
        path_split_name(seg, path);
    }
}
