///
/// @file trnusvr-test.c
/// @authors k. Headley
/// @date 12 jun 2019

/// Test server for trnu clients

/// Compile test using
/// gcc -DWITH_NETIF_TEST -o netif-test netif-test.c netif.c -L../bin -lmframe
/// @sa doxygen-examples.c for more examples of Doxygen markup

/////////////////////////
// Terms of use
/////////////////////////
/*
 Copyright Information

 Copyright 2002-2019 MBARI
 Monterey Bay Aquarium Research Institute, all rights reserved.

 Terms of Use

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 3 of the License, or
 (at your option) any later version. You can access the GPLv3 license at
 http://www.gnu.org/licenses/gpl-3.0.html

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details
 (http://www.gnu.org/licenses/gpl-3.0.html)

 MBARI provides the documentation and software code "as is", with no warranty,
 express or implied, as to the software, title, non-infringement of third party
 rights, merchantability, or fitness for any particular purpose, the accuracy of
 the code, or the performance or results which you may obtain from its use. You
 assume the entire risk associated with use of the code, and you agree to be
 responsible for the entire cost of repair or servicing of the program with
 which you are using the code.

 In no event shall MBARI be liable for any damages, whether general, special,
 incidental or consequential damages, arising out of your use of the software,
 including, but not limited to, the loss or corruption of your data or damages
 of any kind resulting from use of the software, any prohibited use, or your
 inability to use the software. You agree to defend, indemnify and hold harmless
 MBARI and its officers, directors, and employees against any claim, loss,
 liability or expense, including attorneys' fees, resulting from loss of or
 damage to property or the injury to or death of any person arising out of the
 use of the software.

 The MBARI software is provided without obligation on the part of the
 Monterey Bay Aquarium Research Institute to assist in its use, correction,
 modification, or enhancement.

 MBARI assumes no responsibility or liability for any third party and/or
 commercial software required for the database or applications. Licensee agrees
 to obtain and maintain valid licenses for any additional third party software
 required.
 */

/////////////////////////
// Headers
/////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "netif.h"
#include "trn_msg.h"
#include "trnw.h"
#include "trnif_proto.h"

#include "mframe.h"
#include "medebug.h"
#include "mmdebug.h"

/////////////////////////
// Macros
/////////////////////////

// These macros should only be defined for
// application main files rather than general C files
/*
 /// @def PRODUCT
 /// @brief header software product name
 #define PRODUCT "TBD_PRODUCT"

 /// @def COPYRIGHT
 /// @brief header software copyright info
 #define COPYRIGHT "Copyright 2002-2013 MBARI Monterey Bay Aquarium Research Institute, all rights reserved."
 /// @def NOWARRANTY
 /// @brief header software terms of use
 #define NOWARRANTY  \
 "This program is distributed in the hope that it will be useful,\n"\
 "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"\
 "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"\
 "GNU General Public License for more details (http://www.gnu.org/licenses/gpl-3.0.html)\n"
 */

#define TRNUST_HOST_DFL "localhost"
#define TRNUST_PORT_DFL 8000
#define TRNUST_LOGDIR_DFL "."
#define TRNUST_MOD_DFL 3
#define TRNUST_HBTO_DFL 0.0
#define TRNUST_VERBOSE_DFL 0

/////////////////////////
// Declarations
/////////////////////////
typedef struct app_cfg_s{
    int verbose;
    netif_t *netif;
    char *host;
    int port;
    char *logdir;
    int update_mod;
    double hbto;
}app_cfg_t;


bool g_interrupt=false;

/////////////////////////
// Imports
/////////////////////////

/////////////////////////
// Module Global Variables
/////////////////////////

/////////////////////////
// Function Definitions
/////////////////////////

/// @fn void s_show_help()
/// @brief output user help message to stdout.
/// @return none
static void s_show_help()
{
    char help_message[] = "\ntrnif server unit test\n";
    char usage_message[] = "\ntrnusvr-test [options]\n"
    "--verbose=n    : verbose output, n>0\n"
    "--help         : output help message\n"
    "--version      : output version info\n"
    "--host=ip:n    : TRN server host:port\n"
    "--mod=n        : update modulus\n"
    "--hbto=f       : hbeat tiemout\n"
    "--logdir=s     : logdir prefix      [*]\n"
    "[*] - required\n"
    "\n";
    printf("%s",help_message);
    printf("%s",usage_message);
}
// End function s_show_help

