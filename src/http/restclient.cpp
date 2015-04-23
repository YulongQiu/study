/**
 * @file restclient.cpp
 * @brief implementation of the restclient class
 * @author Daniel Schauenberg <d@unwiredcouch.com>
 */


/*========================
         INCLUDES
  ========================*/
#include "restclient.h"

#include <cstring>
#include <string>
#include <iostream>
#include <map>

#include "SystemInfo.h"

#define REST_CONNECT_TIMEOUT_STR std::string("20")

#define REST_CONNECT_TIMEOUT  20
#define REST_DEFAULT_TIMEOUT  30
#define REST_DOWNLOAD_TIMEOUT 60
#define REST_TCP_KEEPIDLE     10L
#define REST_TCP_KEEPINTVL    5L

/** initialize user agent string */
//const char* RestClient::user_agent = "restclient-cpp/" VERSION;
//const char* RestClient::user_agent = "xpower 1.0";
const char* RestClient::user_agent = "Xweb/1.0";
/** initialize authentication variable */
std::string RestClient::user_pass =  std::string();
/** Authentication Methods implementation */
void RestClient::clearAuth(){
  RestClient::user_pass.clear();
}
void RestClient::setAuth(const std::string& user,const std::string& password){
  RestClient::user_pass.clear();
  RestClient::user_pass += user+":"+password;
}
/**
 * @brief HTTP GET method
 *
 * @param url to query
 *
 * @return response struct
 */
RestClient::response RestClient::get(const std::string& url, const ctypelist& ctypes)
{
  /** create return struct */
  RestClient::response ret;

  // use libcurl
  CURL *curl;
  CURLcode res;

  //curl_global_init(CURL_GLOBAL_DEFAULT);
  curl = curl_easy_init();
  if (curl)
  {
    /** set basic authentication if present*/
    if(RestClient::user_pass.length()>0){
      curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
      curl_easy_setopt(curl, CURLOPT_USERPWD, RestClient::user_pass.c_str());
    }
    /** set user agent */
    curl_easy_setopt(curl, CURLOPT_USERAGENT, RestClient::user_agent);
    /** set query URL */
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    /** set callback function */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, RestClient::write_callback);
    /** set data object to pass to callback function */
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ret);
    /** set the header callback function */
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, RestClient::header_callback);
    /** callback object for headers */
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &ret);

#ifdef ENABLE_KEEP_ALIVE
    /* enable TCP keep-alive for this transfer */
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
    /* keep-alive idle time to 120 seconds */
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPIDLE, REST_TCP_KEEPIDLE);
    /* interval time between keep-alive probes: 60 seconds */
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPINTVL, REST_TCP_KEEPINTVL);
#endif //ENABLE_KEEP_ALIVE

    //Set Default timeout
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, REST_DEFAULT_TIMEOUT);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, REST_DEFAULT_TIMEOUT);

    /** set content-type header */
    curl_slist* header = NULL;
    for(size_t i = 0; i < ctypes.size(); ++i) {
        header = curl_slist_append(header, ctypes[i].c_str());
    }

    if (header)
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header);

    /** perform the actual query */
    res = curl_easy_perform(curl);

    if(header)
        curl_slist_free_all(header);

    ret.code_http = res;
    if (res != 0)
    {
      ret.body = "Failed to query.";
      ret.code = -1;
      curl_easy_cleanup(curl);
      return ret;
    }

    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    ret.code = static_cast<int>(http_code);

    curl_easy_cleanup(curl);
    //curl_global_cleanup();
  }

  return ret;
}
/**
 * @brief HTTP POST method
 *
 * @param url to query
 * @param ctype content type as string
 * @param data HTTP POST body
 *
 * @return response struct
 */
