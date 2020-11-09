/* lab6.c template
 * Aaron Bruner
 * ajbrune
 * ECE 2220, Fall 2020
 * MP6
 *
 * Purpose: The purpose of this MP is to sort many structures using different sorting methods
 *
 * Assumptions: We must assume that the user follows the instructions for valid input. We also assume that the
 *              user is piping in correct values.
 *
 * Command line arguments: ./lab6 sort-field [sort-type]
 *                         ./snort6 num-records port-attack [seed]
 * 	sort-field:
 * 	            1. Seqnum
 * 	            2. threat
 * 	            3. DNS name
 * 	            4. IP address
 * 	            5. port scan attack
 *	sort-types:
 *	            1. qsort(default)
 *	            2. bubblesort
 *
 * Known bugs: ?
 *
 * Performance Evaluation: Insert your table of run times for sorting performance.
 *                         For each of the 4 field values, determine the largest
 *                         list size that can be sorted in less than one second
 *                         for that field. Repeat for each type of sort.
 *                         Approximate sizes are acceptable as long as you are
 *                         within 5% of the answer.
 *
 *          			    Sort-type  sort-field   size sorted in 1s
 *			                        1           1   2600000
 *          			            1           2   2400000
 *          			            1           3   1400000
 *          			            1           4   2200000
 *          			            2           1   9800
 *          			            2           2   9800
 *          			            2           3   8000
 *          			            2           4   9600
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#define DEST_POS 0
#define SRC_POS  1
#define NUMLTRS 8

// for printing IP addresses
#define OCT1(x) ((x & 0xFF000000) >> 24)
#define OCT2(x) ((x & 0xFF0000) >> 16)
#define OCT3(x) ((x & 0xFF00) >> 8)
#define OCT4(x)  (x & 0xFF)

struct Record {
    int seqnum;
    float threat;
    unsigned int addrs[2];
    unsigned short int ports[2];
    char dns_name[NUMLTRS];
};

/* This function verifies that the list is sorted.
 * It also checks that the seqnum's are all present
 *
 * The function produces no output if the list is correct.  It
 * causes the program to terminate and print a line beginning with
 * "Assertion failed:" if an error is detected.
 *
 * The checks are not exhaustive, so an error may still exist in the
 * list even if these checks pass.
 *
 * YOU MUST NOT CHANGE THIS FUNCTION.  WE USE IT DURING GRADING TO VERIFY THAT
 * YOUR LIST IS CONSISTENT.
 */
void validate_list(struct Record *ptr, int records,
                   int (*fcomp)(const void *, const void *))
{
    int i;
    int * seqnum_list = (int *)calloc(records, sizeof(int));
    if (seqnum_list == NULL) {
        fprintf(stderr, "lab6: Cannot malloc array of size %d\n", records);
        exit(1);
    }
    for(i = 0; i < records; i++) {
        if (i != records - 1)
            assert(fcomp(ptr+i, ptr+i+1) <= 0);
        assert(ptr[i].seqnum > 0 && ptr[i].seqnum <= records);
        seqnum_list[ptr[i].seqnum-1] = 1;
    }
    for(i = 0; i < records; i++)
        assert(seqnum_list[i] == 1);
    free(seqnum_list);
}

/* turn the binary data into formated ASCII text and send to sdtout
 *
 * This function is complete.
 *
 * YOU MUST NOT CHANGE THIS FUNCTION.  WE USE IT DURING GRADING TO VERIFY THAT
 * YOUR LIST IS CONSISTENT.
 */
