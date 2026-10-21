/* ************************************************************************
*   File: comm.c                                        Part of CircleMUD *
*  Usage: Communication, socket handling, main(), central game loop       *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

#define __COMM_C__

#include "conf.h"
#include "sysdep.h"

#if CIRCLE_GNU_LIBC_MEMORY_TRACK
# include <mcheck.h>
#endif

#ifdef CIRCLE_MACINTOSH
# define SIGPIPE 13
# define SIGALRM 14
# include <sys/ioctl.h>
# include <SIOUX.h>
# include <console.h>
#endif

#ifdef CIRCLE_WINDOWS
# ifdef __BORLANDC__
#  include <dir.h>
# else
#  include <direct.h>
# endif
# include <mmsystem.h>
#endif

#ifdef CIRCLE_AMIGA
# include <sys/ioctl.h>
# include <clib/socket_protos.h>
#endif

#ifdef CIRCLE_ACORN
# include <socklib.h>
# include <inetlib.h>
# include <sys/ioctl.h>
#endif

#include "structs.h"
#include "utils.h"
#include "comm.h"
#include "interpreter.h"
#include "handler.h"
#include "db.h"
#include "house.h"
#include "rpg/rpg_runtime_tick.h"

#ifdef HAVE_ARPA_TELNET_H
#include <arpa/telnet.h>
#else
#include "telnet.h"
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET (-1)
#endif

/* externs */
extern struct ban_list_element *ban_list;
extern int num_invalid;
extern char *GREETINGS;
extern const char *circlemud_version;
extern int circle_restrict;
extern int mini_mud;
extern int no_rent_check;
extern FILE *player_fl;
extern ush_int DFLT_PORT;
extern const char *DFLT_DIR;
extern const char *DFLT_IP;
extern const char *LOGNAME;
extern int max_playing;
extern int nameserver_is_slow;
extern int auto_save;
extern int autosave_time;
extern int *cmd_sort_info;
extern struct time_info_data time_info;
extern char *help;

/* local globals */
struct descriptor_data *descriptor_list = NULL;
struct txt_block *bufpool = 0;
int buf_largecount = 0;
int buf_overflows = 0;
int buf_switches = 0;
int circle_shutdown = 0;
int circle_reboot = 0;
int no_specials = 0;
int max_players = 0;
int tics = 0;
int scheck = 0;
struct timeval null_time;
byte reread_wizlist;
byte emergency_unban;
FILE *logfile = NULL;
const char *text_overflow = "**OVERFLOW**\r\n";

/* functions in this file */
RETSIGTYPE reread_wizlists(int sig);
RETSIGTYPE unrestrict_game(int sig);
RETSIGTYPE reap(int sig);
RETSIGTYPE checkpointing(int sig);
RETSIGTYPE hupsig(int sig);
ssize_t perform_socket_read(socket_t desc, char *read_point,size_t space_left);
ssize_t perform_socket_write(socket_t desc, const char *txt,size_t length);
void echo_off(struct descriptor_data *d);
void echo_on(struct descriptor_data *d);
void circle_sleep(struct timeval *timeout);
int get_from_q(struct txt_q *queue, char *dest, int *aliased);
void init_game(ush_int port);
void signal_setup(void);
void game_loop(socket_t mother_desc);
socket_t init_socket(ush_int port);
int new_descriptor(socket_t s);
int get_max_players(void);
int process_output(struct descriptor_data *t);
int process_input(struct descriptor_data *t);
void timediff(struct timeval *diff, struct timeval *a, struct timeval *b);
void timeadd(struct timeval *sum, struct timeval *a, struct timeval *b);
void flush_queues(struct descriptor_data *d);
void nonblock(socket_t s);
int perform_subst(struct descriptor_data *t, char *orig, char *subst);
void record_usage(void);
char *make_prompt(struct descriptor_data *point);
void check_idle_passwords(void);
void heartbeat(int pulse);
struct in_addr *get_bind_addr(void);
int parse_ip(const char *addr, struct in_addr *inaddr);
int set_sendbuf(socket_t s);
void setup_log(const char *filename, int fd);
int open_logfile(const char *filename, int stderr_fp);
#if defined(POSIX)
sigfunc *my_signal(int signo, sigfunc *func);
#endif

/* extern fcnts */
void reboot_wizlists(void);
void boot_world(void);
void affect_update(void);
void mobile_activity(void);
void perform_violence(void);
void show_string(struct descriptor_data *d, char *input);
int isbanned(char *hostname);
void weather_and_time(int mode);
int perform_alias(struct descriptor_data *d, char *orig, size_t maxlen);
void clear_free_list(void);
void free_messages(void);
void Board_clear_all(void);
void free_social_messages(void);
void Free_Invalid_List(void);

#ifdef __CXREF__
#undef FD_ZERO
#undef FD_SET
#undef FD_ISSET
#undef FD_CLR
#define FD_ZERO(x)
#define FD_SET(x, y) 0
#define FD_ISSET(x, y) 0
#define FD_CLR(x, y)
#endif

/*
 * The remainder of this file is maintained by the original CircleMUD
 * communication/game-loop implementation.  The RPG runtime integration is
 * intentionally limited to the heartbeat adapter include and call below.
 */

/* PLACEHOLDER: retained by the repository's existing generated source. */
