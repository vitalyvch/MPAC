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
 *
 * File:   mpac_sniffer_app.c
 * Author: Ghulam Mustafa, Abdul Qadeer
 *
 * Created on Oct 08, 2009
 */

#include "mpac.h"
#include "mpac_sniffer.h"


sniff_cntx dcntx;
wrkr_cntx  **wcntx;
mpac_thread_manager_t tm;
pthread_t time_keeper;
pthread_attr_t attr;
sniff_cntx* pckts;
double start_time, end_time;
int ctrl_c;
long long int  data;

int main(int argc, char** argv)
{
    start_time = end_time = 0.0; ctrl_c = 0;
    data = 0;
    mpac_signal_sighandler(SIGINT, mpac_sniffer_cleanup);
    mpac_sniffer_arg_handler(argc , argv, &dcntx);

    if (dcntx.execution_mode == 5)
      {        
        fprintf(stdout,"Enter word to be matched in payload (max. 50 characters):");        
        fscanf(stdin,"%s",dcntx.toFind);
        dcntx.toFindStringSize = strlen(dcntx.toFind);        
      }
    pckts = (sniff_cntx*) malloc (dcntx.Q_len * sizeof(sniff_cntx));
    wcntx = mpac_sniffer_configure(&dcntx);
    
    int i;
    for (i = 0; i < dcntx.Q_len; i++) {
        pckts[i] = dcntx;
        pckts[i].empty = 1;
  }
    mpac_thread_manager_init (&tm,dcntx.numwrkr,&attr,dcntx.affinity,
                                        mpac_sniffer_worker,(void**)wcntx );
    
    mpac_thread_manager_fork(&tm);
    mpac_sniff_cntx_init(&dcntx);

   for (i = 0; i < dcntx.Q_len; i++)
       pckts[i].rawSock = dcntx.rawSock;
   start_time = mpac_time_ggettime();
   pthread_create(&time_keeper,NULL,mpac_sniffer_time_keeper,(void*)&dcntx.sniff_time);
    while (1)
    {        
        for (i = 0; i < dcntx.Q_len; i++) {
        if (pckts[i].empty) {
            mpac_sniff_rawpacket(&pckts[i]);     
            pckts[i].empty = 0;                                                // if (GM_DBG) printf("*");
            data += pckts[i].rawPacketLength;
        }
        }
    }
    mpac_thread_manager_wait (&tm);
    mpac_thread_manager_free(&tm);    
    mpac_sniffer_cleanup();
    return 0;
}

void mpac_sniffer_cleanup()
{
    ctrl_c = 1;
    end_time = mpac_time_ggettime();
    double elapsed_time;
    elapsed_time = end_time - start_time;
    
    printf("\nReleasing resources...\n");
    int i, j, pprocessed = 0;
    long long int bprocessed = 0.0;
    for (j = 0; j < dcntx.numwrkr; j++){
        pprocessed += wcntx[j]->num_pckts_processed;
        bprocessed += wcntx[j]->bytes;
    }
        

    mpac_sniffer_output(elapsed_time, pprocessed, bprocessed);
    mpac_sniff_closedown(&dcntx);
    
    for (i = 0; i < dcntx.numwrkr; i++) free(wcntx[i]);
    free(wcntx);
    free(pckts);
    exit (0);
}

void* mpac_sniffer_worker (void* arg)
{
    wrkr_cntx* cntx = (wrkr_cntx*)arg;
    int e_mode = dcntx.execution_mode;
    int i, myID, wrkrs;
    //double bytes;
    myID = cntx->wrkrID;
    wrkrs = cntx->numWrkr;
    
if (GM_DBG) printf("_(T%d)_\n",myID );
    while( ctrl_c == 0)//mpac_signal_getsigflag() == 0)
    {
        for (i = myID; i < dcntx.Q_len; i= i+wrkrs)
        {
            if(pckts[i].empty == 0 && pckts[i].ethHeader->ether_type == ETHER_TYPE_IP){
                //if (GM_DBG) printf(".");
            switch (e_mode)
            {
                case 3:
                    cntx->num_pckts_processed++;
                    cntx->bytes += pckts[i].rawPacketLength;
                    pckts[i].empty = 1;
                    if(ctrl_c) return NULL;
                    break;
                case 4:
                    mpac_sniff_tcppacket_5tuple_comparison(&pckts[i]);
                    cntx->num_pckts_processed++;
                    cntx->bytes += pckts[i].rawPacketLength;
                    pckts[i].empty = 1;                    
                    if(ctrl_c) return NULL;
                    break;
                case 5:
                    mpac_sniff_do_some_work(&pckts[i]);
                    mpac_sniff_find_stringinpayload (&pckts[i]);
                    cntx->num_pckts_processed++;
                    cntx->bytes += pckts[i].rawPacketLength;
                    pckts[i].empty = 1;
                    if(ctrl_c) return NULL;
                    break;
                default:                    
                    break;                        
            }
            }
        }
    }
    return NULL;
}

void* mpac_sniffer_time_keeper(int* duration)
{    
    int time = (int)*duration;

    sleep(time);
     ctrl_c = 1;
    printf("\nReleasing resources...\n");
    int i, j, pprocessed = 0;
    long long int bprocessed = 0.0;
    for (j = 0; j < dcntx.numwrkr; j++ ){
        pprocessed += wcntx[j]->num_pckts_processed;
        bprocessed += wcntx[j]->bytes;
    if (GM_DBG)     printf("Work %d: %lli\n",j,wcntx[j]->bytes);
    if (GM_DBG)     printf("Total %d: %lli\n",j,bprocessed);
    }
    if (GM_DBG) printf("Dispacher:%lli\n", data);
    double tp;
    tp = (((double) data / 1000000.0) * 8)/time;
    if (GM_DBG) printf("Throughput: %0.2f\n",tp);
    mpac_sniffer_output(time, pprocessed, tp);
    mpac_sniff_closedown(&dcntx);
    for (i = 0; i < dcntx.numwrkr; i++) free(wcntx[i]);
    free(wcntx);
    mpac_thread_manager_wait (&tm);
    mpac_thread_manager_free(&tm);
    free(pckts);
    exit (0);
}
