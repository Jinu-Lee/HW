
#include "cache.h"
#include "spim.h"
#include "run.h"

typedef enum { INST, DATA } Type;
typedef enum { LRU, FIFO } RP;
typedef enum { WT, WB } WP;

typedef struct {
	int valid, dirty, tag;
	int* data;
} Block;

typedef struct {
	Block** table;
	int** access;
	int n_data, n_entries, n_ways, hit_time;
	int hit_count, miss_count;
	Type type;
	RP rp;
	WP wp;
} Cache;

int flag_init, level, hit_time_mem;
int s_flag[2], s_addr[2], s_way[2], s_index[2];
Cache l1_inst, l1_data, l2;

typedef int32 reg_word;
extern reg_word read_mem_word(mem_addr addr);

void __init__(Cache* cache, Type type, int size, int n_entries, int n_ways, char* rp, char* wp, int hit_time) {
	cache->type = type;
	cache->n_entries = n_entries;
	cache->n_ways = n_ways;
	cache->hit_time = hit_time;
	cache->hit_count = 0;
	cache->miss_count = 0;
	cache->n_data = size / (n_entries * n_ways * sizeof(int));

	if (strcmp(rp, "LRU") == 0)	cache->rp = LRU;
	else cache->rp = FIFO;

	if (strcmp(wp, "WT") == 0) cache->wp = WT;
	else cache->wp = WB;

	cache->table = (Block**) malloc(n_ways * sizeof(Block*));
	for (int i = 0; i < n_ways; i++) {
		cache->table[i] = (Block*) calloc(n_entries, sizeof(Block));
		for (int j = 0; j < n_entries; j++) 
			cache->table[i][j].data = (int*) calloc(cache->n_ways, sizeof(int));
	}

	cache->access = (int**) malloc(n_entries * sizeof(int*));
	for (int i = 0; i < n_entries; i++) 
		cache->access[i] = (int*) calloc(n_ways, sizeof(int));	
}

void __clear__(Cache* cache) {
	for (int i = 0; i < cache->n_ways; i++) {
		for (int j = 0; j < cache->n_entries; j++) {
			cache->table[i][j].valid = 0;
			cache->table[i][j].dirty = 0;
			cache->table[i][j].tag = 0;
			for (int k = 0; k < cache->n_data; k++)
				cache->table[i][j].data[k] = 0;
		}
	}

	for (int i = 0; i < cache->n_entries; i++) 
		for (int j = 0; j < cache->n_ways; j++)
			cache->access[i][j] = 0;
}

void __free__(Cache* cache) {
	for (int i = 0; i < cache->n_ways; i++) {
		for (int j = 0; j < cache->n_entries; j++) 
			free(cache->table[i][j].data);
		free(cache->table[i]);
	}
	free(cache->table);

	for (int i = 0; i < cache->n_entries; i++)
		free(cache->access[i]);
	free(cache->access);
}

void init() {
	int size, n_entries, n_ways, hit_time;
	char rp[10], wp[10];
	FILE* fp = fopen("../CPU/cache.config", "r");

	fscanf(fp, "%d %d", &level, &hit_time_mem);

	/* L1 Cache */
	fscanf(fp, "%d %d %d %s %s %d", &size, &n_entries, &n_ways, rp, wp, &hit_time);
	__init__(&l1_inst, INST, size, n_entries, n_ways, rp, wp, hit_time);
	__init__(&l1_data, DATA, size, n_entries, n_ways, rp, wp, hit_time);

	if (level == 2) {
		/* L2 Cache */
		fscanf(fp, "%d %d %d %s %s %d", &size, &n_entries, &n_ways, rp, wp, &hit_time);
		__init__(&l2, DATA, size, n_entries, n_ways, rp, wp, hit_time);
	}

	fclose(fp);
	flag_init = 1;
}
 
void clear() {
	__clear__(&l1_inst);
	__clear__(&l1_data);
	if (level == 2) __clear__(&l2);
}

int log2(int x) {
	int ret = 0;
	while (x > 1) {
		x >>= 1;
		ret++;
	}
	return ret;
}

int get_block_offset(int n_data, unsigned int addr) {
	return (addr >> 2) & ((1 << log2(n_data)) - 1);
}

int get_tag(int n_entries, int n_data, unsigned int addr) {
	return addr >> (2 + log2(n_data) + log2(n_entries));
}

