/**************************************************************************
 *
 * Copyright 2008 HPCNL-KICS, UET, Lahore, PAKISTAN.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification,are permitted provided that the following conditions 
 * are met:
 *
 *    1. Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *    2. Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY HPCNL-KICS ``AS IS'' AND ANY EXPRESS OR 
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
 * IN NO EVENT SHALL HPCNL-KICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
 * WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 * 
 * The views and conclusions contained in the software and documentation are
 * those of the authors and should not be interpreted as representing official
 * policies, either expressed or implied, of HPCNL-KICS.
 *
 ***************************************************************************** 
 */

/**
  		mpac_net.h
  @author	Ghulam Mustafa
  
  @doc
  Sample MPAC network microbenchmark.

  This file contains the prototype of the functions which are used 
  to build a sample network benchmark by usind {\bf MPAC} library.
*/

#ifndef _MPAC_NET_H

/** */
#define	_MPAC_NET_H
#include"mpac.h"
#include "mpac_io_net.h"

#define NET_DBG 0
/**
 * 
 * Network benchmark configuration. User input data structure.
 */
struct mpac_net_config_t
{               
     /*@{*/    
                            /** 
                             * 
                             * Communication type (1 for sender, 2 for receiver). */
    int com_type;           
    int  sockopt;            /*Flag for overwriting default socket buffers sizes*/
    char    torm;            /*time based or message count based execution*/
    int interval;
                            /*
                             * 
                             * Size of buffer. */
    long buf_size;          
                            /**
                             * 
                             * Number of threads. */
    int num_thrs;           
                            /**
                             * 
                             * Number of cores. */
    int num_cores;          
                            /**
                             * 
                             * Protocol number: 0 for TCP, 1 for UDP ,2 for RAW. */
    int protocol_num;       
                            /**
                             * 
                             * Port number. */
    int port_num;           
                            /**
                             * 
                             * IP address. */
    char ip[20];               
                            /**
                             * 
                             * Affinity suporrt flag i.e. Required(1)/notRequired(0). */
    unsigned int aff;       
                            /**
                             * 
                             * Number of messages. */
    int num_msg;            
                            /**
                             * 
                             * Size of message. */
    unsigned long msg_sz;   
    /*@}*/
    
} ;

/**
 * 
 * Thread argument structure.
 */
struct mpac_net_context_t
{
    /*@{*/
                                    /**
                                     * 
                                     * A network handler.*/
    struct mpac_io_net_handler*   h;
                                    /**
                                     * 
                                     * Number of messages. */
    int                      n_msgs;
                                    /**
                                     * 
                                     * Number of bytes read. */
    int                    interval;
    unsigned long           bytes_x;
                                    /**
                                     * 
                                     * Thread id. */
    int                      thr_id;
                                    /**
                                     * 
                                     * Number of threads. */
    int                   num_wrkrs;
                                    /**
                                     *
                                     * Total time used to send the data. */
    double            u_time;    
    /*@}*/
 } ;

 /**
  * 
  * Thread data structure.
  */
 struct mpac_net_thdata
 {   
     /*@{*/         /** 
                      * 
                      * Encapsulates locking mechanism for collaborative work.*/
      mpac_lock_t fdlck;
                    /**
                     * 
                     * Number of threads. */
     int num_th;
                    /**
                     * 
                     * File descriptor. */
     int fd;    
                    /**
                     * 
                     * An array of file descriptors.*/
     int* fds;
                    /**
                     *
                     */
     int getfd;
                    /**
                     *
                     */
     int setfd;
     /*@}*/
 } ;
 
  /**
  * 
  * Attribute of thread data structure {\bf mpac_net_thdata}.
  */
 struct mpac_net_thdata apdata;
   
 /**
  *
  * This function will print different usage options to guide the user.
  * For example {\bf -b} option will be used for buffer size, 
  *             {\bf -i} option will be used for IP address.
  *
  * @param prog_name A character poiner that will contain the name of the running program.
  * @return void.
  */
void mpac_net_usage(char *prog_name);