RestClient::response RestClient::post(const std::string& url,
                                      const ctypelist& ctypes,
                                      const std::string& data)
{
  /** create return struct */
  RestClient::response ret;

  // use libcurl
  CURL *curl;
  CURLcode res;

#ifndef _NLOG_
  // SystemInfo::logFile("/tmp/log/test.log", "request time", "start");
#endif

  //curl_global_init(CURL_GLOBAL_DEFAULT);
  curl = curl_easy_init();
  if (curl)
  {
    /** set basic authentication if present*/
    if(RestClient::user_pass.length()>0){
      curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
      curl_easy_setopt(curl, CURLOPT_USERPWD, RestClient::user_pass.c_str());
    }
    /** set user agent */
    curl_easy_setopt(curl, CURLOPT_USERAGENT, RestClient::user_agent);
    /** set query URL */
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    /** Now specify we want to POST data */
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    /** set post fields */
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
    if ((data.size() > 0) && ('{' == data[0])) {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.find_last_of('}') + 1);
    } else {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.size());
    }
    /** set callback function */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, RestClient::write_callback);
    /** set data object to pass to callback function */
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ret);
    /** set the header callback function */
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, RestClient::header_callback);
    /** callback object for headers */
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &ret);

#ifdef ENABLE_KEEP_ALIVE
    /* enable TCP keep-alive for this transfer */
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
    /* keep-alive idle time to 120 seconds */
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPIDLE, REST_TCP_KEEPIDLE);
    /* interval time between keep-alive probes: 60 seconds */
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPINTVL, REST_TCP_KEEPINTVL);
#endif
    //Set Default timeout
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, REST_DEFAULT_TIMEOUT);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, REST_DEFAULT_TIMEOUT);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

    /** set content-type header */
    curl_slist* header = NULL;
    for(size_t i = 0; i < ctypes.size(); ++i) {
        header = curl_slist_append(header, ctypes[i].c_str());
    }
    header = curl_slist_append(header, "Content-Type: application/json");

    if (header)
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header);

    /** perform the actual query */
    res = curl_easy_perform(curl);

    if(header)
        curl_slist_free_all(header);

    ret.code_http = res;
    if (res != 0)
    {
      ret.body = "Failed to query.";
      ret.code = -1;
      curl_easy_cleanup(curl);
      return ret;
    }

    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    ret.code = static_cast<int>(http_code);

    curl_easy_cleanup(curl);
    //curl_global_cleanup();
  }

#ifndef _NLOG_
  // SystemInfo::logFile("/tmp/log/test.log", "request time", "end");
#endif

  return ret;
}
/**
 * @brief HTTP PUT method
 *
 * @param url to query
 * @param ctype content type as string
 * @param data HTTP PUT body
 *
 * @return response struct
 */
RestClient::response RestClient::put(const std::string& url,
                                     const ctypelist& ctypes,
                                     const std::string& data)
{
  /** create return struct */
  RestClient::response ret;
  /** build content-type header string */
  //std::string ctype_header = "Content-Type: " + ctype;

  /** initialize upload object */
  RestClient::upload_object up_obj;
  up_obj.data = data.c_str();
  up_obj.length = data.size();

  // use libcurl
  CURL *curl;
  CURLcode res;

  //curl_global_init(CURL_GLOBAL_DEFAULT);
  curl = curl_easy_init();
  if (curl)
  {
    /** set basic authentication if present*/
    if(RestClient::user_pass.length()>0){
      curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
      curl_easy_setopt(curl, CURLOPT_USERPWD, RestClient::user_pass.c_str());
    }
    /** set user agent */
    curl_easy_setopt(curl, CURLOPT_USERAGENT, RestClient::user_agent);
    /** set query URL */
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    /** Now specify we want to PUT data */
    curl_easy_setopt(curl, CURLOPT_PUT, 1L);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
    /** set read callback function */
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, RestClient::read_callback);
    /** set data object to pass to callback function */
    curl_easy_setopt(curl, CURLOPT_READDATA, &up_obj);
    /** set callback function */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, RestClient::write_callback);
    /** set data object to pass to callback function */
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ret);
    /** set the header callback function */
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, RestClient::header_callback);
    /** callback object for headers */
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &ret);
    /** set data size */
    curl_easy_setopt(curl, CURLOPT_INFILESIZE,
                     static_cast<long>(up_obj.length));