int get_index(int n_entries, int n_data, unsigned int addr) {
	return (addr >> (2 + log2(n_data))) & ((1 << log2(n_entries)) - 1);
}

int search(Cache* cache, unsigned int addr, int* r_way, int* r_index) {
	int tag = get_tag(cache->n_entries, cache->n_data, addr);
	int index = get_index(cache->n_entries, cache->n_data, addr);
	int way, n_ways = cache->n_ways;

	for (way = 0; way < n_ways; way++) 
		if (cache->table[way][index].tag == tag && cache->table[way][index].valid) break;
	
	/* miss */
	if (way == n_ways) return 0;
	
	/* hit */
	if (r_way != NULL) *r_way = way;
	if (r_index != NULL) *r_index = index;
	return 1;
}

int add(Cache* cache, unsigned int addr, int* r_way, int* r_index) {
	int tag = get_tag(cache->n_entries, cache->n_data, addr);
	int index = get_index(cache->n_entries, cache->n_data, addr);
	int block_offset = get_block_offset(cache->n_data, addr);
	int way, n_ways = cache->n_ways, n_stall = 0;

	for (way = 0; way < n_ways; way++) 
		if (cache->table[way][index].valid == 0) break;

	/* set full */
	if (way == n_ways) {
		for (int i = 0; i < n_ways; i++) {
			if (cache->access[index][i] == 1) way = i;
			else cache->access[index][i]--;
		}
		cache->access[index][way] = n_ways;
		if (cache->wp == WB && cache->table[way][index].dirty) n_stall = hit_time_mem; 
	}
	/* not full */
	else {
		for (int i = 0; i < n_ways; i++) {
			if (cache->access[index][i] == 0) {
				cache->access[index][i] = (way = i) + 1; 
				break;
			}
		}
	}
	
	cache->table[way][index].valid = 1;
	cache->table[way][index].dirty = 0;
	cache->table[way][index].tag = tag;
	for (int i = 0; i < cache->n_data; i++) 
		cache->table[way][index].data[i] = read_mem_word(addr + 4 * (i - block_offset));

	if (r_way != NULL) *r_way = way;
	if (r_index != NULL) *r_index = index;

	return n_stall;
}

void update(Cache* cache, unsigned int addr, int way, int index) {
	int block_offset = get_block_offset(cache->n_data, addr);
	int n_ways = cache->n_ways, access;

	if (cache->rp == LRU) {
		access = cache->access[index][way];
		for (int i = 0; i < n_ways; i++) 
			if (cache->access[index][i] < access) 
				cache->access[index][i]++;
		cache->access[index][way] = 1;
	}

	if (cache->wp == WB) cache->table[way][index].dirty = 1;
	cache->table[way][index].data[block_offset] = read_mem_word(addr);
}

void sw() {
    /* L1 cache */
    if (s_flag[0]) {
        update(&l1_data, s_addr[0], s_way[0], s_index[0]);
        s_flag[0] = 0;
    }
    /* L2 cache */
    if (s_flag[1]) {
        update(&l2, s_addr[1], s_way[1], s_index[1]);
        s_flag[1] = 0;
    }
}

void set_sw(int cache, int addr, int way, int index) {
    s_flag[cache] = 1;
    s_addr[cache] = addr;
    s_way[cache] = way;
    s_index[cache] = index;
}

int load_from_mem(Cache* cache, unsigned int addr) {
	if (!flag_init) return 0;
    if (s_flag[0] || s_flag[1]) sw();

	int way, index, n_stall = 0;
	/* Found in L1 Cache */
	if (search(cache, addr, &way, &index)) {
		cache->hit_count++;
		update(cache, addr, way, index);
		return n_stall;
	}
	
	cache->miss_count++;
	if (level == 1) {
		n_stall += hit_time_mem;
		n_stall += add(cache, addr, NULL, NULL);
		return n_stall;
	}

	n_stall += l2.hit_time;
	/* Found in L2 Cache */
	if (search(&l2, addr, &way, &index)) {
		l2.hit_count++;
		n_stall += add(cache, addr, NULL, NULL);
		update(&l2, addr, way, index);
	}
	/* Not found in L2 Cache */
	else {
		l2.miss_count++;
		n_stall += hit_time_mem;
		n_stall += add(cache, addr, NULL, NULL);
		n_stall += add(&l2, addr, NULL, NULL);
	}

	return n_stall;
}