/**
 *
 *
 * This function will print current configurations for network benchmark.
 *
 * @param net_config A pointer to {\bf struct mpac_net_config_t} that will contain 
 *                   information like buffer size, number of threads,
 *                   port number, IP address etc.
 * @return void.
 */
void mpac_net_print_config(struct mpac_net_config_t *net_config);

/**
 *
 * This function will set the default configurations for network benchmark
 * (if the user does not specify the required options).
 *
 * @param net_config A pointer to {\bf mpac_net_config_t} that will 
 *                   contain the values for default options.
 * @return void.
 */
void mpac_net_default_config(struct mpac_net_config_t *net_config);

/**
 *
 * Parses commandline arguments as well as initializes them as needed.
 * It first calls the {\bf mpac_net_default_config()} to make sure
 * that all config parameters have valid values.
 * This function will return {\bf MPAC_FAILURE} if the user specify 
 * any invalid option or the function will return {\bf MPAC_SUCCESS}
 * after successfully parsing all the arguments.
 *
 * @param argc Integer parameter to store count for number of arguments.
 * @param argv Double character pointer to the command line arguments.
 * @param net_config A pointer to {\bf struct mpac_net_config_t} that will 
 *                   contain the values after parsing the command line arguments.
 * @return       MPAC_SUCCESS or MPAC_FAILURE.
 */
int  mpac_net_parse_cmdl(int argc, char **argv, 
			 struct mpac_net_config_t *net_config);



 

/**
 *
 * This function will print the result after testing the network benchmark.
 * The result will include information about throughput and latency time.
 *
 * @param el_time For elapsed time in seconds
 * @param mbps    For throughput in mega bits per second.
 * @param mps     For throughput in number of messages per second.
 * @param lpm     For latency per message in micro seconds.
 * @return void.
 */
void mpac_net_output(double el_time, double mbps, double mps, double lpm);





/**
 *
 * This function is used to initialize the {\bf net_config} handler.
 * If the passed arguments are less than two, then it will initialize the
 * {\bf net_config} with default settings by calling the function {\bf mpac_net_default_config()}.
 * 
 * If the passed arguments are greater than two, then it will initialize the
 * {\bf net_config} with the user specified settings by calling the function 
 * {\bf mpac_net_parse_cmdl()}.
 * 
 * If it successfully initialize the {\bf net_config} handler then it will 
 * return {\bf MPAC_SUCCESS}.
 * If the user specify any invalid option then it
 * will terminate the program by calling {\bf exit()}.
 *
 *
 * @param argc Integer parameter to store count for number of arguments.
 * @param argv Double character pointer to the command line arguments.
 * @param net_config A pointer to {\bf mpac_net_config_t} that will 
 *                   contain the values after parsing the command line arguments.
 * @return       MPAC_SUCCESS.
 */
int mpac_net_arg_handler(int argc, char** argv, struct mpac_net_config_t* net_config);





/**
 *
 * This functoin is used to set up the signal handler to {\bf mpac_net_cleanup()}
 * function. If the user terminate the program during execution then, this function
 * calls {\bf mpac_net_cleanup()} function to deallocate all the allocated resources.
 *
 * @param void.
 * @return void.
 *  
 */
void mpac_net_sighandler();




/**
 *
 * This function creates an array of {\bf mpac_net_context_t}. The size of the 
 * array is equal to the total number of threads specified in {\bf net_config.num_thrs}.
 *
 * @param net_config A pointer to {\bf mpac_net_config_t} that will 
 *                   contain the values after parsing the command line arguments.
 * @return A double pointer of type {\bf mpac_net_context_t}.
 */
struct mpac_net_context_t** mpac_net_configure(struct mpac_net_config_t net_config);

/**
 *
 * This function will decrement the value of static variable {\bf mpac_net_msgcount}
 * by one. 
 * If successful, functions(like {\bf pthread_mutex_unlock()} 
 * will return zero, otherwise an error number will be returned to indicate the error.
 *
 * @param void
 * @return Zero or Error number.
 */
int mpac_net_decrementmc(int);