void print_records(struct Record *S, int nrecs)
{
    int i;
    unsigned int a;
    for (i = 0; i < nrecs; i++)
    {
        printf("\nS[%d]->seqnum = %d threat = %g\n", i, S[i].seqnum, S[i].threat);
        a = S[i].addrs[DEST_POS];
        printf("       dest IP = %d.%d.%d.%d", OCT1(a),OCT2(a),OCT3(a),OCT4(a));
        a = S[i].addrs[SRC_POS];
        printf(" src IP  = %d.%d.%d.%d\n", OCT1(a),OCT2(a),OCT3(a),OCT4(a));
        printf("       ports: dest = %d src = %d\n", S[i].ports[DEST_POS], S[i].ports[SRC_POS]);
        printf("       dns_name = %s\n", S[i].dns_name);
    }
}

/* Your bubble sort algorithm goes here
 *
 * The arguments must be exactly as defined here.
 *
 * Input:
 *    ptr:     points to the start of the array
 *    records: the number of elements in the array
 *    fcomp:   function pointer to comparison function
 *
 * Assumptions:
 *    the function sorts an array of type struct Record.
 *
 *    The comparison function must return an integer less than, equal to,
 *    or greater than zero if the first argument should appear in the list
 *    respectively before, no change, or after the second.
 */
void bubblesort(struct Record *ptr, int records,
                int (*fcomp)(const void *, const void *))
{
    struct Record *temp = (struct Record *) malloc( sizeof(struct Record) );
    int flag = 0;

    for(int i = 0; i < records - 1; i++) {
        flag = 1;
        for(int j = 0; j < records - i - 1; j++) {
            if( fcomp(&ptr[j], &ptr[j+1]) == 1 ) {
                flag = 0;
                memcpy(temp, &ptr[j], sizeof(struct Record) );
                memcpy(&ptr[j], &ptr[j+1], sizeof(struct Record) );
                memcpy(&ptr[j+1], temp, sizeof(struct Record) );
            }

        }
        if(flag == 1)
            break;
    }
    free(temp);
}

/* An example of a function to compare records for use with qsort
 * or bubble sort.
 *
 * The function returns one of three values:
 *    -1 : record a should be before record b
 *     0 : the two records have the same ranking
 *     1 : record a should be after record b
 */
int SeqNumCompare(const void *a, const void *b)
{
    struct Record *sa, *sb;

    sa = (struct Record *) a;
    sb = (struct Record *) b;

    if (sa->seqnum < sb->seqnum) return -1;
    else if (sa->seqnum > sb->seqnum) return 1;
    else return 0;
}

/*----------------------------------------------------------*/
/* The function ThreatCompare for comparing the difference in threat levels
 * based on two different structures a and b
 *
 * input:
 *     a: the pointer to the first structure
 *     b: the pointer to the second structure
 *
 * return value:
 *     -1: record a is < record b
 *     0: record a == record b
 *     1: record a is > record b
 */
int ThreatCompare(const void *a, const void *b)
{

    struct Record *structureA, *structureB;

    structureA = (struct Record *) a;
    structureB = (struct Record *) b;

    if (structureA->threat < structureB->threat) return -1;
    else if (structureA->threat > structureB->threat) return 1;
    else return 0;
}

/*----------------------------------------------------------*/
/* The function AddrCompare for comparing the difference in addresses
 * based on two different structures a and b
 *
 * input:
 *     a: the pointer to the first structure
 *     b: the pointer to the second structure
 *
 * return value:
 *     -1: record a is < record b
 *     0: record a == record b
 *     1: record a is > record b
 */
/*
 *  @brief ThreatCompare
 *	it performs comparison between structure a and b
 *	based on threat level
 * @return
 *    -1 : record a should be before record b
 *     0 : the two records have the same ranking
 *     1 : record a should be after record b
 */
int AddrCompare(const void *a, const void *b)
{

    struct Record *structureA, *structureB;

    structureA = (struct Record *) a;
    structureB = (struct Record *) b;

    //compare destination address
    if (structureA->addrs[0] < structureB->addrs[0]) return -1;
    else if (structureA->addrs[0] > structureB->addrs[0] ) return 1;
        // compare src address
    else if (structureA->addrs[1] < structureB->addrs[1] ) return -1;
    else if (structureA->addrs[1] > structureB->addrs[1] ) return 1;
    else return 0;
}


