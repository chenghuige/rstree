#COMAKE2 edit-mode: -*- Makefile -*-
####################64Bit Mode####################
ifeq ($(shell uname -m),x86_64)
CC=gcc
CXX=g++
CXXFLAGS=-g \
  -pipe \
  -W \
  -Wall \
  -fPIC
CFLAGS=-g \
  -pipe \
  -W \
  -Wall \
  -fPIC
CPPFLAGS=-D_GNU_SOURCE \
  -D__STDC_LIMIT_MACROS \
  -DVERSION=\"1.9.8.7\"
INCPATH=-I. \
  -I./include \
  -I./output \
  -I./output/include
DEP_INCPATH=-I../../../../../com/idlcompiler \
  -I../../../../../com/idlcompiler/include \
  -I../../../../../com/idlcompiler/output \
  -I../../../../../com/idlcompiler/output/include \
  -I../../../../../inf/computing/ant \
  -I../../../../../inf/computing/ant/include \
  -I../../../../../inf/computing/ant/output \
  -I../../../../../inf/computing/ant/output/include \
  -I../../../../../inf/computing/java6 \
  -I../../../../../inf/computing/java6/include \
  -I../../../../../inf/computing/java6/output \
  -I../../../../../inf/computing/java6/output/include \
  -I../../../../../inf/computing/zookeeper \
  -I../../../../../inf/computing/zookeeper/include \
  -I../../../../../inf/computing/zookeeper/output \
  -I../../../../../inf/computing/zookeeper/output/include \
  -I../../../../../lib2-64/bsl \
  -I../../../../../lib2-64/bsl/include \
  -I../../../../../lib2-64/bsl/output \
  -I../../../../../lib2-64/bsl/output/include \
  -I../../../../../lib2-64/ccode \
  -I../../../../../lib2-64/ccode/include \
  -I../../../../../lib2-64/ccode/output \
  -I../../../../../lib2-64/ccode/output/include \
  -I../../../../../lib2-64/dict \
  -I../../../../../lib2-64/dict/include \
  -I../../../../../lib2-64/dict/output \
  -I../../../../../lib2-64/dict/output/include \
  -I../../../../../lib2-64/string \
  -I../../../../../lib2-64/string/include \
  -I../../../../../lib2-64/string/output \
  -I../../../../../lib2-64/string/output/include \
  -I../../../../../lib2-64/ullib \
  -I../../../../../lib2-64/ullib/include \
  -I../../../../../lib2-64/ullib/output \
  -I../../../../../lib2-64/ullib/output/include \
  -I../../../../../op/oped/noah/webfoot/naming-lib \
  -I../../../../../op/oped/noah/webfoot/naming-lib/include \
  -I../../../../../op/oped/noah/webfoot/naming-lib/output \
  -I../../../../../op/oped/noah/webfoot/naming-lib/output/include \
  -I../../../../../public/bslext \
  -I../../../../../public/bslext/include \
  -I../../../../../public/bslext/output \
  -I../../../../../public/bslext/output/include \
  -I../../../../../public/comlog-plugin \
  -I../../../../../public/comlog-plugin/include \
  -I../../../../../public/comlog-plugin/output \
  -I../../../../../public/comlog-plugin/output/include \
  -I../../../../../public/configure \
  -I../../../../../public/configure/include \
  -I../../../../../public/configure/output \
  -I../../../../../public/configure/output/include \
  -I../../../../../public/connectpool \
  -I../../../../../public/connectpool/include \
  -I../../../../../public/connectpool/output \
  -I../../../../../public/connectpool/output/include \
  -I../../../../../public/gm/galileo \
  -I../../../../../public/gm/galileo/include \
  -I../../../../../public/gm/galileo/output \
  -I../../../../../public/gm/galileo/output/include \
  -I../../../../../public/idlcompiler \
  -I../../../../../public/idlcompiler/include \
  -I../../../../../public/idlcompiler/output \
  -I../../../../../public/idlcompiler/output/include \
  -I../../../../../public/mcpack \
  -I../../../../../public/mcpack/include \
  -I../../../../../public/mcpack/output \
  -I../../../../../public/mcpack/output/include \
  -I../../../../../public/nshead \
  -I../../../../../public/nshead/include \
  -I../../../../../public/nshead/output \
  -I../../../../../public/nshead/output/include \
  -I../../../../../public/odict \
  -I../../../../../public/odict/include \
  -I../../../../../public/odict/output \
  -I../../../../../public/odict/output/include \
  -I../../../../../public/spreg \
  -I../../../../../public/spreg/include \
  -I../../../../../public/spreg/output \
  -I../../../../../public/spreg/output/include \
  -I../../../../../public/ub \
  -I../../../../../public/ub/include \
  -I../../../../../public/ub/output \
  -I../../../../../public/ub/output/include \
  -I../../../../../public/uconv \
  -I../../../../../public/uconv/include \
  -I../../../../../public/uconv/output \
  -I../../../../../public/uconv/output/include \
  -I../../../../../third-64/boost \
  -I../../../../../third-64/boost/include \
  -I../../../../../third-64/boost/output \
  -I../../../../../third-64/boost/output/include \
  -I../../../../../third-64/pcre \
  -I../../../../../third-64/pcre/include \
  -I../../../../../third-64/pcre/output \
  -I../../../../../third-64/pcre/output/include \
  -I../../../../../third-64/protobuf \
  -I../../../../../third-64/protobuf/include \
  -I../../../../../third-64/protobuf/output \
  -I../../../../../third-64/protobuf/output/include \
  -I../../../../../third-64/stlport \
  -I../../../../../third-64/stlport/include \
  -I../../../../../third-64/stlport/output \
  -I../../../../../third-64/stlport/output/include