/// @fn void parse_args(int argc, char ** argv, app_cfg_t * cfg)
/// @brief parse command line args, set application configuration.
/// @param[in] argc number of arguments
/// @param[in] argv array of command line arguments (strings)
/// @param[in] cfg application config structure
/// @return none
void parse_args(int argc, char **argv, app_cfg_t *cfg)
{
    extern char WIN_DECLSPEC *optarg;
    int option_index;
    int c;
    bool help=false;
    bool version=false;

    static struct option options[] = {
        {"verbose", required_argument, NULL, 0},
        {"help", no_argument, NULL, 0},
        {"version", no_argument, NULL, 0},
        {"host", required_argument, NULL, 0},
        {"mod", required_argument, NULL, 0},
        {"hbto", required_argument, NULL, 0},
        {"logdir", required_argument, NULL, 0},
        {NULL, 0, NULL, 0}};

    // process argument list
    while ((c = getopt_long(argc, argv, "", options, &option_index)) != -1){
        switch (c) {
            // long options all return c=0
            case 0:
            // verbose
            if (strcmp("verbose", options[option_index].name) == 0) {
                sscanf(optarg,"%d",&cfg->verbose);
            }

            // help
            else if (strcmp("help", options[option_index].name) == 0) {
                help = true;
            }

            // version
            else if (strcmp("version", options[option_index].name) == 0) {
                version = true;
            }

            // host
            else if (strcmp("host", options[option_index].name) == 0) {

                char *hsave=cfg->host;
                char *ocopy=strdup(optarg);
                char *hstr=strtok(ocopy,":");

                if (hstr==NULL) {
                    cfg->host=strdup(TRNUST_HOST_DFL);
                }else{
                    cfg->host=strdup(hstr);
                }
                char *ip = strtok(NULL,":");
                if (ip!=NULL) {
                    sscanf(ip,"%d",&cfg->port);
                }
                // don't free ocopy here
                if(hsave!=NULL){
                    free(hsave);
                }
            }
            // logdir
            else if (strcmp("logdir", options[option_index].name) == 0) {
                if(NULL!=cfg->logdir)free(cfg->logdir);
                cfg->logdir=(NULL==optarg?NULL:strdup(optarg));

            }
            // update_mod
            else if (strcmp("mod", options[option_index].name) == 0) {
                sscanf(optarg,"%d",&cfg->update_mod);
            }
            // hbto
            else if (strcmp("hbto", options[option_index].name) == 0) {
                sscanf(optarg,"%lf",&cfg->hbto);
            }
            break;
            default:
            help=true;
            break;
        }
        if (version) {
            fprintf(stderr,"no version\n");
            exit(0);
        }
        if (help) {
            s_show_help();
            exit(0);
        }
    }// while

    fprintf(stderr,"verbose   [%d]\n",cfg->verbose);
    fprintf(stderr,"host      [%s]\n",cfg->host);
    fprintf(stderr,"port      [%d]\n",cfg->port);
    fprintf(stderr,"logdir    [%s]\n",cfg->logdir);
    fprintf(stderr,"mod       [%d]\n",cfg->update_mod);
    fprintf(stderr,"hbto      [%lf]\n",cfg->hbto);

}
// End function parse_args

/// @fn void termination_handler (int signum)
/// @brief termination signal handler.
/// @param[in] signum signal number
/// @return none
static void s_termination_handler (int signum)
{
    switch (signum) {
        case SIGINT:
        case SIGHUP:
        case SIGTERM:
        fprintf(stderr,"\nsig received[%d]\n",signum);
        g_interrupt=true;
        break;
        default:
        fprintf(stderr,"\ns_termination_handler: sig not handled[%d]\n",signum);
        break;
    }
}
// End function termination_handler