#ifdef ENABLE_KEEP_ALIVE
    /* enable TCP keep-alive for this transfer */
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
    /* keep-alive idle time to 120 seconds */
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPIDLE, REST_TCP_KEEPIDLE);
    /* interval time between keep-alive probes: 60 seconds */
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPINTVL, REST_TCP_KEEPINTVL);
#endif

    //Set Default timeout
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, REST_DEFAULT_TIMEOUT);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, REST_DEFAULT_TIMEOUT);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

    /** set content-type header */
    curl_slist* header = NULL;
    for(size_t i = 0; i < ctypes.size(); ++i) {
        header = curl_slist_append(header, ctypes[i].c_str());
    }
    header = curl_slist_append(header, "Content-Type: application/json");

    if(header)
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header);

    /** perform the actual query */
    res = curl_easy_perform(curl);

    if(header)
        curl_slist_free_all(header);

    ret.code_http = res;
    if (res != 0)
    {
      ret.body = "Failed to query.";
      ret.code = -1;
      curl_easy_cleanup(curl);
      return ret;
    }
    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    ret.code = static_cast<int>(http_code);

    curl_easy_cleanup(curl);
    //curl_global_cleanup();
  }

  return ret;
}
/**
 * @brief HTTP DELETE method
 *
 * @param url to query
 *
 * @return response struct
 */
RestClient::response RestClient::del(const std::string& url, const ctypelist& ctypes)
{
  /** create return struct */
  RestClient::response ret;

  /** we want HTTP DELETE */
  const char* http_delete = "DELETE";

  // use libcurl
  CURL *curl;
  CURLcode res;

  //curl_global_init(CURL_GLOBAL_DEFAULT);
  curl = curl_easy_init();
  if (curl)
  {
    /** set basic authentication if present*/
    if(RestClient::user_pass.length()>0){
      curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
      curl_easy_setopt(curl, CURLOPT_USERPWD, RestClient::user_pass.c_str());
    }
    /** set user agent */
    curl_easy_setopt(curl, CURLOPT_USERAGENT, RestClient::user_agent);
    /** set query URL */
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    /** set HTTP DELETE METHOD */
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, http_delete);
    /** set callback function */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, RestClient::write_callback);
    /** set data object to pass to callback function */
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ret);
    /** set the header callback function */
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, RestClient::header_callback);
    /** callback object for headers */
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &ret);

    //Set Default timeout
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, REST_DEFAULT_TIMEOUT);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, REST_DEFAULT_TIMEOUT);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

    /** set content-type header */
    curl_slist* header = NULL;
    for(size_t i = 0; i < ctypes.size(); ++i) {
        header = curl_slist_append(header, ctypes[i].c_str());
    }

    if(header)
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header);

    /** perform the actual query */
    res = curl_easy_perform(curl);

    if(header)
        curl_slist_free_all(header);

    ret.code_http = res;
    if (res != 0)
    {
      ret.body = "Failed to query.";
      ret.code = -1;
      curl_easy_cleanup(curl);
      return ret;
    }
    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    ret.code = static_cast<int>(http_code);

    curl_easy_cleanup(curl);
    //curl_global_cleanup();
  }

  return ret;
}

/**
 * @brief write callback function for libcurl
 *
 * @param data returned data of size (size*nmemb)
 * @param size size parameter
 * @param nmemb memblock parameter
 * @param userdata pointer to user data to save/work with return data
 *
 * @return (size * nmemb)
 */
size_t RestClient::write_callback(void *data, size_t size, size_t nmemb,
                            void *userdata)
{
  RestClient::response* r;
  r = reinterpret_cast<RestClient::response*>(userdata);
  r->body.append(reinterpret_cast<char*>(data), size*nmemb);

  return (size * nmemb);
}

/**
 * @brief header callback for libcurl
 * 
 * @param data returned (header line) 
 * @param size of data
 * @param nmemb memblock
 * @param userdata pointer to user data object to save headr data
 * @return size * nmemb;
 */
