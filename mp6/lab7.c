/* lab7.c
 * Aaron Bruner
 * ajbrune
 * ECE 2220, Fall 2020
 * MP7
 *
 * Purpose: The purpose of this MP is to generate sub processes that all work based on a main parent process. The parent
 *          process will feed commands
 *
 * Assumptions: We are assuming that four terminal windows are open and that the user knows the ID for each terminal.
 *              Then, we assume that the user knows the commands and uses them correctly.
 *
 * Command line arguments: Usage: ./lab7 1 2 3 4
 *			    1 tty terminal number of monitor
 *			    2 terminal number of IT service
 *			    3 terminal number of IT service
 *			    4 terminal number of IT service
 *
 *			Ops center Commands:
 *			sn:  Send SNMP request
 *			rn:  reconfiguration request
 *			kn:	 kill service n
 *			where n is the IT service number and n should be 1 to 3
 *
 * Known Bugs: N/A
 *
 * It's good to note that the over majority of the code is replicable. Meaning you can use the same code for each terminal
 * Thus, the code is almost identical for each terminal.
 *
 */


#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>

#define TERM_COUNT 4
#define SIGSNMP 16
#define SIGRECONFIG 17
#define EXCODE_SUCCESSIVE 1
#define EXCODE_COMPROMISED 2
#define EXCODE_CRASH 3
#define FALSE 0
#define TRUE 1
#define THREAT_RED 2
#define THREAT_YELLOW 1
#define THREAT_GREEN 0
#define THREAT_CONDITIONAL_RED 10
#define THREAT_CONDITIONAL_YELLOW 5
#define MIN_REPORT_TIME 5
#define THREAT_CRITICAL 15

typedef struct timeval TimeStruct;
struct timeval OldTime;
int SubtractTimevals(TimeStruct *result, TimeStruct *x, TimeStruct *y);
int calculateThreat(int threat);
char getServiceNumber(int *Services, int ser_amt, int pid);
void serviceStatus(int status_code, unsigned char *exit_code, unsigned char *signal_byte);
void openTerms(char *argv[], char (*strDev)[50], int *Terminal, int *term_cnt);

void Task_Monitor(int servicePIDs[TERM_COUNT]);
void ITService1(char *dev);
void ITService2(char *dev);
void ITService3(char *dev);

//global vars to store info about the IT services
int Terminal[TERM_COUNT], servicePIDs[TERM_COUNT];
int parentPID;
char strDev[TERM_COUNT][50];

int main(int argc, char *argv[]) {

    int servicePID;
    int term_cnt = 0, processCounter = 0, Server_status = 0;
    unsigned char exit_code = 0, return_signal = 0;
    unsigned char service_num=-1;
    unsigned char ret_status[TERM_COUNT-1];

    if (argc != TERM_COUNT+1) {
        printf("Usage: ./lab7 1 2 3 4, where the numbers are the terminal pts values\n");
        exit(1);
    }

    // Good examples of the below code are in signal4.c and openterms.c

    openTerms(argv, strDev, Terminal, &term_cnt); // check if ttys are open
    parentPID = getpid(); // get parent PID

    processCounter = 1; // service 1
    servicePIDs[processCounter] = fork(); // service 1 PID

    if(servicePIDs[processCounter] !=0 ) {
        processCounter++;
        servicePIDs[processCounter] = fork(); // service 2
        if(servicePIDs[processCounter] !=0 ) {
            processCounter++;
            servicePIDs[processCounter] = fork(); // service 3
            if(servicePIDs[processCounter] !=0 ) {
                processCounter++;
                servicePIDs[0] = fork();// monitor service
                if(servicePIDs[0] !=0 ) { //parent task
                    for( ; processCounter > 1; processCounter--) {

                        servicePID = wait(&Server_status ); //obtain service pid
                        service_num = getServiceNumber(servicePIDs,TERM_COUNT,servicePID); //obtain service number
                        serviceStatus(Server_status, &exit_code, &return_signal); //obtain returned exit code and signal

                        if(return_signal != SIGKILL)
                            ret_status[service_num-1] = exit_code;
                        else
                            ret_status[service_num-1] = SIGKILL;
                    }
                    for(service_num = 1; service_num < TERM_COUNT; service_num++) { // IT status at end
                    exit_code = ret_status[service_num-1];
                        switch(exit_code ) {
                            case EXIT_SUCCESS:
                                printf("Job well done IT specialist %d. Prepare for new attack\n", service_num);
                                break;
                            case EXCODE_CRASH:
                                printf("Call HR, we need a cybersecurity expert for service %d\n",service_num);
                                break;
                            case EXCODE_COMPROMISED:
                                printf("IT service %d compromised, we are going to out of business\n", service_num);
                                break;
                            case SIGKILL:
                                printf("Terminated IT service %d\n", service_num);// when service was killed by user
                                break;
                        }
                    }
                    kill(servicePIDs[0],SIGKILL); // close monitor
                }
                else {
                    servicePIDs[0] = getpid(); // get monitor pid
                    // sprintf(strDev, "/dev/pts/%d", ttyindex);
                    sprintf(strDev[0],"/dev/pts/%d", Terminal[0]);
                    Task_Monitor(servicePIDs); //pass terminal dev to monitor
                }
            }
            else {
                sprintf(strDev[processCounter], "/dev/pts/%d", Terminal[processCounter]);
                ITService3(strDev[processCounter]); //pass terminal dev to service 3
            }
        }
        else {
            sprintf(strDev[processCounter], "/dev/pts/%d", Terminal[processCounter]);
            ITService2(strDev[processCounter]); //pass terminal dev to service 2
        }
    }
    else {
        sprintf(strDev[processCounter], "/dev/pts/%d", Terminal[processCounter]);
        ITService1(strDev[processCounter]); //pass terminal dev to service 1
    }

    exit(EXIT_SUCCESS);
}

