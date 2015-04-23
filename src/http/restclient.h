/**
 * @file restclient.h
 * @brief libcurl wrapper for REST calls
 * @author Daniel Schauenberg <d@unwiredcouch.com>
 * @version
 * @date 2010-10-11
 */

#ifndef INCLUDE_RESTCLIENT_H_
#define INCLUDE_RESTCLIENT_H_

#include <curl/curl.h>
#include <string>
#include <map>
#include <vector>
#include <cstdlib>
#include "meta.h"
#include <algorithm>

typedef enum {
    HTTP_EXEC_STATUS_WAIT_INSTALL = 0,
    HTTP_EXEC_STATUS_UNSUCCESS,
    HTTP_EXEC_STATUS_DOING,
    HTTP_EXEC_STATUS_SUCCESS,
} HTTP_EXEC_STATUS;

enum {
    REST_CODE_FAILED = -1,
    REST_CODE_OK = 200,
    REST_CODE_CREATE = 201,
    REST_CODE_NO_CONTENT = 204,
    REST_CODE_BAD_REQUEST = 400,
    REST_CODE_FORBIDDEN = 401,
    REST_CODE_NOT_FOUND = 404,
    REST_CODE_NOT_ALLOWED = 405,
    REST_CODE_CONFLICT = 409,
    REST_CODE_INTERNAL_ERROR = 500,

    REST_CODE_USER_BEGIN = 400000,

    REST_CODE_TOKEN_INVALID = 400002,
    REST_CODE_JSON_ERROR = 400003,
    REST_CODE_UNKNOW_ERROR = 400004,
};

class RestClient
{
  public:
    /**
     * public data definitions
     */
    typedef std::map<std::string, std::string> headermap;
    typedef std::vector<std::string> ctypelist;

    /** response struct for queries */
    typedef struct
    {
      int code;
      int code_http;
      std::string body;
      headermap headers;
    } response;
    /** struct used for uploading data */
    typedef struct
    {
      const char* data;
      size_t length;
    } upload_object;

    /** public methods */
    // Auth
    static void clearAuth();
    static void setAuth(const std::string& user,const std::string& password);
    // HTTP GET
    static response get(const std::string& url, const ctypelist& ctypes);
    // HTTP POST
    static response post(const std::string& url, const ctypelist& ctypes, const std::string& data);
    // HTTP PUT
    static response put(const std::string& url, const ctypelist& ctypes, const std::string& data);
    // HTTP DELETE
    static response del(const std::string& url, const ctypelist& ctypes);

    static bool download(const std::string& url, const std::string& filename, int timeout = -1);

    static int  downloadProgress();

    static void globalInit();
    static void clearAll();


    // HTTP GET
//    static response get_SystemCall(const std::string& url, const ctypelist& ctypes, const std::string& tmpFileName);
    // HTTP POST
//    static response post_SystemCall(const std::string& url, const ctypelist& ctypes, const std::string& data, const std::string& tmpFileName);
    // HTTP PUT
//    static response put_SystemCall(const std::string& url, const ctypelist& ctypes, const std::string& data, const std::string& tmpFileName);


private:
    // writedata callback function
    static size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata);

    // header callback function
    static size_t header_callback(void *ptr, size_t size, size_t nmemb, void *userdata);
    // read callback function
    static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *userdata);

    static const char* user_agent;
    static std::string user_pass;

    // trim from start
    static inline std::string &ltrim(std::string &s) {
      s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
      return s;
    }

    // trim from end
    static inline std::string &rtrim(std::string &s) {
      s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
      return s;
    }

    // trim from both ends
    static inline std::string &trim(std::string &s) {
      return ltrim(rtrim(s));
    }
};

#endif  // INCLUDE_RESTCLIENT_H_