size_t RestClient::header_callback(void *data, size_t size, size_t nmemb,
                            void *userdata)
{
  RestClient::response* r;
  r = reinterpret_cast<RestClient::response*>(userdata);
  std::string header(reinterpret_cast<char*>(data), size*nmemb);
  size_t seperator = header.find_first_of(":");
  if ( std::string::npos == seperator ) { 
    //roll with non seperated headers... 
    trim(header); 
    if ( 0 == header.length() ){ 
	return (size * nmemb); //blank line;
    } 
    r->headers[header] = "present";
  } else {
    std::string key = header.substr(0, seperator);
    trim(key);
    std::string value = header.substr(seperator + 1);
    trim (value);
    r->headers[key] = value;
  }

  return (size * nmemb);
}

/**
 * @brief read callback function for libcurl
 *
 * @param pointer of max size (size*nmemb) to write data to
 * @param size size parameter
 * @param nmemb memblock parameter
 * @param userdata pointer to user data to read data from
 *
 * @return (size * nmemb)
 */
size_t RestClient::read_callback(void *data, size_t size, size_t nmemb,
                            void *userdata)
{
  /** get upload struct */
  RestClient::upload_object* u;
  u = reinterpret_cast<RestClient::upload_object*>(userdata);
  /** set correct sizes */
  size_t curl_size = size * nmemb;
  size_t copy_size = (u->length < curl_size) ? u->length : curl_size;
  /** copy data to buffer */
  memcpy(data, u->data, copy_size);
  /** decrement length and increment data pointer */
  u->length -= copy_size;
  u->data += copy_size;
  /** return copied size */
  return copy_size;
}

/**
  *  download
  *
  */
#include <stdio.h>

#define STOP_DOWNLOAD_AFTER_THIS_MANY_BYTES         6000
#define MINIMAL_PROGRESS_FUNCTIONALITY_INTERVAL     3

static size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written;
    written = fwrite(ptr, size, nmemb, stream);
    return written;
}

struct CurlProgress {
  double lastruntime;
  CURL *curl;
};

static int s_curl_progress = 0;

/* this is how the CURLOPT_XFERINFOFUNCTION callback works */
static int xferinfo(void *p,
                    curl_off_t dltotal, curl_off_t dlnow,
                    curl_off_t ultotal, curl_off_t ulnow)
{
  struct CurlProgress *myp = (struct CurlProgress *)p;
  CURL *curl = myp->curl;
  double curtime = 0;

  curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &curtime);

  /* under certain circumstances it may be desirable for certain functionality
     to only run every N seconds, in order to do this the transaction time can
     be used */
  if((curtime - myp->lastruntime) >= MINIMAL_PROGRESS_FUNCTIONALITY_INTERVAL) {
    myp->lastruntime = curtime;
    fprintf(stderr, "TOTAL TIME: %f \r\n", curtime);
  }

//  fprintf(stderr, "UP: %" CURL_FORMAT_CURL_OFF_T " of %" CURL_FORMAT_CURL_OFF_T
//          "  DOWN: %" CURL_FORMAT_CURL_OFF_T " of %" CURL_FORMAT_CURL_OFF_T
//          "\r\n",
//          ulnow, ultotal, dlnow, dltotal);

//  if(dlnow > STOP_DOWNLOAD_AFTER_THIS_MANY_BYTES)
//    return 1;

  s_curl_progress = (dlnow * 100) / (dltotal + 1);

  fprintf(stderr, "TOTAL PROGRESS: %d \r\n", s_curl_progress);

  return 0;
}

/* for libcurl older than 7.32.0 (CURLOPT_PROGRESSFUNCTION) */
static int older_progress(void *p,
                          double dltotal, double dlnow,
                          double ultotal, double ulnow)
{
  return xferinfo(p,
                  (curl_off_t)dltotal,
                  (curl_off_t)dlnow,
                  (curl_off_t)ultotal,
                  (curl_off_t)ulnow);
}

int  RestClient::downloadProgress()
{
    return s_curl_progress;
}

