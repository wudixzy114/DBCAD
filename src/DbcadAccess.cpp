#include "DbcadAccess.h"

#include "access.hxx"
#include "neo4j.hxx"
#include <mgclient-1.4.2/mgclient.h>
#include "acis/include/kernapi.hxx"
#include "acis/include/part_api.hxx"

extern void api_save_entity_list_neo4j_part(const Neo4jPart& conn, const ENTITY_LIST& entity_list);
extern void api_restore_entity_list_neo4j_part(const Neo4jPart& conn, ENTITY_LIST& entity_list);
extern int64_t count_partnode(const Neo4jPart& conn);
extern void api_save_neo4j(const Neo4jPart& conn);
extern void api_restore_neo4j(const Neo4jPart& conn, int generation_id);
extern int initialize_acis();
extern void terminate_acis(int level);