/* The function serviceStatus is to parse the status code from the services to determine
 * the exit code & signal.
 *
 * input:
 *     status_code: type int, status code from wait() function
 *     *exit_code: exit code parsed from the status_code (RETURN)
 *     *signal: the signal to return from the services' status (RETURN)
 *
 * return value: N/A
 *
 * It's worth noting that this code came from the kill.c example file. ( Line 33 & 34 )
 *
 */
void serviceStatus(int status_code, unsigned char *exit_code, unsigned char *signal_byte) {
    *exit_code = (unsigned char) (status_code >> 8);
    *signal_byte = (unsigned char) (status_code & 0x7F);
}

/* The function getServiceNumber returns the IT service number
 *
 * input:
 *     *Services: type int pointer, array of service pids
 *     ser_amt: type int, number of IT services
 *     pid: type int, PID of services to find the service number
 *
 * return value: Either IT service number or -1 if not found
 */
char getServiceNumber(int *Services, int ser_amt, int pid) {

    for(int i = 0; i < ser_amt; i++)
        if(pid == Services[i])
            return (char)(i);

    return -1;
}

/* The function openTerms obtains the tty terminal numbers and stores them for later use.
 *
 * input:
 *     *argv[]: type char, terminal numbers from the terminal that executed lab7
 *     (*strDev)[50]: type char, 2D array to store terminal names
 *     *Terminal: type int, array stores terminal numbers
 *     *term_cnt: type int, final terminal count (RETURN VALUE)
 *
 * return value: N/A
 *
 * It's worth noting that almost all of this code came from openterms.c which was provided.
 *
 */
void openTerms(char *argv[], char (*strDev)[50], int *Terminal, int *term_cnt) {

    FILE *fpt;
    int ttyindex, term_count = *term_cnt;

    for (term_count = 0; term_count < TERM_COUNT; term_count++) {

        ttyindex = -1;
        ttyindex = atoi(argv[term_count+1]);
        if (ttyindex < 1) {
            printf("ERROR . . . terminal number incorrect : %s\n", argv[term_count+1]);
            exit(1);
        }

        sprintf(strDev[term_count], "/dev/pts/%d", ttyindex); // terminal names

        if ((fpt = fopen(strDev[term_count], "w")) != NULL) {
            Terminal[term_count] = ttyindex;
            fprintf(fpt, "IT Service Terminal ID[%d], Service Number: [%d]\n", ttyindex, term_count);
            printf("Terminal[%d] = %d\n", term_count, ttyindex);
            fclose(fpt);
        } else {
            printf("failed to open terminal %s\n", strDev[term_count]);
            exit(1);
        }
    }
    *term_cnt = term_count;
}

/* The function calculateThreat was provided in the main PDF.
 * The function determines the new threat level for the terminal if a signal alarm as been indicated.
 *
 * input:
 *     threat: an integer value, current threat level
 *
 * return value:
 *     threat: an integer value, new threat level
 */