#============ CCP vars ============
CCHECK=@ccheck.py
CCHECK_FLAGS=
PCLINT=@pclint
PCLINT_FLAGS=
CCP=@ccp.py
CCP_FLAGS=


#COMAKE UUID
COMAKE_MD5=ea09311e8ad0e80bcc2ae11be248ee57  COMAKE


.PHONY:all
all:rstree 
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mall[0m']"
	@echo "make all done"

.PHONY:comake2_makefile_check
comake2_makefile_check:
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mcomake2_makefile_check[0m']"
	#in case of error, update 'Makefile' by 'comake2'
	@echo "$(COMAKE_MD5)">comake2.md5
	@md5sum -c --status comake2.md5
	@rm -f comake2.md5

.PHONY:ccpclean
ccpclean:
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mccpclean[0m']"
	@echo "make ccpclean done"

.PHONY:clean
clean:ccpclean
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mclean[0m']"
	rm -rf rstree
	rm -rf ./output/bin/rstree
	rm -rf src/rstree_rstree_main.o
	rm -rf src/rstree_rstree.o
	rm -rf src/rstree_conf.o

.PHONY:dist
dist:
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mdist[0m']"
	tar czvf output.tar.gz output
	@echo "make dist done"

.PHONY:distclean
distclean:clean
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mdistclean[0m']"
	rm -f output.tar.gz
	@echo "make distclean done"

.PHONY:love
love:
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mlove[0m']"
	@echo "make love done"

rstree:src/rstree_rstree_main.o \
  src/rstree_rstree.o \
  src/rstree_conf.o
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mrstree[0m']"
	$(CXX) src/rstree_rstree_main.o \
  src/rstree_rstree.o \
  src/rstree_conf.o -Xlinker "-("  ../../../../../com/idlcompiler/astyle/libastyle.a \
  ../../../../../com/idlcompiler/cxx/libskeleton.a \
  ../../../../../com/idlcompiler/java/libjava_skeleton.a \
  ../../../../../com/idlcompiler/parser/libparser.a \
  ../../../../../com/idlcompiler/php/libphp_skeleton.a \
  ../../../../../inf/computing/zookeeper/output/c_client/lib/libzookeeper_mt.a \
  ../../../../../inf/computing/zookeeper/output/c_client/lib/libzookeeper_st.a \
  ../../../../../lib2-64/bsl/lib/libbsl.a \
  ../../../../../lib2-64/bsl/lib/libbsl_ResourcePool.a \
  ../../../../../lib2-64/bsl/lib/libbsl_archive.a \
  ../../../../../lib2-64/bsl/lib/libbsl_buffer.a \
  ../../../../../lib2-64/bsl/lib/libbsl_check_cast.a \
  ../../../../../lib2-64/bsl/lib/libbsl_exception.a \
  ../../../../../lib2-64/bsl/lib/libbsl_pool.a \
  ../../../../../lib2-64/bsl/lib/libbsl_utils.a \
  ../../../../../lib2-64/bsl/lib/libbsl_var.a \
  ../../../../../lib2-64/bsl/lib/libbsl_var_implement.a \
  ../../../../../lib2-64/bsl/lib/libbsl_var_utils.a \
  ../../../../../lib2-64/ccode/lib/libulccode.a \
  ../../../../../lib2-64/dict/lib/libuldict.a \
  ../../../../../lib2-64/string/lib/libstring.a \
  ../../../../../lib2-64/ullib/lib/libullib.a \
  ../../../../../op/oped/noah/webfoot/naming-lib/output/lib/libwebfoot_naming.a \
  ../../../../../public/bslext/output/lib/libbsl_bml.a \
  ../../../../../public/bslext/output/lib/libbsl_containers_utils.a \
  ../../../../../public/bslext/output/lib/libbsl_var_scripting.a \
  ../../../../../public/bslext/output/lib/libbsl_var_serializer.a \
  ../../../../../public/bslext/output/lib/libbsl_var_vscript.a \
  ../../../../../public/bslext/output/lib/libbsl_vs.a \
  ../../../../../public/bslext/output/lib/libbslext.a \
  ../../../../../public/comlog-plugin/libcomlog.a \
  ../../../../../public/comlog-plugin/output/lib/libdfsappender.a \
  ../../../../../public/configure/libconfig.a \
  ../../../../../public/connectpool/libconnectpool.a \
  ../../../../../public/gm/galileo/libgalileo.a \
  ../../../../../public/gm/galileo/output/lib/libzookeeper_mt.a \
  ../../../../../public/idlcompiler/output/lib/libmcpack_idl.a \
  ../../../../../public/mcpack/libmcpack.a \
  ../../../../../public/nshead/libnshead.a \
  ../../../../../public/odict/libodict.a \
  ../../../../../public/spreg/libspreg.a \
  ../../../../../public/ub/output/lib/libub.a \
  ../../../../../public/ub/output/lib/libub_aserver.a \
  ../../../../../public/ub/output/lib/libub_client.a \
  ../../../../../public/ub/output/lib/libub_conf.a \
  ../../../../../public/ub/output/lib/libub_galileo.a \
  ../../../../../public/ub/output/lib/libub_log.a \
  ../../../../../public/ub/output/lib/libub_misc.a \
  ../../../../../public/ub/output/lib/libub_monitor.a \
  ../../../../../public/ub/output/lib/libub_naming.a \
  ../../../../../public/ub/output/lib/libub_server.a \
  ../../../../../public/ub/output/lib/libubex.a \
  ../../../../../public/ub/output/lib/libubfw.a \
  ../../../../../public/uconv/libuconv.a \
  ../../../../../third-64/pcre/lib/libpcre.a \
  ../../../../../third-64/pcre/lib/libpcrecpp.a \
  ../../../../../third-64/pcre/lib/libpcreposix.a \
  ../../../../../third-64/protobuf/lib/libprotobuf-lite.a \
  ../../../../../third-64/protobuf/lib/libprotobuf.a \
  ../../../../../third-64/protobuf/lib/libprotoc.a -lpthread \
  -lcrypto \
  -lrt -Xlinker "-)" -o rstree
	mkdir -p ./output/bin
	cp -f --link rstree ./output/bin

src/rstree_rstree_main.o:src/rstree_main.cpp \
  include/rstree.h \
  include/rstree_def.h \
  include/conf.h \
  include/rstree_def.h \
  include/rstree_util.h
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40msrc/rstree_rstree_main.o[0m']"
	$(CXX) -c $(INCPATH) $(DEP_INCPATH) $(CPPFLAGS) $(CXXFLAGS)  -o src/rstree_rstree_main.o src/rstree_main.cpp

src/rstree_rstree.o:src/rstree.cpp \
  include/rstree.h \
  include/rstree_def.h \
  include/rstree_util.h
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40msrc/rstree_rstree.o[0m']"
	$(CXX) -c $(INCPATH) $(DEP_INCPATH) $(CPPFLAGS) $(CXXFLAGS)  -o src/rstree_rstree.o src/rstree.cpp

src/rstree_conf.o:src/conf.cpp \
  include/conf.h
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40msrc/rstree_conf.o[0m']"
	$(CXX) -c $(INCPATH) $(DEP_INCPATH) $(CPPFLAGS) $(CXXFLAGS)  -o src/rstree_conf.o src/conf.cpp

endif #ifeq ($(shell uname -m),x86_64)


