/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _APPEND_H_RPCGEN
#define _APPEND_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


#define APPENDPROG 0x12340001
#define APPENDVERS 1

#if defined(__STDC__) || defined(__cplusplus)
#define RPCINITAPPENDSERVER 1
extern  int * rpcinitappendserver_1(char **, CLIENT *);
extern  int * rpcinitappendserver_1_svc(char **, struct svc_req *);
#define RPCAPPEND 2
extern  int * rpcappend_1(char **, CLIENT *);
extern  int * rpcappend_1_svc(char **, struct svc_req *);
extern int appendprog_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define RPCINITAPPENDSERVER 1
extern  int * rpcinitappendserver_1();
extern  int * rpcinitappendserver_1_svc();
#define RPCAPPEND 2
extern  int * rpcappend_1();
extern  int * rpcappend_1_svc();
extern int appendprog_1_freeresult ();
#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_APPEND_H_RPCGEN */