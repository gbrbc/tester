// -*- compile-command: "gmake thermo6"; compile-read-command: t ;  -*-
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
#include <condition_variable>
#include <csignal>
#include <cstdarg>
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

extern int getnetval(const char *, int, char *);
extern int putnetlog(const char *, int, const char *);
extern int putnetval(const char *, int, const char *);

#ifndef MACH
#define MACH "172.16.0.1"
#endif

//page 44 of manual
/**
 * \defgroup Variables Global variables
*/
/**@{*/

int slopeport=0;
#define SLOPELIV 5199
#define SLOPEBED 5198

int depth = 0;

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


/*   file FILE *logd
 *  \brief debugging log file handle
 */
FILE *logd; /*!< debug log file handle */

/*******************************************************/

/*******************************************************/
// moved from decider to global

/*******************************************************/

/*! \var float currenttemp
 *  \brief last temperature taken file of readings
 */
float currenttemp; /* temperature */

/* \var float templaston
 *  \brief temp when a/c last started, used in slope calc (slope)
 */
// float templaston; // temp when last turned on

/* var float templastoff
 * brief temp when when last stopped, used in slope calc (slope)
 */
// float templastoff; // temp when last turned off

/* var time_t timelaston
 * brief time when a/c last turned on  (slope)
 */
// time_t timelaston; // time when last turned on

/* var time_t timelastoff
 * brief time when a/c last turned off  (slope)
 */
// time_t timelastoff; // time when last turned off

/*! \var float myslope
 * \brief  save result of slope calc  (slope)
 */
float myslope;


/**@}*/


/*******************************************************/

enum Canrun
{
  STOP,
  OFF
}; //!< permission to turn on, used in StatusClass.  Mirror of port 5009/5010
//  NEINGO   // should also be OFF & RUN  (and lose NEINGO)

enum ACrun
{
  ACON,
  ACOFF
}; //!< is actual physical machine power on/off
   // this should replace the physicalstate variable below
//#define ACON 1
//#define ACOFF 0

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

// ACrun physicalstate = ACOFF;
// int physicalstate = ACOFF;

/*******************************************************/
// moved from decider to global

/*******************************************************/

void turnoff();
void turnon();
//void decider();

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
//#if __SIZEOF_LONG__ == 8
//#define TEMPCOL 19

/* \def TEMPCOL 19
 * \brief column in logfile to find temperature(conditional)
 */
//#else

//#define TEMPCOL 21
/* \def TEMPCOL
 * \brief column in logfile to find temperature
 */

//#endif

#define TEMPCOL 21

/*! \def TEMPCOL
 * \brief column in logfile to find temperature
 */

/*********************************************/
/*!  \see log_mut1_unlocked
 *
 */


std::mutex log_mut1; // for logthread

//std::recursive_mutex avgt_mut;

/*! \var std::condition_variable g_cv;
 * \brief part of the signal to logging thread
 */
std::condition_variable g_cv;

/*! \var bool log_mut1_unlocked
 * \brief flag indicating slope() value can be sent to logger
 */
bool log_mut1_unlocked =    false; // Flag to signal when the mutex is "unlocked" conceptually

/*! \var float number_to_send;
 * \brief pass the value of slope() to logging thread
 */
float number_to_send;


/*! \var std::mutex timebuf_mut4;
  \brief  mutex used by mfillintime
*/
std::mutex timebuf_mut4;


/*! \def mfillintime(mtimebuf)
  \brief  gets current time in printable form
*/


#define mfillintime(mtimebufarg)                                               \
  {                                                                            \
    std::lock_guard<std::mutex> lock(timebuf_mut4);                            \
    time_t now;                                                                \
    struct tm deal;                                                            \
    mtimebufarg[0] = '\0';                                                     \
    time(&now);                                                                \
    memcpy(&deal, localtime(&now), sizeof(deal));                              \
    strftime(mtimebufarg, sizeof(mtimebufarg), "%D %H:%M:%S ", &deal);         \
  }




/*!   \brief needs to go away as std::time() being used
 *
 */
std::unique_ptr<std::string> mfillintime2()
{
  time_t now;
  struct tm deal;
  char mtimebufarg[124];
  mtimebufarg[0] = '\0';
  time(&now);
  memcpy(&deal, localtime(&now), sizeof(deal));
  strftime(mtimebufarg, sizeof(mtimebufarg), "%D %H:%M:%S ", &deal);
  return std::make_unique<std::string>(mtimebufarg);
}




/*! \def log172d(format, ...)
 * \brief  log debugging messages to file
 */