/*
 *  @brief DNSNameCompare
 *	it performs comparison between structure a and b
 *	based on DNS name
 * @return
 *    -1 : record a should be before record b
 *     0 : the two records have the same ranking
 *     1 : record a should be after record b
 */
int DNSNameCompare(const void *a, const void *b)
{

    struct Record *sa, *sb;
    int i=0;
    sa = (struct Record *) a;
    sb = (struct Record *) b;
    //sort the length of dns name first
    if(strlen(sa->dns_name)< strlen(sb->dns_name)  ) return -1;
    else if(strlen(sa->dns_name )> strlen(sb->dns_name) ) return 1;
    else {
        //if the lengths of dns name are equal
        //then compare each letter
        while( sa->dns_name[i]!='\0' && sb->dns_name[i]!='\0' )
        {
            if (sa->dns_name[i] < sb->dns_name[i]) return -1;
            else if (sa->dns_name[i] > sb->dns_name[i]) return 1;
            i++;
        }
    }

    //if both length of dns name and letters are same, return 0
    return 0;
}


/*
 *  @brief PortCompare
 *	it performs comparison between structure a and b
 *	based on dest port
 * @return
 *    -1 : record a should be before record b
 *     0 : the two records have the same ranking
 *     1 : record a should be after record b
 */
int PortsCompare(const void *a, const void *b)
{

    struct Record *sa, *sb;

    sa = (struct Record *) a;
    sb = (struct Record *) b;

    //compare destination address
    if (sa->ports[0] <sb->ports[0]) return -1;
    else if (sa->ports[0] > sb->ports[0] ) return 1;
        // compare src address
    else return 0;
}


/* @brief scan_consecutive
 * 	 this function takes the block of packets from record, the size of block as input
 * 	 and return the start_point_array and port_count array and the amount of attacks
 * 	 from a set of packet from the same source
 * @*ptr:
 * 	the block of packets
 * @block_size:
 * 	size of the block
 * @**start_port
 * 	return the array storing info of attacks from the same source
 * @**port_count
 * 	return the port_count of the start_port
 * @return
 * 	it return the amount of attacks from a block from the same source
 */

int scan_consecutive(struct Record *ptr, int block_size, int **start_port, int **port_count )
{
    int i=0, consecutive_count, attack_count=0, port_index=0;
    consecutive_count=1;
    //scan how much attack block in a packet
    for(i=0; i<block_size;i++)
    {
        //check if they are consecutive
        if( ptr[i].ports[0] ==(ptr[i+1].ports[0]-1) ) consecutive_count++;
        else if(consecutive_count>=16 )
        {
            attack_count++;	//found an attack
            consecutive_count=1;//reset data
        }
        else if(consecutive_count<16){
            consecutive_count=1;
        }
    }
    //if attack exists, allocate space to store each attack info
    if(attack_count>0)
    {
        *start_port= (int *)malloc(attack_count*sizeof(int) );
        *port_count= (int *)malloc(attack_count*sizeof(int));
        consecutive_count=1;
        port_index=0;
        for(i=0; i<block_size;i++)
        {
            if( ptr[i].ports[0] ==(ptr[i+1].ports[0]-1) ) consecutive_count++;
            else if(consecutive_count>=16 )//attack detected
            {       //store start_port and port_count of each attack to an array
                (*port_count)[port_index]=consecutive_count;
                (*start_port)[port_index]=ptr[i-1].ports[0]-consecutive_count+1;
                port_index++;
                consecutive_count=1;//reset consecutive count
            }
            else if(consecutive_count<16){
                consecutive_count =1;
            }
        }
    }
    else{
        *start_port=NULL;
        *port_count=NULL;
    }

    return attack_count;
}