int calculateThreat(int threat)
{
    if(drand48() < 0.5) {
        threat++;
    } else if ( threat > 1 && drand48() < 0.6) {
        threat--;
    }
    return threat;
}

/* The function SubtractTimevals is intended to return the difference in time between the last
 * SNMP request and the next SNMP request.
 *
 * input:
 *     *result: difference in time (RETURN)
 *     *x: new SNMP time
 *     *y: last SNMP time
 *
 * return value: N/A
 *
 * It's worth noting that all of this code came from time2.c example file.
 *
 */
int SubtractTimevals(TimeStruct *result, TimeStruct *x, TimeStruct *y)
{
    /* Perform the carry for the later subtraction by updating y. */
    if (x->tv_usec < y->tv_usec) {
        int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
        y->tv_usec -= 1000000 * nsec;
        y->tv_sec += nsec;
    }
    if (x->tv_usec - y->tv_usec > 1000000) {
        int nsec = (x->tv_usec - y->tv_usec) / 1000000;
        y->tv_usec += 1000000 * nsec;
        y->tv_sec -= nsec;
    }

    /* Compute the time remaining to wait. tv_usec is certainly positive. */
    result->tv_sec = x->tv_sec - y->tv_sec;
    result->tv_usec = x->tv_usec - y->tv_usec;

    /* Return 1 if result is negative. */
    return x->tv_sec < y->tv_sec;
}

/* The function IT1_SigHandler deals with the SNMP signal, reconfigure signal, and the alarm signal.
 *
 * input:
 *     signum: type int, signal number identifies which handler was received
 *
 * return value: N/A
 *
 */
void IT1_SigHandler(int signum) {

    FILE *fpt;
    static unsigned char reconfigure = FALSE; // BOOL check for reconfigure
    static unsigned char threat = TRUE; // BOOL for threat (1/0)
    static unsigned char threat_color = THREAT_GREEN; // Different threat levels (red,yellow,green)
    static struct timeval NewTime, diff_time; // standard time diff
    static char firstSNMP = TRUE; //flag for detecting first SNMP request

    fpt = fopen(strDev[1], "w");
    switch(signum) {

        case SIGRECONFIG: // reconfiguration signal
            if(reconfigure == FALSE && threat_color == THREAT_RED) { // Good reconfig
                reconfigure = TRUE;
                fprintf(fpt, "Reconfiguring system to thwart attack - this may take a few seconds\n");
            }
            else if(reconfigure == TRUE) { // reconfig more than once
                fprintf(fpt, "Cannot reconfigure more than once. You are fired!\n");
                fclose(fpt);
                exit(EXCODE_CRASH);
            }
            else if(threat_color < THREAT_RED) { // reconfig before red threat
                fprintf(fpt, "Threat level is not critical. You are fired!\n");
                fclose(fpt);
                exit(EXCODE_CRASH);
            }
            break;

        case SIGSNMP: // SNMP request signal
            if(diff_time.tv_sec < MIN_REPORT_TIME && !firstSNMP) { // snmp request faster than 5s
                fprintf(fpt, "Load too high. Threat is increased\n");
                threat++;
            }
            else { //print threat color
                threat_color = (threat >= THREAT_CONDITIONAL_RED) ?
                        THREAT_RED:((threat >= THREAT_CONDITIONAL_YELLOW) ? THREAT_YELLOW:THREAT_GREEN);
                if(threat_color == THREAT_GREEN)
                    fprintf(fpt, "Threat Color: [GREEN]\n");
                else if(threat_color == THREAT_YELLOW)
                    fprintf(fpt, "Threat Color: [YELLOW]\n");
                else if(threat_color == THREAT_RED)
                    fprintf(fpt, "Threat Color: [RED]\n");

                gettimeofday(&OldTime, NULL); //time2.c example file
                firstSNMP = FALSE;
            }
            break;

        case SIGALRM: // alarm signal

            if (!firstSNMP) { // time since last SNMP request
                gettimeofday(&NewTime, NULL);
                SubtractTimevals(&diff_time, &NewTime, &OldTime);
            } // Code from time2.c

            if(reconfigure == TRUE) { // new threat because reconfigure
                threat--;
                if(threat < THREAT_CONDITIONAL_RED) {
                    fprintf(fpt, "Attack averted. Mission complete\n");
                    fclose(fpt);
                    exit(EXIT_SUCCESS);
                }
            }
            else
                threat = calculateThreat(threat); // new threat level

            //alarm time of available SNMP request
            if(diff_time.tv_sec < MIN_REPORT_TIME && !firstSNMP ) {
                fprintf(fpt, "Next report available in %d seconds.\n", 4 - (int)(diff_time.tv_sec) );

            }
            if(threat > THREAT_CRITICAL) {
                fprintf(fpt, "Intruder! Data stolen... \n");
                fclose(fpt);
                exit(EXCODE_COMPROMISED);
            }
            alarm(TRUE);
            break;
    }
    fclose(fpt);
}

