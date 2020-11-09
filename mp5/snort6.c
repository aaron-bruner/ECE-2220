/* snort6.c template
 * Aaron Bruner
 * ajbrune
 * ECE 2220, Fall 2020
 * MP6
 *
 * Purpose: Generate input for MP6 that simulates the behavior of snort
 *
 * Assumptions: Student must complete generation of randomized
 * data in the records as defined in the MP6.pdf specification.
 *
 * Command line argument:
 *    number of items
 *    generate port attack
 *    seed for random number generator (optional)
 *
 * Output:
 *    binary data to stdout.  The first 4 bytes are the number of
 *    records, and the remaining binary data are the structures
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#define DEST_POS 0
#define SRC_POS  1
#define DEST_IP_MIN 2189413888
#define DEST_IP_MAX 2189414143
#define SRC_IP_MIN    16777216
#define SRC_IP_MAX  3758096383
#define NUMBER_PORTS 65536
#define NUMLTRS 8

struct Record {
    int seqnum;
    float threat;
    unsigned int addrs[2];
    unsigned short int ports[2];
    char dns_name[NUMLTRS];
};

/* builds a random permutation of the numbers 1 through n.
 * Use Knuth shuffle to create the random permutation.
 *
 * This function is complete and should not be changed.
 *
 * Output:  a malloced array of ints
 *    the user must free the array
 */
int *build_random(int nodes)
{
    int *narray;
    int i, key, temp;
    narray = (int *) malloc(nodes*sizeof(int));
    for (i = 0; i<nodes; i++)
        narray[i] = i+1;
    for (i = 0; i<nodes; i++) {
        key = (int) (drand48() * (nodes - i)) + i;
        assert(i <= key && key < nodes);
        temp = narray[i]; narray[i] = narray[key]; narray[key] = temp;
    }
    return narray;
}
// These aren't the droids you are looking for
void embed_secret_attack(struct Record *rec_ptr, int num_records);

// Most of main is complete.  You just need to add the part to
// generate additional random data.
int main(int argc, char *argv[])
{
    int num_records = -1;
    int port_attack = -1;
    int seed = time(NULL);

    if (argc != 3 && argc != 4) {
        fprintf(stderr, "Usage: snort6 num-records port-attack [seed]\n");
        exit(1);
    }
    num_records = atoi(argv[1]);
    if (num_records < 2) {
        fprintf(stderr, "snort6: Invalid number records: %d\n", num_records);
        fprintf(stderr, "Usage: snort6 num-records port-attack [seed]\n");
        exit(1);
    }
    port_attack = atoi(argv[2]);
    if (port_attack != 0 && port_attack != 1) {
        fprintf(stderr, "snort6: Invalid port attack (0 no 1 yes): %d\n", port_attack);
        fprintf(stderr, "Usage: snort6 num-records port-attack [seed]\n");
        exit(1);
    }
    // if seed included use it, otherwise use time as seed
    if (argc == 4) {
        int new_seed = -1;
        new_seed = atoi(argv[3]);
        if (new_seed < 1) {
            fprintf(stderr, "snort6: Invalid seed: %d\n", new_seed);
            exit(1);
        }
        seed = new_seed;
    }
    srand48(seed);

    int i, *rand_seqnums;
    struct Record *rec_ptr;
    rec_ptr = (struct Record *) malloc(num_records * sizeof(struct Record));

    rand_seqnums = build_random(num_records);

    for(i = 0; i < num_records; i++) {
        // sets the seqnum to a random permutation from 1 to num_records
        rec_ptr[i].seqnum = rand_seqnums[i];

        // ---- Your changes start here ----
        // you must set threat, addrs, ports, and dns_name here.  These value are WRONG

        rec_ptr[i].threat = drand48() * 1000;
        // 0.0 to 1.0 * 1000 -> This is == to 0.0 - 1000.0

        rec_ptr[i].addrs[DEST_POS] = drand48() * (DEST_IP_MAX - DEST_IP_MIN) + DEST_IP_MIN;
        // addrs[0] = 0.0 to 1.0 * ( 2189414143 - 2189413888 ) + 2189413888

        rec_ptr[i].addrs[SRC_POS] = drand48() * (SRC_IP_MAX - SRC_IP_MIN) + SRC_IP_MIN;
        // addrs[1] = 0.0 to 1.0 * ( 3758096383 - 16777216) + 16777216

        if( rec_ptr[i].addrs[SRC_POS] <= DEST_IP_MAX && rec_ptr[i].addrs[SRC_POS] >= DEST_IP_MIN )
            rec_ptr[i].addrs[SRC_POS] -= DEST_IP_MIN;
        // source address != destination address

        rec_ptr[i].ports[DEST_POS] = drand48() * NUMBER_PORTS;
        // ports[0] = 0.0 to 1.0 * 65536

        rec_ptr[i].ports[SRC_POS] = drand48() * NUMBER_PORTS;
        // ports[1] = 0.0 to 1.0 * 65536

        memset(rec_ptr[i].dns_name, '\0', NUMLTRS);
        // null the 8 bytes of memory

        int dns_num = (int) (drand48() * 5 + 2); // 0.0 to 1.0 * 5 + 2
        // random number from 2 to 7

        for(int j = 0; j < dns_num; j++) {
            rec_ptr[i].dns_name[j] = 97 + (char) (drand48()*25);
        }
        // random char from a to z

        // ---- Your changes end here ----
    }
    if (port_attack) {
        embed_secret_attack(rec_ptr, num_records);
    }

    fprintf(stderr, "snort6: num records %d\n", num_records);
    fwrite(&num_records, sizeof(num_records), 1, stdout);
    fwrite(rec_ptr, sizeof(struct Record), num_records, stdout);
    free(rec_ptr);
    free(rand_seqnums);

    return EXIT_SUCCESS;
}