std::recursive_mutex timebuf_mut3;
#define log172d(format, ...)                                                   \
  {                                                                            \
    if (debug)                                                                 \
    {                                                                          \
      std::lock_guard<std::recursive_mutex> lock(timebuf_mut3);                \
      depth++;                                                                 \
      std::unique_ptr<std::string> mtimebuf = mfillintime2();                  \
      if (depth < 2)                                                           \
      {                                                                        \
        fprintf(logd, "%s ", (*mtimebuf).c_str());                             \
      }                                                                        \
      fprintf(logd, format, ##__VA_ARGS__);                                    \
      fprintf(logd, " at line %d\n", __LINE__);                                \
      fflush(logd);                                                            \
      depth--;                                                                 \
    }                                                                          \
  }



/**
 * @brief Writes a given integer number to a specified IP address and port
 * after a conceptual mutex (controlled by a condition variable) is "unlocked".
 *
 * @return true if the number was sent successfully, false otherwise.
   *
   * \showrefs
   *
 */
bool logthread()
{
  std::string ip_address = "192.168.1.237"; // Replace with your target IP
  //  int port = 5199;

//  time_t now;

  while (1)
  {
    // Wait until the mutex is conceptually "unlocked" (signaled via g_cv)
    std::unique_lock<std::mutex> lock(log_mut1);
    //    std::cout << "Attempting to acquire lock and wait for mutex unlock
    //    signal..." << std::endl;
    g_cv.wait(lock,
              [] {
                return log_mut1_unlocked;
              }); // Wait until log_mut1_unlocked is true
    //    std::cout << "Mutex unlock signal received. Proceeding with socket
    //    operation." << std::endl;

    int sock = 0;
    struct sockaddr_in serv_addr;
    bool success   __attribute__ ((unused));
    success = false;

    // 1. Create socket file descriptor
    // AF_INET: IPv4 Internet protocols
    // SOCK_STREAM: Provides sequenced, reliable, two-way, connection-based byte
    // streams (TCP) 0: Protocol (IP)
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
      std::cerr << "Error: Socket creation failed." << std::endl;
      log172d("logthread conn fail %s", strerror(errno));
      continue;
//      return false;
    }

    // Clear structure and set family, IP address, and port
    memset(&serv_addr, '0', sizeof(serv_addr)); // Zero out the structure
    serv_addr.sin_family = AF_INET;             // IPv4
    serv_addr.sin_port = htons(slopeport); // Convert port to network byte order

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, ip_address.c_str(), &serv_addr.sin_addr) <= 0)
    {
      std::cerr << "Error: Invalid IP address or address not supported."
                << std::endl;
      close(sock);
      log172d("logthread ip addr fail %s", strerror(errno));
      continue;
//      return false;
    }

    // 2. Connect to the server
    // sock: Socket file descriptor
    // (struct sockaddr*)&serv_addr: Pointer to the address structure
    // sizeof(serv_addr): Size of the address structure
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
      std::cerr << "Error: Connection failed to " << ip_address << ":" << slopeport
                << "   " << strerror(errno) << "   " << errno << std::endl;

      if (111 == errno)
      {
        sleep(6);
        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) >=
            0)
        {
          goto fred;
        }
      }

      close(sock);
      log172d("logthread ip addr fail %s", strerror(errno));
      continue;
//      return false;
    }
  fred:
    //    std::cout << "Successfully connected to " << ip_address << ":" << port
    //    << std::endl;

    // 3. Send the number
    // For simplicity, we'll convert the integer to a string and send it.
    // In a real application, you might send it as raw bytes (e.g., using
    // memcpy) or a more structured binary format.

    //    number_to_send = number_to_send - int(2*rand());
    char nts[100]; /* must trim size of slope */
    sprintf(nts, "%.2f", number_to_send * 100);
    //    std::string message = std::to_string(nts /*number_to_send*/);
    size_t bytes_sent = send(sock, nts, strlen(nts), 0);
    //    ssize_t bytes_sent = send(sock, message.c_str(), message.length(), 0);

    if (bytes_sent < 0)
    {
      std::cerr << "Error: Failed to send data." << std::endl;
    }
    else if (bytes_sent != strlen(nts) /*message.length()*/)
    {
      std::cerr << "Warning: Only " << bytes_sent << " of "
                << strlen(nts) /*message.length()*/
                << " bytes sent." << std::endl;
      success = true; // Partial send might still be considered a success
                      // depending on requirements
    }
    else
    {
      //        std::cout << "Successfully sent number: " << number_to_send << "
      //        (" << bytes_sent << " bytes)" << std::endl;
      success = true;
    }

    // 4. Close the socket
    close(sock);

    log_mut1_unlocked = false;

    //    return success;
  }
}

/*! \var std::vector<float> avgtemp
 * \brief used to calc the average of last few readings, lock with avgt_mut
 */