/*
 * @brief attack_detected
 * 	  This function detects the amount of attacks from the packets
 *	  and print out the attack info and return the number of attacks
 * @*rec_ptr:
 * 	  the record array
 * @records:
 * 	  the number of packets in the array
 * @return:
 *	  it return the amount of attacks
 */
int attack_detected(struct Record *rec_ptr, int records )
{
    int attack_num=0;
    unsigned int dest_ip=0, src_ip=0;  // source and destination IP address for attack
    int start_port=0;                  // lowest port num in an attack
    int port_count=0;                  // size of attack (num consequitive ports scanned)
    int num_attacks=0;  // count the number of attacks discovered
    int block_size=0, attack_count=0;
    struct Record *ptr, *packs = (struct Record *) malloc(records*sizeof(struct Record) );
    int *start_port_array, *port_count_array;

    memcpy(packs,rec_ptr, records*sizeof(struct Record)  );
    //sort packets before porting attack
    qsort(packs, records, sizeof(struct Record), AddrCompare );
    ptr= packs;
    //loop through the received packets
    while( ptr < &packs[records-1])
    {
        src_ip = ptr->addrs[1];
        dest_ip= ptr->addrs[0];
        block_size=1;
        //obtain size of block sent from the same src ip to the same dest ip
        while( ptr[ block_size].addrs[1]== src_ip && ptr[block_size].addrs[0] ==dest_ip  )
        {//check packet boundary
            block_size++;
            if(&ptr[block_size] > &packs[records-1 ] )  break;
        }
        //check suspect block with size >=16, which may contain more than one attack blocks
        if(block_size>=16 )
        {
            //sort block data
            qsort(ptr,block_size,sizeof(struct Record), PortsCompare );
            //return count of attack from a block and info of each attack
            attack_count = scan_consecutive(ptr, block_size,&start_port_array ,&port_count_array);
            if(start_port_array!=NULL && port_count_array!=NULL)
            {

                for(;attack_num< num_attacks+attack_count;attack_num++ )
                {
                    start_port=start_port_array[ attack_num-num_attacks ];
                    port_count=port_count_array[attack_num-num_attacks ];
                    fprintf(stderr,"attack %d at %d.%d.%d.%d from %d.%d.%d.%d starting at %d for %d ports\n", attack_num, OCT1(dest_ip),OCT2(dest_ip),OCT3(dest_ip),OCT4(dest_ip),
                            OCT1(src_ip), OCT2(src_ip), OCT3(src_ip), OCT4(src_ip),
                            start_port, port_count);
                }
                free(start_port_array);
                free(port_count_array);
                num_attacks+=attack_count;
            }
        }
        //move to the next block
        ptr+=block_size;

    }
    //print_records(packs, records);
    ptr=NULL;
    free(packs);

    return num_attacks;
}