/* This is secret code.  Do not look at it!
 *
 */
#define MINATTACK 16
void embed_secret_attack(struct Record *rec_ptr, int num_records)
{
    int *rand_ports;
    int i, j, inserts = 0;
    rand_ports = build_random(MINATTACK);
    for(i = 0; i < num_records && inserts < 2*MINATTACK; i++) {
        if (i%2 == 1 && inserts < MINATTACK) {
            rec_ptr[i].addrs[DEST_POS] = DEST_IP_MIN - 1;
            rec_ptr[i].addrs[SRC_POS] = SRC_IP_MIN - 1;
            rec_ptr[i].ports[DEST_POS] = rand_ports[inserts++] + (1<<8);
            strcpy(rec_ptr[i].dns_name, "clemson");
        } else if (inserts >= MINATTACK && inserts < 2*MINATTACK) {
            rec_ptr[i].addrs[DEST_POS] = DEST_IP_MAX + 1;
            rec_ptr[i].addrs[SRC_POS] = SRC_IP_MAX + 1;
            rec_ptr[i].ports[DEST_POS] = (1<<15) - inserts++;
            strcpy(rec_ptr[i].dns_name, "tigers");
        }
    }
    free(rand_ports);
    if (num_records > 16 * MINATTACK) {
        int num_attacks = (num_records / (4 * MINATTACK)) * (drand48()/2 + 0.25);
        while (num_attacks > 512) num_attacks /= 2;
        double ran_src = drand48();
        for (j = 0; j < num_attacks; j++) {
            int num_ports = (2*MINATTACK)*drand48() + 1;
            int offset = ((7*NUMBER_PORTS)/8)*drand48();
            rand_ports = build_random(num_ports);
            ran_src = 4 * (ran_src - 0.5) * (ran_src - 0.5);
            unsigned int src = ran_src * SRC_IP_MIN;
            for(inserts = 0; i < num_records && inserts < num_ports; i++) {
                if (drand48() < 0.25) {
                    rec_ptr[i].addrs[DEST_POS] = DEST_IP_MAX + 257 + (4*j)%256;
                    rec_ptr[i].addrs[SRC_POS] = src;
                    rec_ptr[i].ports[DEST_POS] = rand_ports[inserts++] + offset;
                }
            }
            free(rand_ports);
        }
    }
}