/* The function IT2_SigHandler deals with the SNMP signal, reconfigure signal, and the alarm signal.
 *
 * input:
 *     signum: type int, signal number identifies which handler was received
 *
 * return value: N/A
 *
 */
void IT2_SigHandler(int signum) {

    FILE *fpt;
    static unsigned char reconfigure = FALSE; // BOOL check for reconfigure
    static unsigned char threat = TRUE; // BOOL for threat (1/0)
    static unsigned char threat_color = THREAT_GREEN; // Different threat levels (red,yellow,green)
    static struct timeval NewTime, diff_time; // standard time diff
    static char firstSNMP = TRUE; //flag for detecting first SNMP request

    fpt = fopen(strDev[2], "w");
    switch(signum) {

        case SIGRECONFIG: // reconfiguration signal
            if(reconfigure == FALSE && threat_color == THREAT_RED) { // Good reconfig
                reconfigure = TRUE;
                fprintf(fpt, "Reconfiguring system to thwart attack - this may take a few seconds\n");
            }
            else if(reconfigure == TRUE) { // reconfig more than once
                fprintf(fpt, "Cannot reconfigure more than once. You are fired!\n");
                fclose(fpt);
                exit(EXCODE_CRASH);
            }
            else if(threat_color < THREAT_RED) { // reconfig before red threat
                fprintf(fpt, "Threat level is not critical. You are fired!\n");
                fclose(fpt);
                exit(EXCODE_CRASH);
            }
            break;

        case SIGSNMP: // SNMP request signal
            if(diff_time.tv_sec < MIN_REPORT_TIME && !firstSNMP) { // snmp request faster than 5s
                fprintf(fpt, "Load too high. Threat is increased\n");
                threat++;
            }
            else { //print threat color
                threat_color = (threat >= THREAT_CONDITIONAL_RED) ?
                               THREAT_RED:((threat >= THREAT_CONDITIONAL_YELLOW) ? THREAT_YELLOW:THREAT_GREEN);
                if(threat_color == THREAT_GREEN)
                    fprintf(fpt, "Threat Color: [GREEN]\n");
                else if(threat_color == THREAT_YELLOW)
                    fprintf(fpt, "Threat Color: [YELLOW]\n");
                else if(threat_color == THREAT_RED)
                    fprintf(fpt, "Threat Color: [RED]\n");

                gettimeofday(&OldTime, NULL); //time2.c example file
                firstSNMP = FALSE;
            }
            break;

        case SIGALRM: // alarm signal

            if (!firstSNMP) { // time since last SNMP request
                gettimeofday(&NewTime, NULL);
                SubtractTimevals(&diff_time, &NewTime, &OldTime);
            } // Code from time2.c

            if(reconfigure == TRUE) { // new threat because reconfigure
                threat--;
                if(threat < THREAT_CONDITIONAL_RED) {
                    fprintf(fpt, "Attack averted. Mission complete\n");
                    fclose(fpt);
                    exit(EXIT_SUCCESS);
                }
            }
            else
                threat = calculateThreat(threat); // new threat level

            //alarm time of available SNMP request
            if(diff_time.tv_sec < MIN_REPORT_TIME && !firstSNMP ) {
                fprintf(fpt, "Next report available in %d seconds.\n", 4 - (int)(diff_time.tv_sec) );

            }
            if(threat > THREAT_CRITICAL) {
                fprintf(fpt, "Intruder! Data stolen... \n");
                fclose(fpt);
                exit(EXCODE_COMPROMISED);
            }
            alarm(TRUE);
            break;
    }
    fclose(fpt);
}


/* The function IT3_SigHandler deals with the SNMP signal, reconfigure signal, and the alarm signal.
 *
 * input:
 *     signum: type int, signal number identifies which handler was received
 *
 * return value: N/A
 *
 */
