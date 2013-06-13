/**
 * @file   Conf.cpp
 * @author zhoushu
 * @date   Sat Oct  8 15:26:16 2011
 * 
 * @brief  
 * 
 * 
 */
#include "conf.h"
#include "ul_conf.h"

Conf *Conf::instance_ = NULL;

Conf::Conf()
{
    confdata_ = ul_initconf(CONF_MAX_ITEM_NUM);
}

Conf::~Conf()
{
    if (NULL != confdata_){
        ul_freeconf(confdata_);
    }
}


Conf *Conf::getSharedInstance()
{
    if (NULL == instance_){
        instance_ = new Conf();
    }

    return instance_;
}

void Conf::destroySharedInstance()
{
    if (NULL != instance_){
        delete instance_;
        instance_ = NULL;
    }
}    


int Conf::openConfFile(const char *fullpath, const char *filename)
{
    memset(this->fullPath_, 0, 2048);
    memset(this->fileName_, 0, 2048);
    strncpy(this->fullPath_, fullpath, 2048);
    strncpy(this->fileName_, filename, 2048);

    return ul_readconf(fullpath, filename, this->confdata_);
}

int Conf::saveConfFile()
{
    return ul_writeconf(this->fullPath_, this->fileName_, this->confdata_);
}

void Conf::getConfValueByKey(const char *key, ConfType t, void *value, int n)
{
    if (NULL == key || '\0' == key[0] || NULL == value)
    {
        return;
    }

    switch(t){
    case CONF_VALUE_TYPE_STR:
        ul_getconfnstr(this->confdata_, (char *)key, (char *)value, n);
        break;
    case CONF_VALUE_TYPE_INT32:
        ul_getconfint(this->confdata_, (char *)key, (int *)value);
        break;
    case CONF_VALUE_TYPE_FLOAT:
        ul_getconffloat(this->confdata_, (char *)key, (float *)value);
        break;
    case CONF_VALUE_TYPE_UINT32:
        ul_getconfuint(this->confdata_, (char *)key, (u_int *)value);
        break;
    case CONF_VALUE_TYPE_INT64:
        ul_getconfint64(this->confdata_, (char *)key, (long long *)value);
        break;
    case CONF_VALUE_TYPE_UINT64: 
        ul_getconfuint64(this->confdata_, (char *)key, (unsigned long long *)value);
        break;
    }
}

void Conf::addConfValue(const char *key, void *value, ConfType t, int n)
{
    if (NULL == key || NULL == value || 0 == n)
    {
        return;
    }

    switch(t){
    case CONF_VALUE_TYPE_STR:
        ul_addconfstr(this->confdata_, (char *)key, (char *)value);
        break;
    case CONF_VALUE_TYPE_INT32:
        ul_addconfint(this->confdata_, (char *)key, *(int *)value);
        break;
    case CONF_VALUE_TYPE_FLOAT:
        ul_addconffloat(this->confdata_, (char *)key, *(float *)value);
        break;
    case CONF_VALUE_TYPE_UINT32:
        ul_addconfuint(this->confdata_, (char *)key, *(u_int *)value);
        break;
    case CONF_VALUE_TYPE_INT64:
        ul_addconfint64(this->confdata_, (char *)key, *(long long *)value);
        break;
    case CONF_VALUE_TYPE_UINT64: 
        ul_addconfuint64(this->confdata_, (char *)key, *(unsigned long long *)value);
        break;
    }    
}