static app_cfg_t *app_cfg_new()
{
    app_cfg_t *instance=(app_cfg_t *)malloc(sizeof(app_cfg_t));
    if(NULL!=instance){
        memset(instance,0,sizeof(app_cfg_t));
        instance->netif=NULL;
        instance->logdir=strdup(TRNUST_LOGDIR_DFL);
        instance->host=strdup(TRNUST_HOST_DFL);
        instance->port=TRNUST_PORT_DFL;
        instance->verbose=TRNUST_VERBOSE_DFL;
        instance->update_mod=TRNUST_MOD_DFL;
    }
    return instance;
}
// End function app_cfg_new

static void app_cfg_destroy(app_cfg_t **pself)
{
    if(NULL!=pself){
        app_cfg_t *self=*pself;
        if(NULL!=self){
            if(NULL!=self->netif){
                netif_destroy(&self->netif);
            }
            if(NULL!=self->host){
                free(self->host);
            }
            if(NULL!=self->logdir){
                free(self->logdir);
            }
            free(self);
            *pself=NULL;
        }
    }
    return;
}
// End function app_cfg_destroy

static int s_trnu_pub(trnu_pub_t *update, netif_t *trnusvr)
{
    int retval=-1;

    if(NULL!=update && NULL!=trnusvr){
        retval=0;
        int iobytes=0;

        if( (iobytes=netif_pub(trnusvr,(char *)update, sizeof(trnu_pub_t)))>0){
            retval=iobytes;
        }
    }

    return retval;
}
// End function s_trnu_pub
#define SESSION_BUF_LEN 16

static char *s_session_str(char **pdest, size_t len)
{
    static char session_date[SESSION_BUF_LEN] = {0};
    char *retval=session_date;

    // lazy initialize session time string to use
    // in log file names
    time_t rawtime;
    struct tm *gmt;

    time(&rawtime);
    // Get GMT time
    gmt = gmtime(&rawtime);

    // format YYYYMMDD-HHMMSS
    sprintf(session_date, "%04d%02d%02d-%02d%02d%02d", (gmt->tm_year + 1900), gmt->tm_mon + 1, gmt->tm_mday, gmt->tm_hour,
            gmt->tm_min, gmt->tm_sec);

    if(NULL!=pdest){
        // return requested
        if(NULL==*pdest){
            *pdest=strdup(session_date);
            retval=*pdest;
        }else {
            if(len>=SESSION_BUF_LEN){
                sprintf(*pdest,"%s",session_date);
                retval=*pdest;
            }else{
                fprintf(stderr,"ERR - dest buffer too small");
            }
        }
    }
    return retval;
}
// End function s_session_str

static int s_init_trnusvr(app_cfg_t *cfg, bool verbose)
{
    int retval = -1;
    if(NULL!=cfg && NULL!=cfg->host){
        fprintf(stderr,"configuring trnu server socket using [%s:%d]\n",cfg->host,cfg->port);
        cfg->netif = netif_new("trnusvr",cfg->host,
                               cfg->port,
                               ST_UDP,
                               IFM_REQRES,
                               cfg->hbto,
                               trnif_msg_read_trnu,
                               trnif_msg_handle_trnu,
                               trnif_msg_pub_trnu);


        if(NULL!=cfg->netif){
            netif_set_reqres_res(cfg->netif,NULL);//trn instance
            fprintf(stderr,"trnusvr netif:\n");
            netif_show(cfg->netif,true,5);
            netif_init_log(cfg->netif, "trnusvr", (NULL!=cfg->logdir?cfg->logdir:"."), s_session_str(NULL,0));
            mlog_tprintf(cfg->netif->mlog_id,"*** trnusvr session start (TEST) ***\n");
            mlog_tprintf(cfg->netif->mlog_id,"libnetif v[%s] build[%s]\n",netif_get_version(),netif_get_build());
            retval = netif_connect(cfg->netif);
        }else{
            fprintf(stderr,"%s:%d - ERR allocation\n",__FUNCTION__,__LINE__);
        }
    }else{
        fprintf(stderr,"%s:%d - ERR invalid args\n",__FUNCTION__,__LINE__);
    }
    return retval;
}
// End function s_init_trnusvr