/*----------------------------------------------------------*/
int main(int argc, char *argv[])
{
    int sort_field = 0;
    int sort_type  = 1;
    clock_t start, end;

    if (argc != 2 && argc != 3) {
        fprintf(stderr, "Usage: lab6 sort-field [sort-type]\n");
        fprintf(stderr, "Sort-field\n");
        fprintf(stderr, "  1   seqnum\n");
        fprintf(stderr, "  2   threat score\n");
        fprintf(stderr, "  3   DNS name\n");
        fprintf(stderr, "  4   IP addresses\n");
        fprintf(stderr, "  5   Port Scan Analysis\n");
        fprintf(stderr, "Sort-type\n");
        fprintf(stderr, "  0   do not sort just print data\n");
        fprintf(stderr, "  1   qsort (default)\n");
        fprintf(stderr, "  2   bubble-sort\n");

        exit(1);
    }
    sort_field = atoi(argv[1]);
    if (sort_field < 1 || sort_field > 5) {
        fprintf(stderr, "lab6: Invalid sort field %d\n", sort_field);
        exit(1);
    }
    if (sort_field == 5 && argc != 2) {
        fprintf(stderr, "lab6: Invalid port scan attack analysis %d\n", sort_field);
        exit(1);
    }
    if (argc == 3) {
        sort_type = atoi(argv[2]);
        if (sort_type < 0 || sort_type > 2) {
            fprintf(stderr, "lab6: Invalid sort type %d\n", sort_type);
            exit(1);
        }
    }

    int num_records = -1;
    int num_items = -1;
    struct Record *rec_ptr;

    num_items = fread(&num_records, sizeof(num_records), 1, stdin);
    if (num_items != 1) {
        fprintf(stderr, "lab6: Error fread failed to read number records %d\n", num_items);
        exit(1);
    }
    if (num_records < 2) {
        fprintf(stderr, "lab6: Error invalid number records %d\n", num_records);
        exit(1);
    }
    printf("lab6: num records %d\n", num_records);

    /* Notice this is the same approach we used in MP5 except here
     * we can read all the the records with one call to fread.
     */
    rec_ptr = (struct Record *) malloc(num_records * sizeof(struct Record));
    num_items = fread(rec_ptr, sizeof(struct Record), num_records, stdin);
    if (num_items != num_records) {
        fprintf(stderr, "lab6: failed to read all records %d\n", num_items);
        exit(1);
    }

    // The code in main from the beginning until here is complete
    // Your changes start below
    switch(sort_type) {

        case 1:
            // this is the correct way to time qsort
            start =clock();
            switch(sort_field)
            {
                case 1:
                    qsort(rec_ptr, num_records, sizeof(struct Record), SeqNumCompare);
                    break;
                case 2:
                    qsort(rec_ptr, num_records, sizeof(struct Record), ThreatCompare);
                    break;
                case 3:
                    qsort(rec_ptr, num_records, sizeof(struct Record), DNSNameCompare);
                    break;
                case 4:
                    qsort(rec_ptr, num_records, sizeof(struct Record), AddrCompare);
                    break;
                default:
                    break;

            }
            end = clock();

            // you must print using exactly this format
            fprintf(stderr, "lab6: qsort time=%g for %d records\n",
                    1000*((double)(end-start))/CLOCKS_PER_SEC, num_records);

            // after sorting you must validate that the list is sorted
            switch(sort_field)
            {
                case 1:
                    validate_list(rec_ptr, num_records, SeqNumCompare);
                    break;
                case 2:
                    validate_list(rec_ptr, num_records, ThreatCompare);
                    break;
                case 3:
                    validate_list(rec_ptr, num_records, DNSNameCompare);
                    break;
                case 4:
                    validate_list(rec_ptr, num_records, AddrCompare);
                    break;
                default:
                    break;

            }
            break;
        case 2:
            start = clock();
            switch(sort_field)
            {
                case 1:
                    bubblesort(rec_ptr, num_records, SeqNumCompare);
                    end = clock();
                    break;
                case 2:
                    bubblesort(rec_ptr, num_records, ThreatCompare);
                    end = clock();
                    break;
                case 3:
                    bubblesort(rec_ptr, num_records, DNSNameCompare);
                    end = clock();
                    break;
                case 4:
                    bubblesort(rec_ptr, num_records, AddrCompare);
                    end = clock();
                    break;
                default:
                    end = clock();
                    break;


            }
            // after sorting you must validate that the list is sorted
            switch(sort_field)
            {
                case 1:
                    validate_list(rec_ptr, num_records, SeqNumCompare);
                    break;
                case 2:
                    validate_list(rec_ptr, num_records, ThreatCompare);
                    break;
                case 3:
                    validate_list(rec_ptr, num_records, DNSNameCompare);
                    break;
                case 4:
                    validate_list(rec_ptr, num_records, AddrCompare);
                    break;
                default:
                    break;

            }

            fprintf(stderr, "lab6: bubble sort time=%g for %d records\n",
                    1000*((double)(end-start))/CLOCKS_PER_SEC, num_records);
            break;
        case 5:
            int num_attacks=0;
            //detect attack from a record and return the total number of attacks
            num_attacks= attack_detected(rec_ptr,num_records );
            if(num_attacks==0)
                fprintf(stderr,"lab6: no attacks detected for %d records\n", num_records);
            else{
                // after printing details of each attack, print how many attacks are found
                fprintf(stderr,"lab6: discovered %d attacks\n", num_attacks);
            }
            break;
        default:
            break;
    }
    /*if (sort_type == 1)   // qsort
    {
        // this is the correct way to time qsort
        start =clock();
        switch(sort_field)
        {
            case 1:
                qsort(rec_ptr, num_records, sizeof(struct Record), SeqNumCompare);
                break;
            case 2:
                qsort(rec_ptr, num_records, sizeof(struct Record), ThreatCompare);
                break;
            case 3:
                qsort(rec_ptr, num_records, sizeof(struct Record), DNSNameCompare);
                break;
            case 4:
                qsort(rec_ptr, num_records, sizeof(struct Record), AddrCompare);
                break;
            default:
                break;

        }
        end = clock();

        // you must print using exactly this format
        fprintf(stderr, "lab6: qsort time=%g for %d records\n",
                1000*((double)(end-start))/CLOCKS_PER_SEC, num_records);

        // after sorting you must validate that the list is sorted
        switch(sort_field)
        {
            case 1:
                validate_list(rec_ptr, num_records, SeqNumCompare);
                break;
            case 2:
                validate_list(rec_ptr, num_records, ThreatCompare);
                break;
            case 3:
                validate_list(rec_ptr, num_records, DNSNameCompare);
                break;
            case 4:
                validate_list(rec_ptr, num_records, AddrCompare);
                break;
            default:
                break;

        }
    }
    if (sort_type == 2)   // bubble sort
    {
        start = clock();
        switch(sort_field)
        {
            case 1:
                bubblesort(rec_ptr, num_records, SeqNumCompare);
                end = clock();
                break;
            case 2:
                bubblesort(rec_ptr, num_records, ThreatCompare);
                end = clock();
                break;
            case 3:
                bubblesort(rec_ptr, num_records, DNSNameCompare);
                end = clock();
                break;
            case 4:
                bubblesort(rec_ptr, num_records, AddrCompare);
                end = clock();
                break;
            default:
                end = clock();
                break;


        }
        // after sorting you must validate that the list is sorted
        switch(sort_field)
        {
            case 1:
                validate_list(rec_ptr, num_records, SeqNumCompare);
                break;
            case 2:
                validate_list(rec_ptr, num_records, ThreatCompare);
                break;
            case 3:
                validate_list(rec_ptr, num_records, DNSNameCompare);
                break;
            case 4:
                validate_list(rec_ptr, num_records, AddrCompare);
                break;
            default:
                break;

        }

        fprintf(stderr, "lab6: bubble sort time=%g for %d records\n",
                1000*((double)(end-start))/CLOCKS_PER_SEC, num_records);
    }

    // Somewhere you have to handle the port scan analysis.  You must either
    // use the first print to claim there are no attacks.  Or, you must
    // print one output for each attack discovered and a final count of the number
    // of attacks that are found.

    if (sort_field == 5) {

        int num_attacks=0;
        //detect attack from a record and return the total number of attacks
        num_attacks= attack_detected(rec_ptr,num_records );
        if(num_attacks==0)
            fprintf(stderr,"lab6: no attacks detected for %d records\n", num_records);
        else{
            // after printing details of each attack, print how many attacks are found
            fprintf(stderr,"lab6: discovered %d attacks\n", num_attacks);
        }
    }*/

    // printing the records is required.  If you don't want to save
    // the output, on the shell redirect stdout to /dev/null
    print_records(rec_ptr, num_records);

    free(rec_ptr);

    return EXIT_SUCCESS;
}