std::vector<float> avgtemp;

// avgtemp.resize(5);
// avgtemp.clear();

/*! \var std::vector<float> avgtimet
 * \brief used to calc the average of last few readings, lock with avgt_mut
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
    char mtimebuf[125];                                                        \
    mfillintime(mtimebuf);                                                     \
    fprintf(stdout, "%s ", mtimebuf);                                          \
    fprintf(stdout, format, ##__VA_ARGS__);                                    \
    fprintf(stdout, "\n");                                                     \
    fflush(stdout);                                                            \
  }

//std::recursive_mutex timebuf_mut2;


// no need to add time as the receiver adds this
/*! \def log172(format, ...)
  \brief  log messages to user
*/

#define log172(format, ...)                                                    \
  {                                                                            \
      mfillintime(mtimebuf);                                                   \
      fprintf(logd, "%s F ", mtimebuf);                                        \
      mtimebuf[0] = '\0';                                                      \
      fprintf(logd, format, ##__VA_ARGS__);                                    \
      fprintf(logd, " at line %d\n", __LINE__);                                \
    sprintf(buffer, format, ##__VA_ARGS__);                                    \
    putnetlog("172.16.0.1", ThermoStatus->readlogport(), buffer);	\
  }

// N B    the ## is GNU-only and will disapear in future compiler

//      std::lock_guard<std::recursive_mutex> lock(timebuf_mut4);
//      if (cheat > 0) raise(SIGTRAP);


/*! \var std::mutex timebuf_mut3;
  \brief  mutex used by log172d macro
*/
//here

#define GROTlog172d000(format, ...)                                            \
  {                                                                            \
    if (debug)                                                                 \
    {                                                                          \
      std::lock_guard<std::mutex> lock(timebuf_mut3);                          \
      char mtimebuf[125];                                                      \
      mfillintime(mtimebuf);                                                   \
      fprintf(logd, "%s F ", mtimebuf);                                        \
      mtimebuf[0] = '\0';                                                      \
      fprintf(logd, format, ##__VA_ARGS__);                                    \
      fprintf(logd, " at line %d\n", __LINE__);                                \
      fflush(logd);                                                            \
    }                                                                          \
  }

///  @cond NO
// N B    the ## is GNU-only and will disapear in future compiler

//      fprintf(logd, format, ##__VA_ARGS__);
#if 0
void GROTlog172dd(const char *format, ...)
{
  if (debug)
  {
    std::lock_guard<std::recursive_mutex> lock(timebuf_mut2);
    char mtimebuf[125];
    mtimebuf[0] = '\0';
    fflush(logd);
    mfillintime(mtimebuf);
    fflush(logd);
    va_list args;
    fflush(logd);
    fprintf(logd, "%s B ", mtimebuf);
    fflush(logd);
    va_start(args, format);
    fflush(logd);
    vfprintf(logd, format, args);
    fprintf(logd, " at line %d\n", __LINE__);
    fflush(logd);
    va_end(args);
  }
}

//      fprintf(logd, " len(%d) ", (int)strlen(mtimebuf));
#endif
///  @endcond

/*! \class MutexGuard
 * \brief   provide StatusClass a mutex for a/c's state
 */

class MutexGuard
{
private:
  std::recursive_mutex &m_Mutex;

public:
  inline MutexGuard(std::recursive_mutex &mutex) : m_Mutex(mutex)
  {
    m_Mutex.lock();
  };

  inline ~MutexGuard() { m_Mutex.unlock(); };
};

// https://stackoverflow.com/questions/13300729/thread-safety-of-a-single-variable

/*! \class Range
 *
 *
 * \brief Store the min and max range of temperatures
 */

class Range
{
private:
  std::recursive_mutex m_Mutex;

  // public:
  float max;
  float min;
  float slopetarg;

public:
  Range()
  {
    min = -2;
    max = -2;
    slopetarg = -2.0;
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

/* \var Range *range = new Range();
 * \brief range instantiation for the Range class
 */

/**
 * \addtogroup Variables Global variables
*/
/**@{*/

/* \var Range *range = new Range();
 * \brief range instantiation for the Range class
 */

Range *range = new Range();


/**@}*/


/*! \class StatusClass
 * \brief   indicates whether a/c is on or off using a mutex
 */
/* \var ACrun physicalstate
 * \brief physicalstate of the a/c  ON or OFF
 */

class StatusClass
{
private:
  std::recursive_mutex m_Mutex;

private:
  Canrun permitted_state; // permission to run   STOP(no) or OFF(yes)
  ACrun physicalstate;  // is the a/c physically on or off?
  Room runroom; // bedroom or living
  int serverport;     // tcpip port to receive cmds
  int logport = 5190; // logging port
  int trim = 8;	      // temp readings to keep for slope calc


public:
  StatusClass() //: permitted_state(STOP)
  {
    runroom = (Room)-2;
    permitted_state = STOP;
    serverport = -2;
    physicalstate = ACOFF;
    logport = 5190;
    trim = 8;
  }

  /*! \brief  set state of STOP or NEINGO
   *
   */
  void setpermittedstate(Canrun statearg)
  {
    MutexGuard scopedLock(m_Mutex); // lock the mutex
    permitted_state = statearg;
    // mutex automatically unlocked
  }

  /*! \brief read permission of STOP or NEINGO
   *
   */
  Canrun readpermittedstate()
  {
    MutexGuard scopedLock(m_Mutex); // lock the mutex
    return permitted_state;
    // mutex automatically unlocked
  }





  /*! \brief return string of physicalstate of STOP or OFF
   *
   */
  const char *prnpermittedstatestr()
  {
    MutexGuard scopedLock(m_Mutex); // lock the mutex
    switch (permitted_state)
    {
    case STOP:
      return "STOP";
      break;

    case OFF:
      return "OFF";
      break;

    default:
      return "UNKN";
    } // end switch

    return "UNKN";

    // mutex automatically unlocked
  }

  /*! \brief  set state of a/c being ON or OFF
   *
   */
  void setphysicalstate(ACrun statearg)
  {
    MutexGuard scopedLock(m_Mutex); // lock the mutex
    physicalstate = statearg;
    // mutex automatically unlocked
  }

  /*! \brief read physicalstate of a/c being ON or OFF
   *
   */
  ACrun readphysicalstate()
  {
    MutexGuard scopedLock(m_Mutex); // lock the mutex
    return physicalstate;
    // mutex automatically unlocked
  }

  /*! \brief set room of LIV or BED
   *
   */
  void setroom(Room room)
  {
    MutexGuard scopedLock(m_Mutex); // lock the mutex
    runroom = room;
    if (room == LIV)
      {
      serverport = LIVPORT;
      slopeport = SLOPELIV;
      }
    if (room == BED)
      {
      serverport = BEDPORT;
      slopeport = SLOPEBED;
      }

    // mutex automatically unlocked
  }

  /*! \brief read room, either LIV or BED
   *
   */
  int readroom()
  {

    MutexGuard scopedLock(m_Mutex); // lock the mutex

    return runroom;
    // mutex automatically unlocked
  }

  const char *prnreadroom()
  {
    MutexGuard scopedLock(m_Mutex); // lock the mutex
    switch (runroom)
      {
      case LIV:
	return "LIV";

      case BED:
	return "BED";
	
      case BADNULL:
	return "NADA";

      }
    return "NADA";
    
  } // prnreadroom




 /*! \brief set port program receives commands on
   *
   */
  void setserverport(int portarg)
  {
    MutexGuard scopedLock(m_Mutex); // lock the mutex
    serverport = portarg;
    // mutex automatically unlocked
  }

 /*! \brief read port program receives commands on
   *
   */
  int readserverport()
  {
    MutexGuard scopedLock(m_Mutex); // lock the mutex
    return serverport;
    // mutex automatically unlocked
  }


 /*! \brief set port program sends logging to
   *
   */  
  void setlogport(int portarg)
  {
    MutexGuard scopedLock(m_Mutex); // lock the mutex
    logport = portarg;
    // mutex automatically unlocked
  }

 /*! \brief read port program sends logging to
   *
   */  
  int readlogport()
  {
    MutexGuard scopedLock(m_Mutex); // lock the mutex
    return logport;
    // mutex automatically unlocked
  }

 /*! \brief set number of temp readings use to compute slope
   *
   */  
  void settrim(int trimarg)
  {
    MutexGuard scopedLock(m_Mutex); // lock the mutex
    trim = trimarg;
    // mutex automatically unlocked
  }

 /*! \brief read number of temp readings use to compute slope
   *
   */  
  long unsigned int readtrim()
  {
    MutexGuard scopedLock(m_Mutex); // lock the mutex
    return trim;
    // mutex automatically unlocked
  }




  /*! \brief prints string ON or OFF given value of class variable physicalstate
   *
   *
   */
  const char *prnphysicalstate()
  {
    return (const char *)(readphysicalstate() == ACON) ? "ON" : "OFF";
  }

}; // StatusClass

/**
 * \addtogroup Variables Global variables
*/
/**@{*/


StatusClass *ThermoStatus = new StatusClass();


/**@}*/


/*! \brief print the status in response to command over net
 *
 */

void print_status()
{

  sout("Camera %d Debug %d  State %s  Ignoreslope %d Room %s canrun %s", camera,
       debug, ThermoStatus->prnphysicalstate(), ignoreslope,
       ThermoStatus->prnreadroom() ,
       ThermoStatus->prnpermittedstatestr());

  log172d("Camera %d Debug %d State %s Ignoreslope %d Room %s canrun %s",
          camera, debug, ThermoStatus->prnphysicalstate(), ignoreslope,
          ThermoStatus->prnreadroom(),
          ThermoStatus->prnpermittedstatestr()
	  );
}




///  @cond NO
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

///  @endcond

/*! \def QLEN
 * \brief length of network queue
 */

#define QLEN 16 /* size of request queue        */

bool isacon(); // function prototype

/*! \brief handles message from tcp socket  in thread
 *     \showrefs
 *
 * \param csock the socket to handle
 *
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
    //      ThermoStatus->permitted_state = 1;
    ThermoStatus->setpermittedstate(OFF); //    ThermoStatus->setpermittedstate(NEINGO);
    log172("START START");
    log172d("START START");
    fflush(stdout);

    // if the temp is higher than MIN than turn on the a/c

    if (currenttemp >= range->readmin() &&
        ACOFF ==
            ThermoStatus->readphysicalstate()) // and also make sure not running
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

    ThermoStatus->setpermittedstate(STOP);
    //      ThermoStatus->permitted_state = 0;
    log172("STOP STOP state %s", ThermoStatus->prnphysicalstate());
    log172d("STOP STOP state %s", ThermoStatus->prnphysicalstate());
    fflush(stdout);
    if (LIV == ThermoStatus->readroom())
    {
      putnetval((char *)"192.168.1.237", 5010, "STOP");
    }
    else
    {
      putnetval((char *)"192.168.1.237", 5009, "STOP"); // bed
    }

    if (ThermoStatus->readphysicalstate() == ACON) // if the a/c is running
    {
      turnoff();
      log172d("changing logger to STOP from state %s",
              ThermoStatus->prnphysicalstate());
      ThermoStatus->setpermittedstate(STOP); // to make sure the physicalstate doesn't become OFF
      log172d("a/c was running, changing to STOP");

      ThermoStatus->setpermittedstate(STOP);
    }
  } //  STOP

  /*********************NEINGO*************************/
  if (0 == strncasecmp("00unstop", buffer, (buffer_len >= 4) ? 4 : buffer_len))
  {
    //      sleep(1);
    sout("neingo set");
    fflush(stdout);

    ThermoStatus->setpermittedstate(OFF); //    ThermoStatus->setpermittedstate(NEINGO);
    //      ThermoStatus->permitted_state = 0;
    log172("NEINGO NEINGO");
    log172d("NEINGO NEINGO");
    fflush(stdout);
    if (LIV == ThermoStatus->readroom())
    {
      log172d("changing logger to NEINGO from state %s",
              ThermoStatus->prnphysicalstate());
      putnetval((char *)"192.168.1.237", 5010, "OFF");
      log172d("NEINGO liv");
    }
    else
    {
      log172d("changing logger to OFF from state %s", ThermoStatus->prnphysicalstate());
      putnetval((char *)"192.168.1.237", 5009, "OFF"); // bed
      log172d("NEINGO bed");
    }

///  @cond NO
#if 0
      if (physicalstate == ON )	// if the a/c is running
	{
	  turnoff();
	}
#endif
    ///  @endcond

  } //  NEINGO

  /*********************LOG*************************/
  if (0 == strncasecmp("log", buffer, (buffer_len >= 3) ? 3 : buffer_len))
  {
    //      sleep(1);
    sout("log: %s", &buffer[4]);

    fflush(stdout);

    log172d("log: %s", &buffer[4]);

  } //  LOG

  /*********************LOG*************************/
  if (0 == strncasecmp("off", buffer, (buffer_len >= 3) ? 3 : buffer_len))
  {

    fflush(stdout);
    ThermoStatus->setpermittedstate(OFF); //    ThermoStatus->setpermittedstate(NEINGO);
    log172d("off");

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
 *     \showrefs
 */
void waitthread(int a)
{

  struct sockaddr_in sadr;
  socklen_t addr_size;
  int port = ThermoStatus->readserverport(); // was 5020;
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
    fprintf(stderr, "bind for port %d failed line %d\n", port, __LINE__);
    _exit(1);
  }

  /*
     Specify a size of request queue
   */
  if (listen(sd, QLEN) < 0)
  {
    fprintf(stderr, "listen for port %d failed line %d\n", port, __LINE__);
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



///  @cond NO
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
///  @endcond

/*! \brief execute command to turn off/on the a/c
 *
 */

void cmd()
{
  assert((LIV == ThermoStatus->readroom()) | (BED == ThermoStatus->readroom()));

  if (LIV == ThermoStatus->readroom())
  {
    log172d("power irsend to liv");
    system(
        "cat /dev/null | irsend -a 192.168.1.8:8700  SEND_ONCE frigid power");
  }

  if (BED == ThermoStatus->readroom())
  {
    log172d("power irsend to bed");
    system(
        "cat /dev/null | irsend -a 192.168.1.194:8700  SEND_ONCE frigid power");
  }
}

/*! \var int actioncounter
 * \brief monotonic counter for number of times in currentstate
 */
int actioncounter = 0; /* incr each time */
                       /*
                          current physicalstate out of range
                        */

/*! \brief figure the slope between temperatures
 *
 * \return the slope calculated
 *
 */

float slope()
{
  time_t now;
  float the_answer;
  time(&now);


  if (avgtemp.size() > (ThermoStatus->readtrim()))
    {
    avgtemp.erase(avgtemp.begin(), avgtemp.begin() + (avgtemp.size() - ThermoStatus->readtrim()));
    //    log172d("avg trim %ld %ld",avgtemp.begin() , avgtemp.size());
    } // if

  if (avgtimet.size() > (ThermoStatus->readtrim()))
    avgtimet.erase(avgtimet.begin(), avgtimet.begin() + (avgtimet.size() - ThermoStatus->readtrim()));

  time_t oldtime = now - avgtimet.front();
  float tempdiff = avgtemp.back() - avgtemp.front();
  //  int x,y;
  //  x=avgtemp.size();
  //  y=x;

  //  time_t oldtime = now-avgtimet.front();
  //  float tempdiff = avgtemp.back()-avgtemp.front();

  if (oldtime > 0)
  {
    the_answer = (100 * tempdiff) / (oldtime);
  }
  else
  {
    the_answer = 0;
  }

  log172d("slope timdif=%ld 100Xtempdiff=%2.2f ans=%2.2f", oldtime,
          100 * tempdiff, the_answer);

  // send slope to logger
  number_to_send = the_answer;
  log_mut1_unlocked = true; // release the kracken/mutex
  g_cv.notify_one();        // Notify one waiting thread

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

  if (LIV == ThermoStatus->readroom())
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

  if (LIV == ThermoStatus->readroom())
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

  // fall thru to here if ac is not in desired physicalstate

  do
  {
    // emergency exit to stop the loop if STOP is set by runac

    if (ThermoStatus->readpermittedstate() == STOP)
      return;

    sleep(5);

    cmd();
    log172d("cmd called in checkitworked %d ", desire);

    if (0 == desire && isacoff())
      return;

    if (1 == desire && isacon())
      return;

    log172d("stuck ac in checkitworked desire %d  ", desire);

  } while (1);

} // check..

/*! \brief execute system commands to turn off ac and verify it
 *
 *     \showrefs
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

  actioncounter++;

  log172d("turnoff()   %.2f<min actioncounter %d", currenttemp, actioncounter);

  /*********************************************/
  // turn off when it hits the min temp
  // slope calc
  myslope = slope(/*timelaston, temp - templaston*/);

  // end slope calc

  avgtemp.clear(); // no longer need to collect times, clear after calling slope
  avgtimet.clear(); // no longer need to collect times, clear after calling slope
  avgtemp.push_back(currenttemp); //  templastoff = currenttemp;
  avgtimet.push_back(std::time(nullptr));      //  time(&timelastoff);

  log172d("turnoff() calling cmd()");
  cmd();

  log172("power off at %2.2f slope %2.2f turnoff line %d", currenttemp, myslope,
         __LINE__);
  log172d("power off at %2.2f slope %2.2f turnoff", currenttemp, myslope);
  // see, debugging called as well

  /*********************************************/
  /*********************************************/
  /******run python gnuplot data****************/
  /*********************************************/
  /*********************************************/

  if (LIV == ThermoStatus->readroom())
  {
    system("/u/reilly/bin_sec/runplotac liv");
  }

  if (BED == ThermoStatus->readroom())
  {
    system("/u/reilly/bin_sec/runplotac bed");
  }

  if (LIV == ThermoStatus->readroom())
  {
    log172d("turnoff() changing liv logger to OFF from state %s",
            ThermoStatus->prnphysicalstate());
    putnetval((char *)"192.168.1.237", 5010, "OFF");
  }
  else
  {
    log172d("turnoff() changing bed logger to OFF from state %s",
            ThermoStatus->prnphysicalstate());
    putnetval((char *)"192.168.1.237", 5009, "OFF"); // bed
  }

  ThermoStatus->setphysicalstate(ACOFF);
  actioncounter = 0;

  // DID IT WORK?
  checkitworked(0);

  /*********************************************/
}

/*!  \brief execute system commands to turn on ac and verify it
 *
 *     \showrefs
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
  }
  // end slope calc

  avgtemp.clear();  // restart collecting temps
  avgtimet.clear(); // restart collecting temps

  avgtemp.push_back(currenttemp); //  templaston = currenttemp;
  avgtimet.push_back(std::time(nullptr));      //  time(&timelaston);

  log172d("turnon() calling cmd()");
  cmd();
  ThermoStatus->setphysicalstate(ACON);
  log172d("cmd used for power on at %2.2f %2.2f %2.2f ", currenttemp, myslope,
          sum_elems);

  //  fillintime();

  log172("power on at %2.2f %2.2f %2.2f Line %d", currenttemp, myslope,
         sum_elems, __LINE__);

  //            fprintf(log, "%s power on at %2.2f\t%2.2f\n", timebuf, temp,
  //            myslope); fflush(log);

  if (LIV == ThermoStatus->readroom())
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

  if (LIV == ThermoStatus->readroom())
  {

    log172d("power double tap irsend to liv");
    system("cat /dev/null | irsend -a ac.sec.com:8700  SEND_ONCE frigid right");
    system("cat /dev/null | irsend -a ac.sec.com:8700  SEND_ONCE frigid right");
  }

  if (BED == ThermoStatus->readroom())
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

//int a /**< [in] docs for input parameter v. */)


/*!   \brief loop to control the a/c
 *
 */


void decider()
{

  /*!
   *  void decider()  loop to control the a/c
   *
   *
   * \showrefs
   */

  char *testret; /* test return */
  FILE *tempfile;

  if (LIV == ThermoStatus->readroom())
  {
    tempfile = fopen("/u/reilly/templog5", "r"); // input
  }

  if (BED == ThermoStatus->readroom())
  {
    tempfile = fopen("/u/reilly/templog4", "r"); // input
  }

  float min = 78.0, max = 81.2;

  // UNUSED #define QUIETTIME 60
  //    wait this long after sending cmd before the wall

  //  templaston = min;  // temp when last turned on
  //  templastoff = max; // temp when last turned off

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
  // UNEEDED as it was initialized as OFF
  //  physicalstate = ACOFF;
  // assume power = OFF;

  //  time(&timelastoff);
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

    if (LIV == ThermoStatus->readroom())
    {
      getnetval((char *)"192.168.1.237", 5201, valbuf);
      tempmin = atof(valbuf);
      if (tempmin > 20 && tempmin != min)
      {
        actioncounter = 0; // reset the 3-in-a-row
        min = tempmin;
        range->setmin(min);
        //        fillintime();
        log172("Min now %.2f", min);
      } // set the min
      getnetval((char *)"192.168.1.237", 5202, valbuf);
      tempmax = atof(valbuf);
      if (tempmax > 20 && tempmax != max)
      {
        actioncounter = 0; // reset the 3-in-a-row
        max = tempmax;
        range->setmax(max);
        //        fillintime();

        log172("Max now %.2f", max);
      } // set the max
    }   // if LIV
    else
    { // fall here for BED
      getnetval((char *)"192.168.1.237", 5204, valbuf);
      tempmin = atof(valbuf);
      if (tempmin > 20 && tempmin != min)
      {
        actioncounter = 0; // reset the 3-in-a-row
        min = tempmin;
        range->setmin(min);
        //        fillintime();
        log172("Min now %.2f", min);
      } // set the min
      getnetval((char *)"192.168.1.237", 5205, valbuf);
      tempmax = atof(valbuf);
      if (tempmax > 20 && tempmax != max)
      {
        actioncounter = 0; // reset the 3-in-a-row
        max = tempmax;
        range->setmax(max);
        //        fillintime();

        log172("Max now %.2f", max);
      } // set the max
    }   // else BED

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

    memset(tempbuf, '\0', sizeof(tempbuf));
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
    avgtimet.push_back(std::time(nullptr));

    // conditional found at
    // https://gcc.gnu.org/onlinedocs/cpp/System-specific-Predefined-Macros.html

#if __arm__
    log172d("t %2.2f '%s'  %u %d ", currenttemp, tempbuf, avgtemp.size(),
            ThermoStatus->readpermittedstate()

    );

#else
    log172d("t %2.2f '%s'  #%lu %2.2f %s ", currenttemp, tempbuf,
            avgtemp.size(), slope(), ThermoStatus->prnphysicalstate()

    );
#endif
    /**********************************************/
    //       end get stuff from templog
    /**********************************************/

    // actioncounter counts how many temps are matches(would cause action) in a
    // row

    if (currenttemp > min && currenttemp < max)
    {
      actioncounter = 0; /* not THRES in a row, reset counter */
      log172d(
          "not THRES in a row, reset counter   min %.2f  max %.2f temp %.2f",
          min, max, currenttemp);
      continue;
    }

    /***********************************/
    /***********************************/
    /*
       this will cut short the loop if ThermoStatus->permitted_state==0
     */
    /***********************************/

    if (ThermoStatus->readphysicalstate() == ACOFF &&
        ThermoStatus->readpermittedstate() == STOP)
    {
      log172d("continue b/c STOP");
      continue;
    }

    log172d("decid phys==%s  temp<=min %s  slope %2.2f permit %s  "
            "actioncounter %d  "
            ,ThermoStatus->prnphysicalstate()
	    , (currenttemp <= min) ? "yes" : "no", (slope())
	    , ThermoStatus->prnpermittedstatestr()
	    , actioncounter);

    // start should I stop it?
    //*******************************************************

    bool stoprun;

    // turn off if it is cold and the a/c is on
    stoprun = (currenttemp <= min && ThermoStatus->readphysicalstate() == ACON);
    if (stoprun)
    {
      log172d("decid stoprun=1 calling turnoff() ");
      turnoff();
    }

    // increment the counter that is compared to threshold
    if (ThermoStatus->readphysicalstate() == ACOFF && currenttemp >= max)
    {
      actioncounter++;

      log172d("state off %.2f>max %d ", currenttemp, actioncounter);
    }

    // fall in here if a/c off and it is colder than max
    if (ThermoStatus->readphysicalstate() == ACOFF && currenttemp < max)
    {
      actioncounter = 0;
      log172d("state still off %.2f<max %d ", currenttemp, actioncounter);
    }

    // fell though to here since action needs to be taken
    // make it here if up to threshold and decide what to do with a/c
    if (actioncounter >= THRES)
    {
      actioncounter = 0;

      switch (ThermoStatus->readphysicalstate())
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

      } /* physicalstate */

    } /* if actioncounter>THRES */

  } /* while 1 */
} /* decider */

/*!
  * \mainpage Thermostat   
  *
  */
int main(int argc /**< [in] count */, char *argv[] /**< [in] command line */)
{

  /*!
       int main(int argc, char *argv[])  the main show
   *
   * \showrefs
   */

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
    ThermoStatus->setroom(BED);
    camera = 1;
    debug = 1;
    ThermoStatus->setlogport(5191);
  }

  if (0 == strncmp("liv", argv[1], 3))
  {
    ThermoStatus->setroom(LIV);
    debug = 1;
  }

  if (LIV == ThermoStatus->readroom())
  {
    logd = fopen("/tmp/debug.log", "a+");
  }
  if (BED == ThermoStatus->readroom())
  {
    logd = fopen("/tmp/debug.log2", "a+");
  }

  //  STARTUP IN STOP AND MAKE SURE VISIBLE ON .237 TCL BOX

  if (LIV == ThermoStatus->readroom())
  {
    putnetval((char *)"192.168.1.237", 5010, "STOP");
    log172d("initial status set to STOP liv");
  }
  else
  {
    putnetval((char *)"192.168.1.237", 5009, "STOP"); // bed
    log172d("initial status set to STOP bed");
  }

  ThermoStatus->setpermittedstate(STOP);
  log172d("initial status set to STOP");

  //  what is this?

  //   assert(ThermoStatus->readroom());

  print_status();

/**
 * \defgroup Threads Threads start here
*/
/**@{*/


  //  std::thread th(decider,a);
  //  th.detach();
  //  th.join();
  std::thread th(waitthread, a);
  th.detach();

  //??  log_mut.lock();		// lock the logging thread
  //  std::thread thl(logthread, a);
  //  thl.detach();

  log_mut1_unlocked = false; // start as locked

  // Create a thread to send logging messages
  std::thread writer_thread(
      [&]() { logthread(); });

  //  decider();


  // Create a thread to send logging messages
    std::thread decider_thread(
        [&]() { decider(); });


/**@}*/
    
    while(1) { sleep(5100); }


  int filler = 34;
  filler = filler - 1;
} // main


//  after page was \tableofcontents  but not supported

/*! \page page1 A documentation page 
Leading text.
\section sec An example section
This page contains the subsections \ref subsection1 and \ref subsection2. For more info see page \ref page2.
\subsection subsection1 The first subsection
Text.
\subsection subsection2 The second subsection More text.
*/
/*! \page page2 Another page Even more info.
*/
