// -*- compile-command: "gmake thermo8"; compile-read-command: t ;  -*-
//  V6 is the split to having 2 servers running, liv and bed


// g++  -o thermo2 thermo2.c++   client_stub.c   -lpthread
// https://en.cppreference.com/w/cpp/thread/async
//  30 jul 14:38 was a real manual  turnoff needed....but ran too long

// 30 jul 19:50 real turnon
// 07/30/23 19:32:41  power on at 81.68   0.06
// Need manual turnon  0.03
// 31 jul 20:08   turnon is real
// 01 aug 10:55  turnon is real
// 04 aug 10:39 turnoff is real
//    and also 10:40
// 7 aug 1903 off is real

#include <arpa/inet.h>
#include <assert.h>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <execinfo.h>
#include <fcntl.h>
#include <iostream>
#include <math.h>
#include <mutex>
#include <netdb.h>
#include <netinet/in.h>
#include <numeric>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h> /* See NOTES */
#include <thread>
#include <time.h>
#include <unistd.h>
#include <vector>
#include <cstdarg>
#include <cstdio>
#include <csignal>
extern int getnetval(const char *, int, char *);
extern int putnetlog(const char *, int, const char *);
extern int putnetval(const char *, int, const char *);


#ifndef MACH
#define MACH "172.16.0.1"
#endif

int cheat=0;

/*! \var int backfd
 * \brief  backtrace(3) support
 */
int backfd = -1;

/*! \var int debug
 * \brief debug switch
 */
int debug = 0;

/*! \var int camera
 *  \brief is camera in use to verify a/c on/off
 */
int camera = 1;

/*! \var int ignoreslope
 *  \brief should slope be ignored in determing turning a/c off
 */
int ignoreslope = 1; // if 1 ignore slope calc

int logport = 5190; // logging port

/*   file FILE *logd
 *  \brief debugging log file handle
 */
FILE *logd; /*!< debug log file handle */

/*******************************************************/

/*******************************************************/
// moved from decider to global

/*******************************************************/

/*! \var float temp
 *  \brief last temperature taken file of readings
 */
float currenttemp; /* temperature */

/*! \var float templaston
 *  \brief temp when a/c last started, used in slope calc (slope)
 */
float templaston; // temp when last turned on

/*! \var float templastoff
 * \brief temp when when last stopped, used in slope calc (slope)
 */
float templastoff; // temp when last turned off

/*! \var time_t timelaston
 * \brief time when a/c last turned on  (slope)
 */
time_t timelaston; // time when last turned on

/*! \var time_t timelastoff
 * \brief time when a/c last turned off  (slope)
 */
time_t timelastoff; // time when last turned off

/*! \var float myslope
 * \brief  save result of slope calc  (slope)
 */
float myslope;

/*******************************************************/


/*!
 * \bug errmsg is a hack for printing error messages, needs to go away
 */
char errmsg[120]; // bad coding, fix some time

enum Canrun
{
  STOP,
  UNSTOP   // should also be OFF & RUN  (and lose UNSTOP)
  ,OFF
}; //!< permission to turn on, used in StatusClass.  Mirror of port 5009/5010

//enum ACrun {  ON,  OFF}; //!< is actual physical machine power on/off
   // this should replace the state variable below
#define ACON 1
#define ACOFF 0

enum Room
{
  BADNULL,
  BED,
  LIV
}; //!< what room is being operated, used in StatusClass
//#define BED 7
//#define LIV 13

#define BEDPORT 5050
#define LIVPORT 5020

/*! \var int state
 * \brief state of the a/c  ON or OFF
 */
int state = ACOFF;


/*! \brief prints string ON or OFF given value of global variable state
 *
 *
 */
const char *prnstate() { return (const char *)(state == ACON) ? "ON" : "OFF"; }

/*******************************************************/
// moved from decider to global

/*******************************************************/

void turnoff();
void turnon();
void decider(int);

/*********************************************/
/* definitions for decider()  */
/*********************************************/

/*! \def THRES
 * \brief threshold for how long to above MAX or below MIN before turn{on,off}
 * called
 */
#define THRES 3

/*! \def ARB_LARGE
 * \brief arbitrarily large amount of memory to malloc
 */
#define ARB_LARGE 9276

// column start 5 digits of temp in F
#if __SIZEOF_LONG__ == 8
#define TEMPCOL 19

/*! \def TEMPCOL 19
 * \brief column in logfile to find temperature(conditional)
 */
#else

#define TEMPCOL 21
/*! \def TEMPCOL
 * \brief column in logfile to find temperature
 */

#endif

#define TEMPCOL 21

/*! \def TEMPCOL
 * \brief column in logfile to find temperature
 */

/*********************************************/

/*! \var std::vector<float> avgtemp
 * \brief used to calc the average of last few readings
 */
std::vector<float> avgtemp;



/*! \var std::vector<float> avgtimet
 * \brief used to calc the average of last few readings
 */
std::vector<time_t> avgtimet;




/*********************************************/

/*! \def TIMEBUF
 * \brief  size of the timebuf array
 */
#define TIMEBUF 455


/*! \def sout(format, ...)
 * \brief   use this instead of printf to stdout
 */

