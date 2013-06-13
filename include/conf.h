/**
 * @file   Conf.h
 * @author zhoushu
 * @date   Sat Oct  8 15:02:56 2011
 * 
 * @brief  ≈‰÷√Œƒº˛¿‡
 * 
 * 
 */
#ifndef _CONF_H_
#define _CONF_H_
#include "ul_conf.h"

#define CONF_MAX_ITEM_NUM 1024

typedef enum ConfType_t{
    CONF_VALUE_TYPE_STR = 0,
    CONF_VALUE_TYPE_INT32,
    CONF_VALUE_TYPE_INT64,
    CONF_VALUE_TYPE_UINT32,
    CONF_VALUE_TYPE_UINT64,
    CONF_VALUE_TYPE_FLOAT
}ConfType;

class Conf{
public:
    static Conf *getSharedInstance();
    static void destroySharedInstance();
    int openConfFile(const char *fullpath, const char *filename);
    int saveConfFile();
    void getConfValueByKey(const char *key, ConfType t, void *value, int n);
    void addConfValue(const char *key, void *value, ConfType t, int n);
    virtual ~Conf();
protected:
    Conf();

private:
    static Conf *instance_;
    ul_confdata_t *confdata_;
    char fullPath_[2048];
    char fileName_[2048];
};


#endif
