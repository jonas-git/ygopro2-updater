#include <stdio.h>
#include <string.h>
#include "path.h"

int main(void)
{
    /*struct path_segments segx;
    char parth[] = ".git";
    int ret = path_split_name(&segx, parth);
    printf("%s\n%s\n%s\n", segx.name, segx.ext, ret < 0 ? "false" : "true");
    return 0;*/
    
    char path[] = "./.git/";
    struct path_segments seg;
    path_split(&seg, path);
    printf("%s\n%s\n%s\n", seg.dir, seg.name, seg.ext);
    
    return 0;
}