void IT3_SigHandler(int signum) {

    FILE *fpt;
    static unsigned char reconfigure = FALSE; // BOOL check for reconfigure
    static unsigned char threat = TRUE; // BOOL for threat (1/0)
    static unsigned char threat_color = THREAT_GREEN; // Different threat levels (red,yellow,green)
    static struct timeval NewTime, diff_time; // standard time diff
    static char firstSNMP = TRUE; //flag for detecting first SNMP request

    fpt = fopen(strDev[3], "w");
    switch(signum) {

        case SIGRECONFIG: // reconfiguration signal
            if(reconfigure == FALSE && threat_color == THREAT_RED) { // Good reconfig
                reconfigure = TRUE;
                fprintf(fpt, "Reconfiguring system to thwart attack - this may take a few seconds\n");
            }
            else if(reconfigure == TRUE) { // reconfig more than once
                fprintf(fpt, "Cannot reconfigure more than once. You are fired!\n");
                fclose(fpt);
                exit(EXCODE_CRASH);
            }
            else if(threat_color < THREAT_RED) { // reconfig before red threat
                fprintf(fpt, "Threat level is not critical. You are fired!\n");
                fclose(fpt);
                exit(EXCODE_CRASH);
            }
            break;

        case SIGSNMP: // SNMP request signal
            if(diff_time.tv_sec < MIN_REPORT_TIME && !firstSNMP) { // snmp request faster than 5s
                fprintf(fpt, "Load too high. Threat is increased\n");
                threat++;
            }
            else { //print threat color
                threat_color = (threat >= THREAT_CONDITIONAL_RED) ?
                               THREAT_RED:((threat >= THREAT_CONDITIONAL_YELLOW) ? THREAT_YELLOW:THREAT_GREEN);
                if(threat_color == THREAT_GREEN)
                    fprintf(fpt, "Threat Color: [GREEN]\n");
                else if(threat_color == THREAT_YELLOW)
                    fprintf(fpt, "Threat Color: [YELLOW]\n");
                else if(threat_color == THREAT_RED)
                    fprintf(fpt, "Threat Color: [RED]\n");

                gettimeofday(&OldTime, NULL); //time2.c example file
                firstSNMP = FALSE;
            }
            break;

        case SIGALRM: // alarm signal

            if (!firstSNMP) { // time since last SNMP request
                gettimeofday(&NewTime, NULL);
                SubtractTimevals(&diff_time, &NewTime, &OldTime);
            } // Code from time2.c

            if(reconfigure == TRUE) { // new threat because reconfigure
                threat--;
                if(threat < THREAT_CONDITIONAL_RED) {
                    fprintf(fpt, "Attack averted. Mission complete\n");
                    fclose(fpt);
                    exit(EXIT_SUCCESS);
                }
            }
            else
                threat = calculateThreat(threat); // new threat level

            //alarm time of available SNMP request
            if(diff_time.tv_sec < MIN_REPORT_TIME && !firstSNMP ) {
                fprintf(fpt, "Next report available in %d seconds.\n", 4 - (int)(diff_time.tv_sec) );

            }
            if(threat > THREAT_CRITICAL) {
                fprintf(fpt, "Intruder! Data stolen... \n");
                fclose(fpt);
                exit(EXCODE_COMPROMISED);
            }
            alarm(TRUE);
            break;
    }
    fclose(fpt);
}

/* The function Monitor_SignalHandler is used to kill all child processes when kill or CTRL+C is received.
 *
 * input:
 *     signum: identifies what signal the handler received
 *
 * return value: N/A
 */
void Monitor_SignalHandler(int signum) {

    for(int i = 1; i < TERM_COUNT; i++) // kill all of the child processes
        kill(servicePIDs[i], SIGKILL);

    kill(parentPID, SIGKILL);
    exit(0);
}

/* The function ITService1 is the service 1 terminal things. Everything done for service one is here.
 *
 * input:
 *     *dev: terminal name
 *
 * return value: N/A
 *
 */
void ITService1(char *dev) {
    FILE *fpt;
    int seed = time(NULL); srand48(seed); // random seed

    alarm(TRUE);

    if((fpt = fopen(dev, "w")) != NULL) {
        fprintf(fpt,"\nIT Service 1 Status is . . . ONLINE\n");
        fclose(fpt);
    }

    signal(SIGRECONFIG, IT1_SigHandler); // signal handlers ( CODE FROM alarm1.c )
    signal(SIGSNMP, IT1_SigHandler);
    signal(SIGALRM, IT1_SigHandler);

    while(1) // wait for monitor signals
        pause();
}