static void s_advance_update(trnu_pub_t *update)
{
    static int count=0;

    if(NULL!=update){
        update->est[TRNU_EST_PT].time=mtime_dtime();
        update->est[TRNU_EST_MLE].time=mtime_dtime();
        update->est[TRNU_EST_MMSE].time=mtime_dtime();
        update->mb1_cycle++;
        update->ping_number++;
        update->mb1_time=mtime_dtime();
        update->update_time=mtime_dtime();

        update->filter_state=count%5;
        if( (count%3)==0){
            update->reinit_count++;
            update->success=update->success==0?1:0;
            update->is_converged=update->is_converged==0?1:0;
            update->is_valid=update->is_valid==0?1:0;
        }
        count++;
    }
    return;
}
// End function s_advance_update

static int s_run(app_cfg_t *cfg)
{
    int retval=-1;
    if(NULL!=cfg){
        trnu_pub_t update_s={
            TRNU_PUB_SYNC,
            {
                // pt info {time,x,y,z,{cov[0],cov[2],cov[5],cov[1]}}
                {mtime_dtime(),0.0,0.0,0.0,
                    {0.0,0.0,0.0,0.0}
                },
                // mle info {time,x,y,z,{cov[0],cov[2],cov[5],cov[1]}}
                {mtime_dtime(),0.0,0.0,0.0,
                    {0.0,0.0,0.0,0.0}
                },
                // mmse info {time,x,y,z,{cov[0],cov[2],cov[5],cov[1]}}
                {mtime_dtime(),0.0,0.0,0.0,
                    {0.0,0.0,0.0,0.0}
                },
            },
            0,// reinit_count
            0.0,// reinit_tlast
            0,// filter_state
            0,// success
            0,// is_converged
            0,// is_valid
            0,// mb1_cycle
            0,// ping_number
            mtime_dtime(),// mb1_time
            mtime_dtime(),// update_time
        };
        trnu_pub_t *update=&update_s;
        int count=0;
        fprintf(stderr,"trnusvr waiting for connection...(CTRL-C to exit)\n");
        while(!g_interrupt){
            // server: connect to client
            netif_update_connections(cfg->netif);

            // server: get TRN_MSG_PING, return TRN_MSG_ACK
            netif_reqres(cfg->netif);
            sleep(1);
            if((count%cfg->update_mod)==0){
                s_advance_update(update);
                s_trnu_pub(update,cfg->netif);
            }
            count++;
        }
        fprintf(stderr,"interrupted by user - returning\n");
        retval = 0;
    }
    return retval;
}


static int s_app_main(app_cfg_t *cfg)
{
    int retval=-1;

    if(NULL!=cfg){

        signal(SIGINT,s_termination_handler);

        s_init_trnusvr(cfg,true);

        if(NULL!=cfg->netif ){
            double start_time=mtime_dtime();

            netif_init_mmd();
            netif_show(cfg->netif,true,5);

            // initialize message log
            int il = netif_init_log(cfg->netif, NETIF_MLOG_NAME, NULL,NULL);
            fprintf(stderr,"netif_init_log returned[%d]\n",il);

            mlog_tprintf(cfg->netif->mlog_id,"*** netif session start (TEST) ***\n");
            mlog_tprintf(cfg->netif->mlog_id,"libnetif v[%s] build[%s]\n",netif_get_version(),netif_get_build());

            // server: open socket, listen
            int nc = netif_connect(cfg->netif);
            fprintf(stderr,"netif_connect returned[%d]\n",nc);

            // test trn_server/commsT protocol
            s_run(cfg);

            mlog_tprintf(cfg->netif->mlog_id,"*** netif session end (TEST) uptime[%.3lf] ***\n",(mtime_dtime()-start_time));

            mmd_release();

            retval=0;
        }else{
            fprintf(stderr,"component allocation failed netif[%p]\n",cfg->netif);
        }
    }else{
        fprintf(stderr,"invalid argument\n");
    }
    return retval;
}


int main(int argc, char **argv)
{
    int retval=-1;

    app_cfg_t *cfg=app_cfg_new();

    if(NULL!=cfg){
        parse_args(argc,argv,cfg);

        retval=s_app_main(cfg);

        app_cfg_destroy(&cfg);
    }
    //#ifdef WITH_NETIF_TEST
    //    netif_test();
    //#else
    //    fprintf(stderr,"netif_test not implemented - compile using -DNETIF_WITH_TEST (WITH_NETIF_TEST=1 make...)\r\n");
    //#endif
    return retval;
}