int data_load (unsigned int addr) {
	return load_from_mem(&l1_data, addr);		// Return value: stall cycles due to L1 cache miss
	///////////////////////////////////////////////////////////
}

int instruction_load (unsigned int addr) {
	if (addr == 0x400024) init();
	return load_from_mem(&l1_inst, addr);		// Return value: stall cycles due to L1 cache miss
	///////////////////////////////////////////////////////////
}

int data_store(unsigned int addr) {
	/* You have to implement your own data_store function here! */
	if (!flag_init) return 0;
    if (s_flag[0] || s_flag[1]) sw();

	int way[2], index[2], n_stall = 0;
	/* Found in L1 Cache */
	if (search(&l1_data, addr, &way[0], &index[0])) {
		l1_data.hit_count++;
        set_sw(0, addr, way[0], index[0]);
		return 0;
	}
	
	l1_data.miss_count++;
	if (level == 1) {
		n_stall += hit_time_mem;
		n_stall += add(&l1_data, addr, &way[0], &index[0]);
		set_sw(0, addr, way[0], index[0]);
		return n_stall;
	}

	n_stall += l2.hit_time;
	/* Found in L2 Cache */
	if (search(&l2, addr, &way[1], &index[1])) {
		l2.hit_count++;
		n_stall += add(&l1_data, addr, &way[0], &index[0]);
        set_sw(1, addr, way[1], index[1]);
	}
	/* Not found in L2 Cache */
	else {
		l2.miss_count++;
		n_stall += hit_time_mem;
		n_stall += add(&l1_data, addr, &way[0], &index[0]);
		n_stall += add(&l2, addr, &way[1], &index[1]);

		if (l1_data.wp == WT) {
            n_stall += l2.hit_time;
            set_sw(1, addr, way[1], index[1]);
        }
	}

    set_sw(0, addr, way[0], index[0]);

	return n_stall;		// Return value: stall cycles due to L1 cache miss
	///////////////////////////////////////////////////////////
}

void print_cache_result(int n_cycles) {
	/* You have to print the result of hit/miss count of each cache. You have to follow the format as below example.
	Calculate hit ratio down to three places of decimals.
	Example)
	Level 1 Cache
	Hit Count of l1i-cache: 4
	Miss Count of l1i-cache: 5
	Hit Ratio of l1i-cache: 0.444

	Hit Count of l1d-cache: 1
	Miss Count of l1d-cache: 3
	Hit Ratio of l1d-cache: 0.250

	Level 2 Cache
	Hit Count: 0
	Miss Count: 8
	Hit Ratio: 0.000

	Total Hit Ratio: 0.385
	*/

	/* You have to implement your own print_cache_result function here! */

	float ratio;

	printf("\nLevel 1 Cache\n");
	printf("Hit Count of l1i-cache: %d\n", l1_inst.hit_count);
	printf("Miss Count of l1i-cache: %d\n", l1_inst.miss_count);
	ratio = (float) l1_inst.hit_count / (l1_inst.hit_count + l1_inst.miss_count);
	printf("Hit Ratio of l1i-cache: %.3f\n\n", ratio);

	printf("Hit Count of l1d-cache: %d\n", l1_data.hit_count);
	printf("Miss Count of l1d-cache: %d\n", l1_data.miss_count);
	ratio = (float) l1_data.hit_count / (l1_data.hit_count + l1_data.miss_count);
	printf("Hit Ratio of l1d-cache: %.3f\n\n", ratio);

	ratio = ((float) (l1_inst.hit_count + l1_data.hit_count)) / (l1_inst.hit_count + l1_inst.miss_count + l1_data.hit_count + l1_data.miss_count);

	if (level == 2) {
		printf("Level 2 Cache\n");
		printf("Hit Count: %d\n", l2.hit_count);
		printf("Miss Count: %d\n", l2.miss_count);
		ratio = (float)l2.hit_count / (l2.hit_count + l2.miss_count);
		printf("Hit Ratio: %.3f\n\n", ratio);
		ratio = ((float) (l1_inst.hit_count + l1_data.hit_count + l2.hit_count)) / (l1_inst.hit_count + l1_inst.miss_count + l1_data.hit_count + l1_data.miss_count + l2.hit_count + l2.miss_count);
	}

	printf("Total Hit Ratio: %.3f\n", ratio);

	__free__(&l1_inst);
	__free__(&l1_data);
	if (level == 2) __free__(&l2);

	//////////////////////////////////////////////////////////////////////
}