/* The function ITService2 is the service 2 terminal things. Everything done for service two is here.
 *
 * input:
 *     *dev: terminal name
 *
 * return value: N/A
 *
 */
void ITService2(char *dev) {
    FILE *fpt;
    int seed = time(NULL); srand48(seed); // random seed

    alarm(TRUE);

    if((fpt = fopen(dev, "w")) != NULL) {
        fprintf(fpt,"\nIT Service 2 Status is . . . ONLINE\n");
        fclose(fpt);
    }

    signal(SIGRECONFIG, IT2_SigHandler); // signal handlers ( CODE FROM alarm1.c )
    signal(SIGSNMP, IT2_SigHandler);
    signal(SIGALRM, IT2_SigHandler);

    while(1) // wait for monitor signals
        pause();
}

/* The function ITService3 is the service 3 terminal things. Everything done for service three is here.
 *
 * input:
 *     *dev: terminal name
 *
 * return value: N/A
 *
 */
void ITService3(char *dev) {
    FILE *fpt;
    int seed = time(NULL); srand48(seed); // random seed

    alarm(TRUE);

    if((fpt = fopen(dev, "w")) != NULL) {
        fprintf(fpt,"\nIT Service 3 Status is . . . ONLINE\n");
        fclose(fpt);
    }

    signal(SIGRECONFIG, IT3_SigHandler); // signal handlers ( CODE FROM alarm1.c )
    signal(SIGSNMP, IT3_SigHandler);
    signal(SIGALRM, IT3_SigHandler);

    while(1) // wait for monitor signals
        pause();
}

/* The function Task_Monitor is the primary driver of this MP. This is where the signals are fed to the other services.
 *
 * input:
 *     servicePIDs[TERM_COUNT]: type int, this is an array of service PIDs
 *
 * return value: N/A
 *
 */
void Task_Monitor(int servicePIDs[TERM_COUNT]) {

    char command[3]; // Max command length is 2 (plug null character)
    unsigned char serviceNumber;

    signal(SIGINT, Monitor_SignalHandler); // CTRL+C
    signal(SIGKILL, Monitor_SignalHandler); // KILL

    printf("\nTask Monitor status is . . . ONLINE\n\n");

    printf("-------------- AVAILABLE COMMANDS --------------\n");
    printf("sn  -  Sends an SNMP request to IT service n where n is one of 1, 2, or 3.\n");
    printf("rn  -  Sends a reconfiguration script to IT service n.\n");
    printf("kn  -  Instructs IT service n to shutdown (the process is killed).\n");
    printf("------------------------------------------------\n");

    while(1) {
        scanf("%s", command);
        serviceNumber = (int)atoi((const char *) &command[1]); // We want the second character in the command so
                                                               // s1 <- the 1 is the service number
        if(strlen(command) != 2 || (command[0] != 's' && command[0] != 'r' && command[0] != 'k')) {
            printf("\nInvalid command. Please choose from the below commands...\n\n");
            printf("sn  -  Sends an SNMP request to IT service n where n is one of 1, 2, or 3.\n");
            printf("rn  -  Sends a reconfiguration script to IT service n.\n");
            printf("kn  -  Instructs IT service n to shutdown (the process is killed).\n\n");
        }
        else if(serviceNumber > TERM_COUNT-1 || serviceNumber < 1 )
            printf("\nThe only valid IT Service numbers are between 1 and 3.\n\n");

        else { // finally a valid input...
            switch(command[0]) { // command[0] is the first letter of the command string
                case 's': // If the first letter is 's' then we will signal SNMP to the designated terminal
                    kill(servicePIDs[serviceNumber], SIGSNMP);
                    break;
                case 'r': // If the first letter is 'r' then we will signal reconfigure to the designated terminal
                    kill(servicePIDs[serviceNumber], SIGRECONFIG);
                    break;
                case 'k': // If the first letter is 'k' then we will signal kill to the designated terminal
                    kill(servicePIDs[serviceNumber], SIGKILL);
                    printf("\nTerminate IT Service %d\n\n", serviceNumber);
                    break;
                default:
                    break;
            }
        }
    }

    exit(0); // VERY HAPPY EXIT :)
}