#define sout(format, ...)                                                      \
  {                                                                            \
   char mtimebuf[125];							\
    mfillintime(mtimebuf);                                                    \
    fprintf(stdout, "%s ", mtimebuf);                                        \
    fprintf(stdout, format, ##__VA_ARGS__);                                    \
    fprintf(stdout, "\n");                                                     \
    fflush(stdout);                                                            \
  }

std::mutex timebuf_mut2; 

/*! \def mfillintime(mtimebuf)
  \brief  gets current time in printable form
*/

#define mfillintime(mtimebuf)       \
{       \
  time_t now;       \
  struct tm deal;       \
  std::lock_guard<std::mutex> lock(timebuf_mut2);       \
  mtimebuf[0] = '\0';		       \
  time(&now);        \
  memcpy(&deal, localtime(&now), sizeof(deal));        \
  strftime(mtimebuf, sizeof(mtimebuf), "'%D %H:%M:%S '", &deal);       \
}        



// no need to add time as the receiver adds this
/*! \def log172(format, ...)
  \brief  log messages to user
*/

#define log172(format, ...)                                                    \
  {                                                                            \
    char buffer[1024];                                                         \
     char mtimebuf[125];						\
     mfillintime(mtimebuf);					       	\
    sprintf(buffer, format, ##__VA_ARGS__);                                    \
    putnetlog("172.16.0.1", logport, buffer);                                  \
  }

// N B    the ## is GNU-only and will disapear in future compiler


std::mutex timebuf_mut4; 
//      std::lock_guard<std::mutex> lock(timebuf_mut4);			
//      if (cheat > 0) raise(SIGTRAP);					


/*! \def log172d(format, ...)
 * \brief  log debugging messages to file
 */

#define log172d(format, ...)                                                   \
  {                                                                            \
    if (debug)                                                                 \
    {                                                                          \
      printf("going for lock mutex\n"); \
      cheat++; \
      char mtimebuf[125];						\
      mfillintime(mtimebuf);					       	\
      fprintf(logd, "%s ", mtimebuf);                                          \
      fprintf(logd, format, ##__VA_ARGS__);                                    \
      fprintf(logd, " at line %d\n", __LINE__);                                \
      fflush(logd);                                                            \
      printf("leaving lock mutex\n"); \
      cheat--; \
    }                                                                          \
  }
// N B    the ## is GNU-only and will disapear in future compiler



void log172dd(const char * format, ...)                                                   \
  {                                                                            \
    if (debug)                                                                 \
    {                                                                          \
      std::lock_guard<std::mutex> lock(timebuf_mut4);			\
      char mtimebuf[125];						\
      mfillintime(mtimebuf);					       	\
      va_list args;
      va_start(args,format);
      fprintf(logd, "%s ", mtimebuf);                                          \
      vfprintf(logd, format, args);                                    \
      fprintf(logd, " at line %d\n", __LINE__);                                \
      fflush(logd);                                                            \
      va_end(args);
    }                                                                          \
  }

//      fprintf(logd, " len(%d) ", (int)strlen(mtimebuf));		




/*! \class MutexGuard
 * \brief   provide StatusClass a mutex for a/c's state
 */

class MutexGuard
{
private:
  std::mutex &m_Mutex;

public:
  inline MutexGuard(std::mutex &mutex) : m_Mutex(mutex) { m_Mutex.lock(); };

  inline ~MutexGuard() { m_Mutex.unlock(); };
};

// https://stackoverflow.com/questions/13300729/thread-safety-of-a-single-variable

/*! \class Range
 * \brief Store the min and max range of temperatures
 */

class Range
{
private:
  std::mutex m_Mutex;

  //public:
  float max;
  float min;
  float slopetarg;

public:
  Range() {
	  min=-2;
	  max=-2;
	  slopetarg=-2.0;
  }
  float mean()
  {
    MutexGuard scopedLock(m_Mutex); // lock the mutex
    return (max + min) / 2;
  }

  void setmin(float statearg)
  {
    MutexGuard scopedLock(m_Mutex); // lock the mutex
    min = statearg;
    // mutex automatically unlocked
  }

  void setmax(float statearg)
  {
    MutexGuard scopedLock(m_Mutex); // lock the mutex
    max = statearg;
    // mutex automatically unlocked
  }

  float readmin()
  {
    MutexGuard scopedLock(m_Mutex); // lock the mutex
    return min;
    // mutex automatically unlocked
  }

  float readmax()
  {
    MutexGuard scopedLock(m_Mutex); // lock the mutex
    return max;
    // mutex automatically unlocked
  }

///  @cond NO
#if 0
  void setslopetarg(float statearg)
  {
    MutexGuard scopedLock(m_Mutex); // lock the mutex
    slopetarg = statearg;
    // mutex automatically unlocked
  }

  float readslopetarg()
  {
    MutexGuard scopedLock(m_Mutex); // lock the mutex
    return slopetarg;
    // mutex automatically unlocked
  }
#endif
///  @endcond

}; // end Range

Range *range = new Range();

/*! \class StatusClass
 * \brief   indicates whether a/c is on or off using a mutex
 */

class StatusClass
{
private:
  std::mutex m_Mutex;

private:
  Canrun runstate; // off or on   STOP or UNSTOP

  Room runmode; // bedroom or living
  int port;     // tcpip port to receive cmds

public:
  StatusClass() //: runstate(STOP)
  {
    runmode = (Room)-2;
    runstate = STOP;
    port=-2;
  }


  /*! \brief  set state of STOP or UNSTOP
   *
   */
  void setstate(Canrun statearg)
  {
    MutexGuard scopedLock(m_Mutex); // lock the mutex
    runstate = statearg;
    // mutex automatically unlocked
  }

  /*! \brief read state of STOP or UNSTOP
   *
   */
  Canrun readstate()
  {
    MutexGuard scopedLock(m_Mutex); // lock the mutex
    return runstate;
    // mutex automatically unlocked
  }


  /*! \brief return string of state of STOP or UNSTOP
   *
   */
const char * readstatestr()
  {
    MutexGuard scopedLock(m_Mutex); // lock the mutex
    switch(runstate)
      {
	case STOP:
	  return "STOP";
	  break;

	  case UNSTOP:
	    return "UNSTOP";
	    break;

          case OFF:
	    return "OFF";
	    break;

	  default:
	    return "UNKN";
      }	// end switch

    return "UNKN";

    // mutex automatically unlocked
  }




  /*! \brief set room of LIV or BED
   *
   */
  void setmode(Room mode)
  {
    MutexGuard scopedLock(m_Mutex); // lock the mutex
    runmode = mode;
    if (mode == LIV)
      port = LIVPORT;
    if (mode == BED)
      port = BEDPORT;
    // mutex automatically unlocked
  }

  /*! \brief read room, either LIV or BED
   *
   */
  int readmode()
  {

    MutexGuard scopedLock(m_Mutex); // lock the mutex

    return runmode;
    // mutex automatically unlocked
  }

  void setport(int portarg)
  {
    MutexGuard scopedLock(m_Mutex); // lock the mutex
    port = portarg;
    // mutex automatically unlocked
  }

  int readport()
  {
    MutexGuard scopedLock(m_Mutex); // lock the mutex
    return port;
    // mutex automatically unlocked
  }

  // new thing to give string back of mode
  /*! \brief returns string, either LIV or BED
   *
   */
  char *printmode()
  {
    MutexGuard scopedLock(m_Mutex); // lock the mutex

    if (runmode == LIV)
    {
      strcpy(errmsg, "LIV");
      return errmsg;
    }
    if (runmode == BED)
    {
      strcpy(errmsg, "BED");
      return errmsg;
    }
    strcpy(errmsg, "ERR");
     return errmsg;
    // mutex automatically unlocked
  }

  /*! \brief returns string, either UNSTOP or STOP
   *
   */
  char *printstate()
  {
    MutexGuard scopedLock(m_Mutex); // lock the mutex

    switch (runstate)
    {
    case STOP:
    {
      strcpy(errmsg, "STOP");
      return errmsg;
    }

    break;

    case UNSTOP:
    {
      strcpy(errmsg, "UNSTOP");
      return errmsg;
    }

    break;

    default:
      sprintf(errmsg, "error %d", runstate);
      return errmsg;
      break;
    } // switch

    // mutex automatically unlocked
  }

}; // StatusClass

StatusClass *ThermoStatus = new StatusClass();

/*! \brief print the status in response to command over net
 *
 */

void print_status()
{

  sout("Camera %d  Debug %d  State %s  Ignoreslope %d Mode %s canrun %s",
       camera, debug, state ? "ON" : "OFF", ignoreslope,
       (LIV == ThermoStatus->readmode()) ? "LIV" : "BED",
       (UNSTOP == ThermoStatus->readstate()) ? "UNSTOP" : "STOP");

log172d("Camera %d  Debug %d  State %s  Ignoreslope %d Mode %s canrun %s",
       camera, debug, state ? "ON" : "OFF", ignoreslope,
       (LIV == ThermoStatus->readmode()) ? "LIV" : "BED",
       (UNSTOP == ThermoStatus->readstate()) ? "UNSTOP" : "STOP");
}

/*! \brief calc heatindex
 * \param atemp   current temperature
 * \param  humidity current hum
 * \return adjusted temperature for heat index
 */
double calcheatIndex(float atemp, float humidity)
{
  double c1 = -42.379;
  double c2 = 2.04901523;
  double c3 = 10.14333127;
  double c4 = -.22475541;
  double c5 = -0.00683783;
  double c6 = -0.05481717;
  double c7 = 0.00122874;
  double c8 = 0.00085282;
  double c9 = -0.00000199;

  double heatIndex =
      c1 + (c2 * atemp) + (c3 * humidity) + (c4 * atemp * humidity) +
      (c5 * (atemp * atemp)) + (c6 * (humidity * humidity)) +
      (c7 * (atemp * atemp) * humidity) + (c8 * atemp * (humidity * humidity)) +
      (c9 * (atemp * atemp) * (humidity * humidity));

  return heatIndex;
} /* calc. */

/*! \def QLEN
 * \brief length of network queue
 */

#define QLEN 16 /* size of request queue        */

bool isacon(); // function prototype

/*! \brief handles message from tcp socket  in thread
 * \param csock the socket to handle
 */
void SocketHandler(int csock)
{
  char buffer[1024];
  int buffer_len = 1024;
  int bytecount;

  memset(buffer, 0, buffer_len);
  if ((bytecount = recv(csock, buffer, buffer_len, 0)) == -1)
  {
    fprintf(stderr, "Error receiving data %d  line %d\n", errno, __LINE__);
    return;
  }

  /*****************AVG*****************************/
  if (0 == strncasecmp("avg", buffer, (buffer_len >= 3) ? 3 : buffer_len))
  {
    float sum_elems = std::accumulate(avgtemp.begin(), avgtemp.end(), 0.0);
    sum_elems = sum_elems / avgtemp.size();
    sout("Average temp is %2.2f", sum_elems);

  } // if avg

  /*****************IGNORESLOPE*****************************/
  if (0 ==
      strncasecmp("ignoreslope", buffer, (buffer_len >= 10) ? 10 : buffer_len))
  {
    if (ignoreslope)
      ignoreslope = 0;
    else
      ignoreslope = 1;

  } // if ignoreslope

  /*****************ISACON*****************************/
  if (0 == strncasecmp("isacon", buffer, (buffer_len >= 6) ? 6 : buffer_len))
  {
    sout("isacon is %d", isacon());

  } // if isacon

  /*****************EXIT*****************************/
  if (0 == strncasecmp("exit", buffer, (buffer_len >= 4) ? 4 : buffer_len))
  {
    exit(0);

  } // if exit

  /*****************STATUS**************************/
  if (0 == strncasecmp("status", buffer, (buffer_len >= 6) ? 6 : buffer_len))
  {
    print_status();
  } // end status

  /**************************DEBUG********************/
  if (0 == strncasecmp("debug", buffer, (buffer_len >= 5) ? 5 : buffer_len))
  {
    if (debug)
    {
      debug = 0;
    }
    else
    {
      debug = 1;
    }
  } //  DEBUG

  /********************CAMERA**************************/
  if (0 == strncasecmp("camera", buffer, (buffer_len >= 6) ? 6 : buffer_len))
  {
    if (camera)
    {
      camera = 0;
      sout("camera off");
      fflush(stdout);
    }
    else
    {
      camera = 1;
      sout("camera set");
      fflush(stdout);
    }
  } //  CAMERA

  /*************************START*********************/
  if (0 == strncasecmp("start", buffer, (buffer_len >= 5) ? 5 : buffer_len))
  {
    //      sleep(1);
    sout("start set");
    fflush(stdout);
    //      ThermoStatus->runstate = 1;
    ThermoStatus->setstate(UNSTOP);
    log172("START START");
    log172d("START START");
    fflush(stdout);

    // if the temp is higher than MIN than turn on the a/c

    if (currenttemp >= range->readmin() &&
        ACOFF == state) // and also make sure not running
    {
      turnon();
    }

  } // start

  /*********************QUIT*************************/
  if (0 == strncasecmp("quit", buffer, (buffer_len >= 4) ? 4 : buffer_len))
  {
    exit(0);
  } //  QUIT

  /*********************STOP*************************/
  if (0 == strncasecmp("stop", buffer, (buffer_len >= 4) ? 4 : buffer_len))
  {
    //      sleep(1);
    sout("stop set");
    fflush(stdout);

    ThermoStatus->setstate(STOP);
    //      ThermoStatus->runstate = 0;
    log172("STOP STOP state %s", prnstate());
    log172d("STOP STOP state %s", prnstate());
    fflush(stdout);
    if (LIV == ThermoStatus->readmode())
    {
      putnetval((char *)"192.168.1.237", 5010, "STOP");
    }
    else
    {
      putnetval((char *)"192.168.1.237", 5009, "STOP"); // bed
    }

    if (state == ACON) // if the a/c is running
    {
      turnoff();
      log172d("changing logger to STOP from state %s", prnstate());
      ThermoStatus->setstate(STOP); // to make sure the state doesn't become OFF
      log172d("a/c was running, changing to STOP");

      ThermoStatus->setstate(STOP);
    }
  } //  STOP

  /*********************UNSTOP*************************/
  if (0 == strncasecmp("unstop", buffer, (buffer_len >= 4) ? 4 : buffer_len))
  {
    //      sleep(1);
    sout("unstop set");
    fflush(stdout);

    ThermoStatus->setstate(UNSTOP);
    //      ThermoStatus->runstate = 0;
    log172("UNSTOP UNSTOP");
    log172d("UNSTOP UNSTOP");
    fflush(stdout);
    if (LIV == ThermoStatus->readmode())
    {
      log172d("changing logger to UNSTOP from state %s", prnstate());
      putnetval((char *)"192.168.1.237", 5010, "OFF");
      log172d("UNSTOP liv");
    }
    else
    {
      log172d("changing logger to OFF from state %s", prnstate());
      putnetval((char *)"192.168.1.237", 5009, "OFF"); // bed
      log172d("UNSTOP bed");
    }

///  @cond NO
#if 0
      if (state == ON )	// if the a/c is running
	{
	  turnoff();
	}
#endif
///  @endcond

  } //  UNSTOP

  /*********************LOG*************************/
  if (0 == strncasecmp("log", buffer, (buffer_len >= 3) ? 3 : buffer_len))
  {
    //      sleep(1);
    sout("log: %s", &buffer[4]);

    fflush(stdout);

    log172d("log: %s", &buffer[4]);

  } //  LOG

  log172d("Received bytes %d\nReceived string \"%s\"", bytecount, buffer);
  //       strcat(buffer, " SERVER ECHO");

  if ((bytecount = send(csock, buffer, strlen(buffer), 0)) == -1)
  {
    fprintf(stderr, "Error sending data %d\n", errno);
  }

  log172d("Sent bytes %d", bytecount);

  close(csock);

} // SocketHandler

/*! \brief  waits for message to come in over tcp socket in thread
 *
 * \param a   dummy
 */
void waitthread(int a)
{

  struct sockaddr_in sadr;
  socklen_t addr_size;
  int port = ThermoStatus->readport(); // was 5020;
  struct protoent *ptrp;               /* pointer to a protocol table entry */
  int sd;

  memset((char *)&sadr, 0, sizeof(sadr)); /* clear sockaddr structure   */
  sadr.sin_family = AF_INET;              /* set family to Internet     */
  //   sadr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);       /* set the local IP
  //   address */
  sadr.sin_addr.s_addr = inet_addr(MACH); /* set the local IP address */
  sadr.sin_port = htons((u_short)port);
  /*
     Map TCP transport protocol name to protocol number
   */

  if (((ptrp = getprotobyname("tcp"))) == 0)
  {
    fprintf(stderr, "cannot map \"tcp\" to protocol number  line %d", __LINE__);
    _exit(1);
  }

  /*
     Create a socket
   */
  sd = socket(PF_INET, SOCK_STREAM, ptrp->p_proto);
  if (sd < 0)
  {
    fprintf(stderr, "socket creation failed line %d\n", __LINE__);
    _exit(1);
  }

  /*
     Bind a local address to the socket
   */
  if (bind(sd, (struct sockaddr *)&sadr, sizeof(sadr)) < 0)
  {
    perror("bind failed");
    fprintf(stderr, "bind failed line %d\n", __LINE__);
    _exit(1);
  }

  /*
     Specify a size of request queue
   */
  if (listen(sd, QLEN) < 0)
  {
    fprintf(stderr, "listen failed line %d\n", __LINE__);
    _exit(1);
  }

  while (true)
  {

    log172d("waiting for a connection");
    int csock = -1;

    if ((csock = accept(sd, (sockaddr *)&sadr, &addr_size)) != -1)
    {

      log172d("---------------------\nReceived connection from   %s",
              inet_ntoa(sadr.sin_addr));
      // std::thread th(&SocketHandler, NULL);

      std::thread th(SocketHandler, csock);
      th.detach();
    }
    else
    {
      fprintf(stderr, "Error accepting %d %s\n", errno, strerror(errno));
    }
  }
}



/*! \fn now
   \brief return the epoch time
 *
 *
 */

time_t now()
{
  time_t mynow;
  time(&mynow);
  return mynow;
}


/*! \fn bedtime
   \brief tell decider when it is bedtime, stop loop if it is time for people to
 be asleep \return true if it is bedtime
 *
 *
 */

bool bedtime()
{             //
  time_t now; /* tmp copy */
  struct tm tmm;

  time(&now);
  memcpy(&tmm, localtime(&now), sizeof(struct tm)); /* current time */

  if (23 == tmm.tm_hour && tmm.tm_min > 30)
    return true;

  return false;
}

/*! \brief execute command to turn off/on the a/c
 *
 */

void cmd()
{
  assert((LIV == ThermoStatus->readmode()) | (BED == ThermoStatus->readmode()));

  if (LIV == ThermoStatus->readmode())
  {
    log172d("power irsend to liv");
    system(
        "cat /dev/null | irsend -a 192.168.1.8:8700  SEND_ONCE frigid power");
  }

  if (BED == ThermoStatus->readmode())
  {
    log172d("power irsend to bed");
    system(
        "cat /dev/null | irsend -a 192.168.1.194:8700  SEND_ONCE frigid power");
  }
}

/*! \var int instate
 * \brief monotonic counter for number of times in currentstate
 */
int instate = 0; /* incr each time */
                 /*
                    current state out of range
                  */





/*! \brief figure the slope between temperatures
 *
 * \param oldtime denominator for slope
 * \param tempdiff of last two temperatures
 * \return the slope calculated
 *
 */

//float slope(time_t oldtime, float tempdiff)
float slope()
{
  time_t now;
  float the_answer;
  time(&now);

  time_t oldtime = now-avgtimet.front();
  float tempdiff = avgtemp.back()-avgtemp.front();


  if ( oldtime > 0 )
    {
the_answer = (10 * tempdiff) / ( oldtime);
    }
  else
    {
the_answer = 0;
    }

  

  log172d("slope nnow=%ld front=%2.2f old=%ld tempdiff=%2.2f ans=%2.2f",/* now*/ avgtimet.front(), avgtemp.front(), oldtime, 10*tempdiff, the_answer);

  return the_answer;


} // slope

/*!\fn isacoff
   \brief true if camera says ac is off
 * \return true if camera says ac is off
 */
bool isacoff() // true if camera says ac is off
{
  /* file
   * \brief /tmp/acstate created on server giving a/c status from ac.sec.com
   */
  FILE *f;
  char statefile[200];
  //  char statecmd[200];
  statefile[0] = '\0';
  //  statecmd[0]='\0';

  if (LIV == ThermoStatus->readmode())
  {
    strcpy(statefile, "/tmp/acstate");
  }
  else
  {
    strcpy(statefile, "/tmp/acstate2");
  } // bed

///  @cond NO
#if 0
  strcat(statecmd, "cat /dev/null | rm -f ");
  strcat(statecmd, statefile);
  system( statecmd);
#endif
///  @endcond

  // get rid of file

  log172d("isacoff del file %s  ", statefile);
  if (0 != unlink(statefile))
  { // deal with problem
    char errmsg[400];
    strcpy(errmsg, "Deleting ");
    strcat(errmsg, statefile);
    strcat(errmsg, " error: ");
    strcat(errmsg, strerror(errno));
    log172d("isacoff del file %s failed %s  ", statefile, errmsg);
            
    char errmsg2[600];
    sprintf(errmsg2, "cat /dev/null | echo '%s' | sudo wall", errmsg);
    system(errmsg2);
  } // end unlink deals with error

  if (LIV == ThermoStatus->readmode())
  {
    system("ssh -n pi@ac.sec.com bin_pi/testonoff");
  }
  else
  {
    system("ssh -n pi@bedac.sec.com bin_pi/testonoff");
  }

  sleep(1); // this is tricky, as it could return
            // before the file is copied over

  if (NULL == (f = fopen(statefile, "r")))
  {
    std::string tmp = "ssh -n reilly@gbrmail.com echo \"\007\007AC STATE \"    "
                      "+ statefile +   \" isacoff \" | wall";
    system(tmp.c_str());
    log172d("stuck ac in isacoff state %s ", statefile);
    system("cat /dev/null | echo 'acstate empty' | wall");

    return 1;
  }

  char buff[200];
  buff[0] = '\0'; // zero out first character

  if (NULL == fgets(buff, 200, f))
  {
    log172d("NULL FROM READING acstate  ");
    system("cat /dev/null | echo 'empty file acstate?' | sudo wall");
    return 1;
  }
  if (0 == strncasecmp(buff, "OF", 2))
  {
    log172d("camera says ac is off '%s'  ", buff);
    return 1;
  }
  else
  {
    log172d("camera says ac is on '%s'  ", buff);
    return 0;
  }

} // isacoff

/*!\fn isacon
   \brief true if camera says ac is on by returning !isacoff
 * \return  true if camera says ac is on by returning !isacoff
 */
bool isacon() // true if camera says ac is on
{
  return !isacoff();
} // isacon

/*!   \brief check using camera if a/c is on or off
 *
 */

void checkitworked(int desire  /**< [in] 1 if you want camera on and 0 if you want camera off */)
{
  if (0 == camera)
    return;

  if (0 == desire && isacoff())
    return;

  if (1 == desire && isacon())
    return;

  // fall thru to here if ac is not in desired state

  do
  {
    // emergency exit to stop the loop if STOP is set by runac

    if (ThermoStatus->readstate() == STOP)
      return;

    sleep(5);

    cmd();
    log172d("cmd called in checkitworked %d ", desire );

    if (0 == desire && isacoff())
      return;

    if (1 == desire && isacon())
      return;

    log172d("stuck ac in checkitworked desire %d  ", desire);

  } while (1);

} // check..

/*! \fn turnoff
   \brief execute system commands to turn off ac and verify it
 *
 */

void turnoff()
{
  if (debug)
  {
    sout("OFF");
  }

  system("cat /dev/null | event 'ac off'");


  float sum_elems = std::accumulate(avgtemp.begin(), avgtemp.end(), 0.0);
  sum_elems = sum_elems / avgtemp.size();




  instate++;

  log172d("turnoff()   %.2f<min instate %d", currenttemp, instate);

  /*********************************************/
  // turn off when it hits the min temp
  // slope calc
  myslope = slope(/*timelaston, temp - templaston*/);
  time(&timelastoff);
  templastoff = currenttemp;
  // end slope calc

  avgtemp.clear(); // no longer need to collect times, clear after calling slope
  avgtimet.clear(); // no longer need to collect times, clear after calling slope

  log172d("turnoff() calling cmd()");
  cmd();
  //  fillintime();
  log172("power off at %2.2f slope %2.2f turnoff line %d", currenttemp, myslope,
         __LINE__);
  log172d("power off at %2.2f slope %2.2f turnoff", currenttemp, myslope);
  // see, debugging called as well

  //         fprintf(log, "%s power off at %2.2f\t%2.2f\n", timebuf, temp,
  //         myslope); fflush(log);

  /*********************************************/
  /*********************************************/
  /******run python gnuplot data****************/
  /*********************************************/
  /*********************************************/

  if (LIV == ThermoStatus->readmode())
  {
    system("/u/reilly/bin_sec/runplotac liv");
  }

  if (BED == ThermoStatus->readmode())
  {
    system("/u/reilly/bin_sec/runplotac bed");
  }

  if (LIV == ThermoStatus->readmode())
  {
    log172d("turnoff() changing liv logger to OFF from state %s", prnstate());
    putnetval((char *)"192.168.1.237", 5010, "OFF");
  }
  else
  {
    log172d("turnoff() changing bed logger to OFF from state %s", prnstate());
    putnetval((char *)"192.168.1.237", 5009, "OFF"); // bed
  }

  state = ACOFF;
  instate = 0;

  // DID IT WORK?
  checkitworked(0);

  /*********************************************/
}

/*! \fn turnon
  \brief execute system commands to turn on ac and verify it
 *
 */

void turnon()
{
  if (debug)
  {
    sout("ON");
  }

  system("cat /dev/null | event 'ac on'");


  float sum_elems = std::accumulate(avgtemp.begin(), avgtemp.end(), 0.0);
  sum_elems = sum_elems / avgtemp.size();


  // slope calc
  {
    myslope = slope(/*timelastoff, temp - templastoff*/);
    time(&timelaston);
    templaston = currenttemp;
  }
  // end slope calc

  avgtemp.clear(); // restart collecting temps
  avgtimet.clear(); // restart collecting temps


  log172d("turnon() calling cmd()");
  cmd();
  state = ACON;
  log172d("cmd used for power on at %2.2f %2.2f %2.2f ", currenttemp, myslope,
          sum_elems);

  //  fillintime();


  log172("power on at %2.2f %2.2f %2.2f Line %d", currenttemp, myslope, sum_elems,
         __LINE__);

  //            fprintf(log, "%s power on at %2.2f\t%2.2f\n", timebuf, temp,
  //            myslope); fflush(log);

  if (LIV == ThermoStatus->readmode())
  {
    putnetval((char *)"192.168.1.237", 5010, "ON");
  }
  else
  {
    putnetval((char *)"192.168.1.237", 5009, "ON"); // bed
  }

  // DID IT WORK?
  checkitworked(1);

  // this code should be run once when the program is first started

///  @cond NO
#ifdef NEVER

  if (LIV == ThermoStatus->readmode())
  {

    log172d("power double tap irsend to liv");
    system("cat /dev/null | irsend -a ac.sec.com:8700  SEND_ONCE frigid right");
    system("cat /dev/null | irsend -a ac.sec.com:8700  SEND_ONCE frigid right");
  }

  if (BED == ThermoStatus->readmode())
  {
    log172d("power double tap irsend to bed");
    system(
        "cat /dev/null | irsend -a 192.168.1.194:8700  SEND_ONCE frigid right");
    system(
        "cat /dev/null | irsend -a 192.168.1.194:8700  SEND_ONCE frigid right");
  }
#endif
///  @endcond


} // turnon

void decider(int a /**< [in] docs for input parameter v. */)
{

  /*!
     void decider(int a)  loop to control the a/c
   *
   *
   * \showrefs
   */

  char *testret; /* test return */
  FILE *tempfile;

  if (LIV == ThermoStatus->readmode())
  {
    tempfile = fopen("/u/reilly/templog5", "r"); // input
  }

  if (BED == ThermoStatus->readmode())
  {
    tempfile = fopen("/u/reilly/templog4", "r"); // input
  }

  float min = 78.0, max = 81.2;

  // UNUSED #define QUIETTIME 60
  //    wait this long after sending cmd before the wall

  templaston = min;  // temp when last turned on
  templastoff = max; // temp when last turned off

  // debugging stuff
  char *thedeb = getenv("DEBUGTH");
  if (NULL != thedeb)
  {
    debug = 1;
    sout("DEBUG MODE");
  } /* if null... */

  /*
     Get large buffer
   */

  char *buf;

  buf = (char *)malloc(ARB_LARGE);
  assert(buf); /* did it work */

  // initialize with target temps
  state = ACOFF;
  // assume power = OFF;

  time(&timelastoff);
  assert(0 == fseek(tempfile, 0, SEEK_END));

  /*
     Loop to read ~/templog3 or 5
   */

  while (1)
  {
    char tempbuf[10]; /* tempur copied here */
    char valbuf[100]; /* getnetval result */
    float tempmin = 0.0;
    float tempmax = 0.0;

    /**********************************************/
    /************GET MIN AND MAX*******************/
    /**********************************************/

    if (LIV == ThermoStatus->readmode())
    {
      getnetval((char *)"192.168.1.237", 5201, valbuf);
      tempmin = atof(valbuf);
      if (tempmin > 20 && tempmin != min)
      {
        instate = 0; // reset the 3-in-a-row
        min = tempmin;
        range->setmin(min);
	//        fillintime();
        log172("Min now %.2f", min);
      }	// set the min
      getnetval((char *)"192.168.1.237", 5202, valbuf);
      tempmax = atof(valbuf);
      if (tempmax > 20 && tempmax != max)
      {
        instate = 0; // reset the 3-in-a-row
        max = tempmax;
        range->setmax(max);
	//        fillintime();

        log172("Max now %.2f", max);
      }	// set the max
    } // if LIV
    else
    {				// fall here for BED
      getnetval((char *)"192.168.1.237", 5204, valbuf);
      tempmin = atof(valbuf);
      if (tempmin > 20 && tempmin != min)
      {
        instate = 0; // reset the 3-in-a-row
        min = tempmin;
        range->setmin(min);
	//        fillintime();
        log172("Min now %.2f", min);
      }	// set the min
      getnetval((char *)"192.168.1.237", 5205, valbuf);
      tempmax = atof(valbuf);
      if (tempmax > 20 && tempmax != max)
      {
        instate = 0; // reset the 3-in-a-row
        max = tempmax;
        range->setmax(max);
	//        fillintime();

        log172("Max now %.2f", max);
      }	// set the max
    } // else BED

///  @cond NO
#if 0
    getnetval((char *)"192.168.1.237", 5203, valbuf);
    tempmax = atof(valbuf);
    if (tempmax != range->readslopetarg())

      //  removed b/c no one calls read/set slopetarg other than here
    {
      //      fillintime();
      log172("Slope now %.2f", tempmax);
    }
    range->setslopetarg(tempmax);

#endif
///  @endcond


    /**********************************************/
    /************END MIN AND MAX*******************/
    /**********************************************/

    /**********************************************/
    // This is a tail of templog*
    /**********************************************/

    testret = NULL;

    do
    {
      sleep(1);
      assert(0 == fseek(tempfile, 0, SEEK_CUR));
      testret = fgets(buf, ARB_LARGE,
                      tempfile); /* does this fail on a 1 line file? */

    } while (NULL == testret); // null testret

    strncpy(tempbuf, &buf[TEMPCOL], 5);

    /**********************************************/
    //  tempbuf now has the latest temperature
    /**********************************************/

    // ugly fix for atof rounding up/down on rpi
    // force a zero to replace blanks at [4] and [5]
    if (' ' == tempbuf[4] || '\t' == tempbuf[4])
      tempbuf[4] = '0';
    if (' ' == tempbuf[5] || '\t' == tempbuf[5])
      tempbuf[5] = '0';

    currenttemp = atof(tempbuf);

    avgtemp.push_back(currenttemp);
    avgtimet.push_back(now());


    // conditional found at
    // https://gcc.gnu.org/onlinedocs/cpp/System-specific-Predefined-Macros.html

#if __arm__
    log172d("t %2.2f '%s'  %u %d ", currenttemp, tempbuf, avgtemp.size(),
            ThermoStatus->readstate()

    );

#else
    log172d("t %2.2f '%s'  #%lu %2.2f %d "
	    , currenttemp
	    , tempbuf
	    , avgtemp.size()
	    , slope()
            ,ThermoStatus->readstate()

    );
#endif
    /**********************************************/
    //       end get stuff from templog
    /**********************************************/

    // instate counts how many temps are matches(would cause action) in a row

    if (currenttemp > min && currenttemp < max)
    {
      instate = 0; /* not THRES in a row, reset counter */
      log172d("not THRES in a row, reset counter   min %.2f  max %.2f temp %.2f", min, max, currenttemp);
      continue;
    }

    /***********************************/
    /***********************************/
    /*
       this will cut short the loop if ThermoStatus->runstate==0
     */
    /***********************************/

    if (state == ACOFF && ThermoStatus->readstate() == STOP)
    {
      log172d("continue b/c STOP");
      continue;
    }

    /***********************************/
    /***********************************/
    // stop loop if it is time for people to be asleep
    /***********************************/

    //      if (bedtime())
    //      {
    //	ThermoStatus->setstate(STOP);

    //         ThermoStatus->runstate = 0;
    //      }

    log172d("decid state==%s  temp<=min %s  slope %2.2f   ThermoStatus %d  instate %d  "
            ,(state == ACON) ? "ON" : "OFF"
	    , (currenttemp <= min) ? "yes" : "no"
	    , (slope(/*timelaston, temp - templaston*/))
            ,ThermoStatus->readstate(), instate);

    // start should I stop it?
    //*******************************************************

    bool stoprun = (state == ACON);

    // ???WHY DOES stop4slope get set twice?

///  @cond NO
#if 0
    bool stop4slope =
        slope(timelaston, currenttemp - templaston) >= range->readslopetarg();


    // if stop4slope and avgtemp<temp then
    stop4slope =
        stop4slope || ((std::accumulate(avgtemp.begin(), avgtemp.end(), 0.0) /
                        avgtemp.size()) < currenttemp);

    // NEW....if a stop is sent to tcp port then
    //       stop4slope = stop4slope ||

    //  FORCE SLOPE TO BE IGNORED WHEN IT IS IN 50's outside

    //this logic seems WRONG
    stop4slope = ignoreslope ? 1 : stop4slope;



    // WHY DOES stoprun get set twice?
    stoprun = stoprun && ((STOP == ThermoStatus->readstate()) ||
                          (currenttemp <= min && stop4slope));

    // end should I stop it?
    //*******************************************************
#endif
///  @endcond

    stoprun = (currenttemp <= min);
    if (stoprun)
    {
      log172d("decid stoprun=1 calling turnoff() ");
      turnoff();
    }

    if (state == ACOFF && currenttemp >= max)
    {
      instate++;

      log172d("state off %.2f>max %d ", currenttemp, instate);
    }

    if (state == ACOFF && currenttemp < max)
    {
      instate = 0;
      log172d("state still off %.2f<max %d ", currenttemp, instate);
    }

    if (instate >= THRES)
    {
      instate = 0;

      switch (state)
      {
      case ACON:
        log172d("decid stateON >=THRES calling turnoff() ");
        turnoff();

        break;

      case ACOFF:
        log172d("decid stateOFF >=THRES calling turnon() ");
        turnon();

        break;

      default:
        log172("I suck\n\n");
        exit(1);

      } /* state */

    } /* instate>THRES */

  } /* while 1 */
} /* decider */

/*! \mainpage Thermostat

\section  More to come

 */

int main(int argc, char *argv[])
{
  int a = 1;

  fclose(stdin); // no one should read

  backfd = creat("/tmp/back.tr", 0666);
  //   assert(backfd > 0);

  //  add an event when starting the program
  //   system("cat /dev/null | /u/reilly/bin_sec/event acon");

  if (argc < 2) // need two args
  {
    fprintf(stderr, "Usage:  thermostat [bed|liv]\n");
    exit(2);
  }

  if (strlen(argv[1]) < 3)
  {
    fprintf(stderr, "Usage:  thermostat [bed|liv]\n");
    exit(2);
  }

  if (0 == strncmp("bed", argv[1], 3))
  {
    ThermoStatus->setmode(BED);
    camera = 1;
    debug = 1;
    logport = 5191;
  }

  if (0 == strncmp("liv", argv[1], 3))
  {
    ThermoStatus->setmode(LIV);
    debug = 1;
  }

  if (LIV == ThermoStatus->readmode())
  {
    logd = fopen("/tmp/debug.log", "a+");
  }
  if (BED == ThermoStatus->readmode())
  {
    logd = fopen("/tmp/debug.log2", "a+");
  }

  //  STARTUP IN STOP AND MAKE SURE VISIBLE ON .237 TCL BOX

  if (LIV == ThermoStatus->readmode())
  {
    putnetval((char *)"192.168.1.237", 5010, "STOP");
    log172d("initial status set to STOP liv");
  }
  else
  {
    putnetval((char *)"192.168.1.237", 5009, "STOP"); // bed
    log172d("initial status set to STOP bed");
  }

  ThermoStatus->setstate(STOP);
  log172d("initial status set to STOP");

  //  what is this?

  //   assert(ThermoStatus->readmode());

  print_status();

  //  std::thread th(decider,a);
  //  th.detach();
  //  th.join();
  std::thread th(waitthread, a);
  th.detach();

  decider(1);

  int filler = 34;
  filler = filler - 1;
} // main