bool RestClient::download(const std::string& url, const std::string& filename, int timeout)
{
    CURL *curl;
    FILE *fp;
    CURLcode res = CURLE_OK;

    struct CurlProgress prog;

    //curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    s_curl_progress = 0;

    if (curl) {
        prog.lastruntime = 0;
        prog.curl = curl;

        fp = fopen(filename.c_str(), "wb");
        if (NULL == fp) {
            curl_easy_cleanup(curl);
            return false;
        }
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

#if 0
        curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, older_progress);
          /* pass the struct pointer into the progress function */
        curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, &prog);

#if LIBCURL_VERSION_NUM >= 0x072000
        curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, xferinfo);
        curl_easy_setopt(curl, CURLOPT_XFERINFODATA, &prog);
#endif
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
#endif

        //Set Default timeout
        if(timeout > 0) {
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
            curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, REST_DEFAULT_TIMEOUT);
        }

        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        //curl_global_cleanup();
        fclose(fp);
        std::cout<<"url="<<url<<", filename="<<filename<<", res="<<res<<std::endl;
    } else {
        res = CURLE_FAILED_INIT;
    }

    return res == CURLE_OK;
}

void RestClient::globalInit()
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

void RestClient::clearAll()
{
    curl_global_cleanup();
}






// HTTP GET
/* RestClient::response RestClient::get_SystemCall(const std::string& url, const ctypelist& ctypes, const std::string& tmpFileName)
{
    std::string cmd = "curl -X GET -H 'Cache-Control: no-cache' -H 'Content-Type: application/json' ";
    for (size_t i=0; i < ctypes.size(); ++i) {
        cmd += "-H '"+ ctypes[i] + "' ";
    }

    cmd += " '" + url + "' ";
    cmd += " --connect-timeout "+REST_CONNECT_TIMEOUT_STR;
    cmd += " -s ";

    response res = {REST_CODE_OK};

    if (SystemInfo::systemCall(cmd, res.body, tmpFileName) < 0) {
        res.code = REST_CODE_FAILED;
    }

    return res;
} */

// HTTP POST
/* RestClient::response RestClient::post_SystemCall(const std::string& url, const ctypelist& ctypes, const std::string& data, const std::string& tmpFileName)
{
    std::string cmd = "curl -X POST -H 'Cache-Control: no-cache' -H 'Content-Type: application/json' ";
    for (size_t i=0; i < ctypes.size(); ++i) {
        cmd += "-H '"+ ctypes[i] + "' ";
    }

    cmd += " -d '" + data + "' ";

    cmd += " '" + url + "' ";
    cmd += " --connect-timeout "+REST_CONNECT_TIMEOUT_STR;
    cmd += " -s ";

    response res = {REST_CODE_OK};

    if (SystemInfo::systemCall(cmd, res.body, tmpFileName) < 0) {
        res.code = REST_CODE_FAILED;
    }

    return res;
}

// HTTP PUT
RestClient::response RestClient::put_SystemCall(const std::string& url, const ctypelist& ctypes, const std::string& data, const std::string& tmpFileName)
{
    std::string cmd = "curl -X PUT -H 'Cache-Control: no-cache' -H 'Content-Type: application/json' ";
    for (size_t i=0; i < ctypes.size(); ++i) {
        cmd += "-H '"+ ctypes[i] + "' ";
    }

    cmd += " -d '" + data + "' ";

    cmd += " '" + url + "' ";
    cmd += " --connect-timeout "+REST_CONNECT_TIMEOUT_STR;
    cmd += " -s ";

    response res = {REST_CODE_OK};

    if (SystemInfo::systemCall(cmd, res.body, tmpFileName) < 0) {
        res.code = REST_CODE_FAILED;
    }

    return res;
}

/*bool RestClient::download_SystemCall(const std::string& url, const std::string& filename, const std::string& tmpFileName, int timeout)
{
    std::string cmd = "curl -X GET -H 'Cache-Control: no-cache' ";

    cmd += " -o '" + filename + "' ";
    cmd += " '" + url + "' ";
//    cmd += " --connect-timeout "+REST_DEFAULT_TIMEOUT;
    cmd += " -m " + timeout;
    cmd += " -s ";

    std::string output;
    if (SystemInfo::systemCall(cmd, output, tmpFileName) < 0) {
        return false;
    }
    return true;
}*/
