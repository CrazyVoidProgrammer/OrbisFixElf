#ifndef _DYNGEN_H_
#define _DYNGEN_H_

#include <stdint.h>
#include <stdbool.h>

//struct orbis_version *generate_sce_version();

//void *generate_sce_process_param(int type);

struct orbis_comment generate_sce_comment(char *path);
void *consolidate_orbis_comment(struct orbis_comment ioc);

void *generate_sce_stack_sizes();

void generate_sce_special();

#endif