/**
 *
 * This function will increment the value of static variable {\bf mpac_net_msgcount}
 * by one and the incremented value will be assigned to the parameter {\bf currentmc}. 
 * If successful, functions(like {\bf pthread_mutex_unlock()}) 
 * will return zero, otherwise an error number will be returned to indicate the error.
 *
 * @param currentmc Integer pointer that will store the value of 
 *        incremented static variable {\bf mpac_net_msgcount}.
 * @return Zero or Error number.
 */
int mpac_net_incrementmc(int* currentmc);

/**
 *
 * This function will assign the value of static variable {\bf mpac_net_msgcount}
 * to the parameter {\bf mc}. If successful, functions(like {\bf pthread_mutex_unlock()}) 
 * will return zero, otherwise an error number will be returned to indicate the error.
 *
 * @param mc Integer pointer that will store the value of static variable 
 *           {\bf mpac_net_msgcount}.
 * @return Zero or Error number.
 */
int mpac_net_getmc();

/**
 *
 * This function assign the value of {\bf mc} to the static variable {\bf mpac_net_msgcount}.
 * 
 * If {\bf mpac_net_msgcount} is not equal to zero, then it will return {\bf MPAC_FAILURE}.
 * If {\bf mpac_net_msgcount} is equal to zero, then it will return {\bf MPAC_SUCCESS}.
 * 
 *
 * @param mc Integet value which will be assigned to the static variable 
 *           {\bf mpac_net_msgcount}.
 * @return   MPAC_SUCCESS or MPAC_FAILURE 
 */
int mpac_net_setmc(int mc);

/**
 *
 * This function will generate a random messsecondage.
 * The message(in bytes) will be equal to the passed parameter {\bf size}.
 * 
 * If the passed parameter {\bf buff} conatins null then it will return {\bf MPAC_FAILURE},
 * otherwise it will return {\bf  MPAC_SUCCESS} after successfully populating the message.
 * 
 * 
 *
 * @param buff      A pointer to the buffer, which will contain the generated message.
 * @param size      Size of message to be generated in bytes.
 * @return      MPAC_SUCCESS or MPAC_FAILURE
 */
int mpac_net_generatemsg(char* buff,int size);

/**
 *  
 *  This function will release all the resources, 
 *  occupied by the Network Benchmark and will call {\bf exit()} function on
 *  its completion. 
 *
 * @param void.
 * @return void.
 */
void mpac_net_cleanup();
void mpac_net_sendsig ();

/**
 *
 * It initializes {\bf td->mpac_lock_t} and {\bf td->fd}.
 *
 * @param td Thread data structure.
 * @param void.
 */
void mpac_net_init_recv(struct mpac_net_thdata* td);

/**
 *
 * It initializes {\bf td->mpac_lock_t} and {\bf td->fd}.
 *
 * @param td Thread data structure.
 * @param MPAC_SUCCESS or MPAC_FAILURE.
 * @return .
 */
int mpac_net_setafd (struct mpac_net_thdata* td);

/**
 *
 * It initializes {\bf td->mpac_lock_t} and {\bf td->fd}.
 *
 * @param td Thread data structure.
 * @param Current available file descriptor.
 * @return .
 */
int mpac_net_getafd (struct mpac_net_thdata* td);

void* mpac_net_tcpreceiver (void* arg);
void* mpac_net_tcpsender(void* arg);
void* mpac_net_udpreceiver(void* arg);
void* mpac_net_udpsender(void* arg);
void* mpac_net_tcpreceiver_timed (void* arg);
void* mpac_net_tcpsender_timed(void* arg);
void* mpac_net_udpreceiver_timed(void* arg);
void* mpac_net_udpsender_timed(void* arg);
void mpac_net_setsigflag();
int mpac_net_getsigflag();
void  mpac_net_tcp_report(struct mpac_net_config_t, struct mpac_net_context_t**);
void  mpac_net_udp_report(struct mpac_net_config_t, struct mpac_net_context_t**);
#endif	/* _MPAC_NET_H */

