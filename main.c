#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <jansson.h>

#include "path.h"

struct config_file {
    char *src_str;
    size_t src_size;
    struct path_segments src;
    char *dest_dir;
    size_t dest_size;
    bool update;
};

struct config_repo {
    char *url;
    size_t url_size;
    struct config_file *files;
    size_t files_size;
};

struct config {
    struct config_repo *repos;
    size_t repos_size;
};

struct config *config_open(const char *filename);
void config_free(struct config *config);

struct config *config_open(const char *filename)
{
    FILE *stream = fopen(filename, "rb");
    if (!stream) {
        fprintf(stderr, "config error: "
            "could not open configuration file %s: "
            "No such file or directory\n", filename);
        goto err_fopen;
    }

    json_error_t error;
    json_t *root = json_loadf(stream, 0, &error);
    fclose(stream);

    if (!root) {
        if (error.line < 0) {
            fprintf(stderr, "config error: %s\n", error.text);
            goto err_load;
        }
        fprintf(stderr, "syntax error: %s:%i:%i: %s\n",
            filename, error.line, error.column, error.text);
        goto err_syntax;
    }

    struct config *config = malloc(sizeof(struct config));

    if (!json_is_array(root)) {
        fprintf(stderr, "config error: the root is not an array\n");
        goto err_type1;
    }

    config->repos_size = json_array_size(root);
    config->repos = malloc(config->repos_size * sizeof(struct config_repo));

    size_t i, j;
    for (i = 0; i < config->repos_size; ++i) {
        struct config_repo *curr_repo = config->repos + i;

        json_t *repo = json_array_get(root, i);
        if (!json_is_object(repo)) {
            fprintf(stderr, "config error: root#%zu "
                "is not an object\n", i + 1);
            goto err_type2;
        }

        json_t *url = json_object_get(repo, "url");
        if (!json_is_string(url)) {
            fprintf(stderr, "config error: root#%zu.url "
                "is not a string\n", i + 1);
            goto err_type2;
        }

        json_t *files = json_object_get(repo, "files");
        if (!json_is_array(files)) {
            fprintf(stderr, "config error: root#%zu.files "
                "is not an array\n", i + 1);
            goto err_type2;
        }

        curr_repo->url_size = json_string_length(url);
        curr_repo->url = malloc((curr_repo->url_size + 1) * sizeof(char));
        strcpy(curr_repo->url, json_string_value(url));

        curr_repo->files_size = json_array_size(files);
        curr_repo->files = malloc(curr_repo->files_size *
            sizeof(struct config_file));

        for (j = 0; j < curr_repo->files_size; ++j) {
            struct config_file *curr_file = curr_repo->files + j;

            json_t *file = json_array_get(files, j);
            if (!json_is_object(file)) {
                fprintf(stderr, "config error: root#%zu.file#%zu "
                    "is not an object\n", i + 1, j + 1);
                goto err_type3;
            }

            json_t *src = json_object_get(file, "src");
            if (!json_is_string(src)) {
                fprintf(stderr, "config error: root#%zu.file#%zu.src "
                    "is not a string\n", i + 1, j + 1);
                goto err_type3;
            }

            json_t *dest = json_object_get(file, "dest");
            if (!json_is_string(dest)) {
                fprintf(stderr, "config error: root#%zu.file#%zu.dest "
                    "is not a string\n", i + 1, j + 1);
                goto err_type3;
            }

            json_t *update = json_object_get(file, "update");
            if (!json_is_boolean(update)) {
                fprintf(stderr, "config error: root#%zu.file#%zu.update "
                    "is not a boolean value\n", i + 1, j + 1);
                goto err_type3;
            }

            curr_file->src_size = json_string_length(src);
            curr_file->src_str = malloc((curr_file->src_size + 1) *
                sizeof(char));

            curr_file->dest_size = json_string_length(dest);
            curr_file->dest_dir = malloc((curr_file->dest_size + 1) *
                sizeof(char));

            strcpy(curr_file->src_str, json_string_value(src));
            strcpy(curr_file->dest_dir, json_string_value(dest));

            path_split(&curr_file->src, curr_file->src_str);
            curr_file->update = json_boolean_value(update);
        }
    }

    json_decref(root);
    return config;

err_type3:
    config->repos[i++].files_size = j;
err_type2:
    config->repos_size = i;
    config_free(config);
    config = NULL;
err_type1:
    free(config);
err_syntax:
err_load:
    json_decref(root);
err_fopen:
    return NULL;
}

void config_free(struct config *config)
{
    for (size_t i = 0; i < config->repos_size; ++i) {
        struct config_repo *repo = config->repos + i;
        for (size_t j = 0; j < repo->files_size; ++j) {
            struct config_file *file = repo->files + j;
            free(file->src_str);
            free(file->dest_dir);
        }

        free(repo->files);
        free(repo->url);
    }

    free(config->repos);
    free(config);
}

int main(void)
{
    struct config *conf = config_open("./conf.json");
    if (!conf) 
        return 1;

    for (size_t i = 0; i < conf->repos_size; ++i) {
        struct config_repo *repo = conf->repos + i;

        printf("%s {\n", repo->url);
        for (size_t j = 0; j < repo->files_size; ++j) {
            struct config_file *file = repo->files + j;
            printf("\tGitHub: %s %s %s\n",
                file->src.dir, file->src.name, file->src.ext);
            printf("\tLocal:  %s\n", file->dest_dir);
            printf("\tUpdate: %s\n", file->update ? "yes" : "no");
        }
        printf("}\n");
    }

    config_free(conf);
    return 0;
}
